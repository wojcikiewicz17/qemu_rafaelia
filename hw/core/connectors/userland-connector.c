/*
 * UserLAnd Connector - Android Userspace Integration
 */

#include "hw/core/rafaelia-integration.h"
#include "hw/core/rafaelia-connector-ipc.h"

#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    bool connected;
    char endpoint[256];
    uint32_t container_count;
    double resource_usage;
    pthread_mutex_t lock;
    rafaelia_ipc_runtime_t ipc;
} userland_state_t;

static userland_state_t userland_state;
static uint64_t userland_msg_seq;

static int userland_backend_execute(const rafaelia_ipc_message_t *msg, void *opaque)
{
    userland_state_t *st = opaque;
    pthread_mutex_lock(&st->lock);

    if (!st->connected) {
        pthread_mutex_unlock(&st->lock);
        return -ENOTCONN;
    }

    switch (msg->type) {
    case MSG_REQUEST:
        if (msg->capabilities & CAP_CONTAINER) {
            st->container_count++;
        }
        if (msg->capabilities & CAP_COMPUTE) {
            st->resource_usage += 0.05;
            if (st->resource_usage > 1.0) {
                st->resource_usage = 1.0;
            }
        }
        break;
    case MSG_EVENT:
        st->resource_usage *= 0.99;
        break;
    default:
        break;
    }

    pthread_mutex_unlock(&st->lock);
    return 0;
}

static int userland_connect(void *config)
{
    const char *endpoint = config ? (const char *)config : "ipc://userland/local";

    pthread_mutex_lock(&userland_state.lock);
    if (userland_state.connected) {
        pthread_mutex_unlock(&userland_state.lock);
        return 0;
    }

    memset(&userland_state.ipc, 0, sizeof(userland_state.ipc));
    snprintf(userland_state.endpoint, sizeof(userland_state.endpoint), "%s", endpoint);
    userland_state.container_count = 0;
    userland_state.resource_usage = 0.0;

    if (rafaelia_ipc_runtime_init(&userland_state.ipc,
                                  "userland",
                                  userland_backend_execute,
                                  &userland_state) != 0) {
        pthread_mutex_unlock(&userland_state.lock);
        return -1;
    }

    userland_state.connected = true;
    pthread_mutex_unlock(&userland_state.lock);
    return 0;
}

static int userland_disconnect(void)
{
    pthread_mutex_lock(&userland_state.lock);
    if (!userland_state.connected) {
        pthread_mutex_unlock(&userland_state.lock);
        return -1;
    }
    userland_state.connected = false;
    pthread_mutex_unlock(&userland_state.lock);

    rafaelia_ipc_runtime_destroy(&userland_state.ipc);
    return 0;
}

static int userland_submit(rafaelia_message_type_t type,
                           const void *data,
                           size_t data_size,
                           const rafaelia_request_t *req,
                           const rafaelia_event_t *evt)
{
    rafaelia_ipc_message_t msg;
    rafaelia_ipc_task_t task;

    pthread_mutex_lock(&userland_state.lock);
    if (!userland_state.connected) {
        pthread_mutex_unlock(&userland_state.lock);
        return -ENOTCONN;
    }

    memset(&msg, 0, sizeof(msg));
    msg.type = type;
    msg.message_id = ++userland_msg_seq;
    msg.source = REPO_QEMU_RAFAELIA;
    msg.target = REPO_USERLAND;
    msg.priority = req ? req->priority : PRIORITY_NORMAL;
    msg.capabilities = req ? req->capabilities_required : CAP_NONE;
    msg.timestamp_us = rafaelia_ipc_now_us();
    msg.payload_size = data_size > RAFAELIA_IPC_PAYLOAD_MAX ? RAFAELIA_IPC_PAYLOAD_MAX : (uint32_t)data_size;
    if (data && msg.payload_size > 0) {
        memcpy(msg.payload, data, msg.payload_size);
    }
    rafaelia_blake3_256((const uint8_t *)&msg, sizeof(msg) - sizeof(msg.digest), msg.digest);
    pthread_mutex_unlock(&userland_state.lock);

    (void)evt;
    rafaelia_ipc_task_init(&task, &msg);
    return rafaelia_ipc_submit_sync(&userland_state.ipc, &task, req ? req->timeout_ms : 1000);
}

static int userland_send_request(const rafaelia_request_t *req)
{
    if (!req) {
        return -EINVAL;
    }
    return userland_submit(MSG_REQUEST, req->data, req->data_size, req, NULL);
}

static int userland_send_event(const rafaelia_event_t *evt)
{
    if (!evt) {
        return -EINVAL;
    }
    return userland_submit(MSG_EVENT, evt->data, evt->data_size, NULL, evt);
}

static int userland_health_check(void)
{
    int ret;
    pthread_mutex_lock(&userland_state.lock);
    ret = userland_state.connected ? 0 : -ENOTCONN;
    pthread_mutex_unlock(&userland_state.lock);
    return ret;
}

int rafaelia_userland_connector_init(rafaelia_connector_t *conn)
{
    if (!conn) {
        return -1;
    }

    memset(&userland_state, 0, sizeof(userland_state));
    pthread_mutex_init(&userland_state.lock, NULL);

    conn->connect = userland_connect;
    conn->disconnect = userland_disconnect;
    conn->send_request = userland_send_request;
    conn->send_event = userland_send_event;
    conn->health_check = userland_health_check;
    return 0;
}

int rafaelia_userland_create_container(const char *distribution, const char *name)
{
    rafaelia_request_t req;
    char payload[RAFAELIA_IPC_PAYLOAD_MAX];

    RAFAELIA_REQUEST_INIT(&req);
    req.target = REPO_USERLAND;
    req.category = OP_USERSPACE;
    req.priority = PRIORITY_HIGH;
    req.capabilities_required = CAP_CONTAINER;
    snprintf(payload, sizeof(payload), "create:%s:%s", distribution ? distribution : "unknown",
             name ? name : "container");
    req.data = payload;
    req.data_size = strlen(payload);
    return userland_send_request(&req);
}

int rafaelia_userland_execute_command(const char *container, const char *command)
{
    rafaelia_request_t req;
    char payload[RAFAELIA_IPC_PAYLOAD_MAX];

    RAFAELIA_REQUEST_INIT(&req);
    req.target = REPO_USERLAND;
    req.category = OP_USERSPACE;
    req.priority = PRIORITY_NORMAL;
    req.capabilities_required = CAP_PROCESS_SPAWN | CAP_COMPUTE;
    snprintf(payload, sizeof(payload), "exec:%s:%s", container ? container : "default",
             command ? command : "true");
    req.data = payload;
    req.data_size = strlen(payload);
    return userland_send_request(&req);
}

int rafaelia_userland_get_filesystem_status(const char *container,
                                           uint64_t *total, uint64_t *used)
{
    (void)container;
    if (!total || !used) {
        return -EINVAL;
    }
    pthread_mutex_lock(&userland_state.lock);
    *total = 2ULL * 1024ULL * 1024ULL * 1024ULL;
    *used = (uint64_t)(*total * userland_state.resource_usage);
    pthread_mutex_unlock(&userland_state.lock);
    return 0;
}

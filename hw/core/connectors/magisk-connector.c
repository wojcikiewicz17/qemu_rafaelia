/*
 * Magisk Connector - Root Access and Kernel Integration
 */

#include "hw/core/rafaelia-integration.h"
#include "hw/core/rafaelia-connector-ipc.h"

#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    bool connected;
    bool root_available;
    char endpoint[256];
    uint32_t modules_loaded;
    uint32_t selinux_rules;
    pthread_mutex_t lock;
    rafaelia_ipc_runtime_t ipc;
} magisk_state_t;

static magisk_state_t magisk_state;
static uint64_t magisk_msg_seq;

static int magisk_backend_execute(const rafaelia_ipc_message_t *msg, void *opaque)
{
    magisk_state_t *st = opaque;
    pthread_mutex_lock(&st->lock);
    if (!st->connected || !st->root_available) {
        pthread_mutex_unlock(&st->lock);
        return -EPERM;
    }

    if (msg->type == MSG_REQUEST) {
        if (msg->capabilities & CAP_KERNEL_MODULE) {
            st->modules_loaded++;
        }
        if (msg->capabilities & CAP_ROOT_ACCESS) {
            st->selinux_rules += (msg->payload_size > 0) ? 1 : 0;
        }
    }

    pthread_mutex_unlock(&st->lock);
    return 0;
}

static int magisk_connect(void *config)
{
    const char *endpoint = config ? (const char *)config : "ipc://magisk/local";

    pthread_mutex_lock(&magisk_state.lock);
    if (magisk_state.connected) {
        pthread_mutex_unlock(&magisk_state.lock);
        return 0;
    }

    snprintf(magisk_state.endpoint, sizeof(magisk_state.endpoint), "%s", endpoint);
    magisk_state.root_available = (geteuid() == 0);
    magisk_state.modules_loaded = 0;
    magisk_state.selinux_rules = 0;

    if (rafaelia_ipc_runtime_init(&magisk_state.ipc,
                                  "magisk",
                                  magisk_backend_execute,
                                  &magisk_state) != 0) {
        pthread_mutex_unlock(&magisk_state.lock);
        return -1;
    }

    magisk_state.connected = true;
    pthread_mutex_unlock(&magisk_state.lock);
    return 0;
}

static int magisk_disconnect(void)
{
    pthread_mutex_lock(&magisk_state.lock);
    if (!magisk_state.connected) {
        pthread_mutex_unlock(&magisk_state.lock);
        return -1;
    }
    magisk_state.connected = false;
    magisk_state.root_available = false;
    pthread_mutex_unlock(&magisk_state.lock);

    rafaelia_ipc_runtime_destroy(&magisk_state.ipc);
    return 0;
}

static int magisk_send_request(const rafaelia_request_t *req)
{
    rafaelia_ipc_message_t msg;
    rafaelia_ipc_task_t task;

    if (!req) {
        return -EINVAL;
    }

    memset(&msg, 0, sizeof(msg));
    msg.type = MSG_REQUEST;
    msg.message_id = ++magisk_msg_seq;
    msg.source = REPO_QEMU_RAFAELIA;
    msg.target = REPO_MAGISK;
    msg.priority = req->priority;
    msg.capabilities = req->capabilities_required;
    msg.timestamp_us = rafaelia_ipc_now_us();
    msg.payload_size = req->data_size > RAFAELIA_IPC_PAYLOAD_MAX ? RAFAELIA_IPC_PAYLOAD_MAX : (uint32_t)req->data_size;
    if (req->data && msg.payload_size > 0) {
        memcpy(msg.payload, req->data, msg.payload_size);
    }
    rafaelia_blake3_256((const uint8_t *)&msg, sizeof(msg) - sizeof(msg.digest), msg.digest);

    rafaelia_ipc_task_init(&task, &msg);
    return rafaelia_ipc_submit_sync(&magisk_state.ipc, &task, req->timeout_ms);
}

static int magisk_send_event(const rafaelia_event_t *evt)
{
    rafaelia_ipc_message_t msg;
    rafaelia_ipc_task_t task;

    if (!evt) {
        return -EINVAL;
    }

    memset(&msg, 0, sizeof(msg));
    msg.type = MSG_EVENT;
    msg.message_id = ++magisk_msg_seq;
    msg.source = evt->source;
    msg.target = REPO_MAGISK;
    msg.priority = PRIORITY_NORMAL;
    msg.capabilities = CAP_NONE;
    msg.timestamp_us = rafaelia_ipc_now_us();
    msg.payload_size = evt->data_size > RAFAELIA_IPC_PAYLOAD_MAX ? RAFAELIA_IPC_PAYLOAD_MAX : (uint32_t)evt->data_size;
    if (evt->data && msg.payload_size > 0) {
        memcpy(msg.payload, evt->data, msg.payload_size);
    }
    rafaelia_blake3_256((const uint8_t *)&msg, sizeof(msg) - sizeof(msg.digest), msg.digest);

    rafaelia_ipc_task_init(&task, &msg);
    return rafaelia_ipc_submit_sync(&magisk_state.ipc, &task, 1000);
}

static int magisk_health_check(void)
{
    int ret;
    pthread_mutex_lock(&magisk_state.lock);
    ret = (magisk_state.connected && magisk_state.root_available) ? 0 : -EPERM;
    pthread_mutex_unlock(&magisk_state.lock);
    return ret;
}

int rafaelia_magisk_connector_init(rafaelia_connector_t *conn)
{
    if (!conn) {
        return -1;
    }

    memset(&magisk_state, 0, sizeof(magisk_state));
    pthread_mutex_init(&magisk_state.lock, NULL);

    conn->connect = magisk_connect;
    conn->disconnect = magisk_disconnect;
    conn->send_request = magisk_send_request;
    conn->send_event = magisk_send_event;
    conn->health_check = magisk_health_check;

    return 0;
}

int rafaelia_magisk_load_module(const char *module_path)
{
    rafaelia_request_t req;
    RAFAELIA_REQUEST_INIT(&req);
    req.target = REPO_MAGISK;
    req.category = OP_PRIVILEGED;
    req.priority = PRIORITY_HIGH;
    req.capabilities_required = CAP_ROOT_ACCESS | CAP_KERNEL_MODULE;
    req.data = (void *)module_path;
    req.data_size = module_path ? strlen(module_path) : 0;
    return magisk_send_request(&req);
}

int rafaelia_magisk_add_selinux_rule(const char *rule)
{
    rafaelia_request_t req;
    RAFAELIA_REQUEST_INIT(&req);
    req.target = REPO_MAGISK;
    req.category = OP_PRIVILEGED;
    req.priority = PRIORITY_NORMAL;
    req.capabilities_required = CAP_ROOT_ACCESS;
    req.data = (void *)rule;
    req.data_size = rule ? strlen(rule) : 0;
    return magisk_send_request(&req);
}

int rafaelia_magisk_execute_privileged(const char *command)
{
    rafaelia_request_t req;
    RAFAELIA_REQUEST_INIT(&req);
    req.target = REPO_MAGISK;
    req.category = OP_PRIVILEGED;
    req.priority = PRIORITY_CRITICAL;
    req.capabilities_required = CAP_ROOT_ACCESS | CAP_COMPUTE;
    req.data = (void *)command;
    req.data_size = command ? strlen(command) : 0;
    return magisk_send_request(&req);
}

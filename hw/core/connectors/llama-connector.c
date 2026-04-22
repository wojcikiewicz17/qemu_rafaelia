/*
 * LLaMA Connector - AI/LLM Intelligence Integration
 */

#include "hw/core/rafaelia-integration.h"
#include "hw/core/rafaelia-connector-ipc.h"

#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    bool connected;
    char endpoint[256];
    char model_name[128];
    uint32_t context_size;
    uint64_t tokens_processed;
    double avg_inference_time_ms;
    pthread_mutex_t lock;
    rafaelia_ipc_runtime_t ipc;
} llama_state_t;

static llama_state_t llama_state;
static uint64_t llama_msg_seq;

static uint32_t count_words(const char *txt)
{
    uint32_t words = 0;
    bool in_word = false;
    while (txt && *txt) {
        if (isspace((unsigned char)*txt)) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            words++;
        }
        txt++;
    }
    return words;
}

static int llama_backend_execute(const rafaelia_ipc_message_t *msg, void *opaque)
{
    llama_state_t *st = opaque;
    pthread_mutex_lock(&st->lock);

    if (!st->connected) {
        pthread_mutex_unlock(&st->lock);
        return -ENOTCONN;
    }

    if (msg->type == MSG_REQUEST) {
        uint32_t words = count_words((const char *)msg->payload);
        st->tokens_processed += (uint64_t)(words * 3U + 8U);
        st->avg_inference_time_ms = (st->avg_inference_time_ms * 0.8) + (double)(words + 1) * 0.2;
    }

    pthread_mutex_unlock(&st->lock);
    return 0;
}

static int llama_connect(void *config)
{
    const char *endpoint = config ? (const char *)config : "ipc://llama/local";

    pthread_mutex_lock(&llama_state.lock);
    if (llama_state.connected) {
        pthread_mutex_unlock(&llama_state.lock);
        return 0;
    }

    snprintf(llama_state.endpoint, sizeof(llama_state.endpoint), "%s", endpoint);
    snprintf(llama_state.model_name, sizeof(llama_state.model_name), "%s", "llama-rafaelia-7b");
    llama_state.context_size = 4096;
    llama_state.tokens_processed = 0;
    llama_state.avg_inference_time_ms = 8.0;

    if (rafaelia_ipc_runtime_init(&llama_state.ipc,
                                  "llama",
                                  llama_backend_execute,
                                  &llama_state) != 0) {
        pthread_mutex_unlock(&llama_state.lock);
        return -1;
    }

    llama_state.connected = true;
    pthread_mutex_unlock(&llama_state.lock);
    return 0;
}

static int llama_disconnect(void)
{
    pthread_mutex_lock(&llama_state.lock);
    if (!llama_state.connected) {
        pthread_mutex_unlock(&llama_state.lock);
        return -1;
    }
    llama_state.connected = false;
    pthread_mutex_unlock(&llama_state.lock);

    rafaelia_ipc_runtime_destroy(&llama_state.ipc);
    return 0;
}

static int llama_send_request(const rafaelia_request_t *req)
{
    rafaelia_ipc_message_t msg;
    rafaelia_ipc_task_t task;

    if (!req) {
        return -EINVAL;
    }

    memset(&msg, 0, sizeof(msg));
    msg.type = MSG_REQUEST;
    msg.message_id = ++llama_msg_seq;
    msg.source = REPO_QEMU_RAFAELIA;
    msg.target = REPO_LLAMA;
    msg.priority = req->priority;
    msg.capabilities = req->capabilities_required;
    msg.timestamp_us = rafaelia_ipc_now_us();
    msg.payload_size = req->data_size > RAFAELIA_IPC_PAYLOAD_MAX ? RAFAELIA_IPC_PAYLOAD_MAX : (uint32_t)req->data_size;
    if (req->data && msg.payload_size > 0) {
        memcpy(msg.payload, req->data, msg.payload_size);
    }
    rafaelia_blake3_256((const uint8_t *)&msg, sizeof(msg) - sizeof(msg.digest), msg.digest);

    rafaelia_ipc_task_init(&task, &msg);
    return rafaelia_ipc_submit_sync(&llama_state.ipc, &task, req->timeout_ms);
}

static int llama_send_event(const rafaelia_event_t *evt)
{
    rafaelia_ipc_message_t msg;
    rafaelia_ipc_task_t task;

    if (!evt) {
        return -EINVAL;
    }

    memset(&msg, 0, sizeof(msg));
    msg.type = MSG_EVENT;
    msg.message_id = ++llama_msg_seq;
    msg.source = evt->source;
    msg.target = REPO_LLAMA;
    msg.priority = PRIORITY_LOW;
    msg.timestamp_us = rafaelia_ipc_now_us();
    msg.payload_size = evt->data_size > RAFAELIA_IPC_PAYLOAD_MAX ? RAFAELIA_IPC_PAYLOAD_MAX : (uint32_t)evt->data_size;
    if (evt->data && msg.payload_size > 0) {
        memcpy(msg.payload, evt->data, msg.payload_size);
    }
    rafaelia_blake3_256((const uint8_t *)&msg, sizeof(msg) - sizeof(msg.digest), msg.digest);

    rafaelia_ipc_task_init(&task, &msg);
    return rafaelia_ipc_submit_sync(&llama_state.ipc, &task, 1000);
}

static int llama_health_check(void)
{
    int ret;
    pthread_mutex_lock(&llama_state.lock);
    ret = llama_state.connected ? 0 : -ENOTCONN;
    pthread_mutex_unlock(&llama_state.lock);
    return ret;
}

int rafaelia_llama_connector_init(rafaelia_connector_t *conn)
{
    if (!conn) {
        return -1;
    }

    memset(&llama_state, 0, sizeof(llama_state));
    pthread_mutex_init(&llama_state.lock, NULL);

    conn->connect = llama_connect;
    conn->disconnect = llama_disconnect;
    conn->send_request = llama_send_request;
    conn->send_event = llama_send_event;
    conn->health_check = llama_health_check;

    return 0;
}

int rafaelia_llama_generate_text(const char *prompt, char *output, size_t max_len)
{
    rafaelia_request_t req;
    int ret;

    if (!prompt || !output || max_len == 0) {
        return -EINVAL;
    }

    RAFAELIA_REQUEST_INIT(&req);
    req.target = REPO_LLAMA;
    req.category = OP_INTELLIGENCE;
    req.priority = PRIORITY_HIGH;
    req.capabilities_required = CAP_AI_INFERENCE | CAP_NLP;
    req.data = (void *)prompt;
    req.data_size = strlen(prompt);
    ret = llama_send_request(&req);
    if (ret != 0) {
        return ret;
    }

    snprintf(output, max_len, "[%s] %s", llama_state.model_name, prompt);
    return 0;
}

int rafaelia_llama_classify_intent(const char *text, char *intent, size_t max_len)
{
    if (!text || !intent || max_len == 0) {
        return -EINVAL;
    }

    if (strstr(text, "start") || strstr(text, "run")) {
        snprintf(intent, max_len, "execute");
    } else if (strstr(text, "create") || strstr(text, "new")) {
        snprintf(intent, max_len, "provision");
    } else {
        snprintf(intent, max_len, "analyze");
    }
    return 0;
}

int rafaelia_llama_extract_entities(const char *text, char *entities, size_t max_len)
{
    if (!text || !entities || max_len == 0) {
        return -EINVAL;
    }

    snprintf(entities, max_len, "len=%zu words=%u", strlen(text), count_words(text));
    return 0;
}

double rafaelia_llama_compute_ethical_score(const char *action)
{
    size_t len;
    if (!action) {
        return 0.0;
    }
    len = strlen(action);
    return 1.0 / (1.0 + (double)(len % 10U));
}

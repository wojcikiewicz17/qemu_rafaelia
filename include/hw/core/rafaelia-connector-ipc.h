#ifndef HW_RAFAELIA_CONNECTOR_IPC_H
#define HW_RAFAELIA_CONNECTOR_IPC_H

#include "hw/core/rafaelia-integration.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

#define RAFAELIA_IPC_PAYLOAD_MAX 512
#define RAFAELIA_IPC_QUEUE_CAPACITY 128

typedef struct {
    rafaelia_message_type_t type;
    uint64_t message_id;
    rafaelia_repository_id_t source;
    rafaelia_repository_id_t target;
    rafaelia_priority_t priority;
    uint32_t capabilities;
    uint64_t timestamp_us;
    uint32_t payload_size;
    uint8_t payload[RAFAELIA_IPC_PAYLOAD_MAX];
    uint8_t digest[32];
} rafaelia_ipc_message_t;

typedef struct {
    rafaelia_ipc_message_t message;
    int status;
    uint64_t sequence;
    bool completed;
    pthread_mutex_t done_mutex;
    pthread_cond_t done_cond;
} rafaelia_ipc_task_t;

typedef int (*rafaelia_ipc_handler_fn)(const rafaelia_ipc_message_t *msg,
                                       void *opaque);

typedef struct {
    pthread_t worker;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool running;
    uint64_t next_sequence;
    rafaelia_ipc_task_t *heap[RAFAELIA_IPC_QUEUE_CAPACITY];
    size_t heap_len;
    rafaelia_ipc_handler_fn handler;
    void *opaque;
    const char *name;
} rafaelia_ipc_runtime_t;

uint64_t rafaelia_ipc_now_us(void);
void rafaelia_blake3_256(const uint8_t *data, size_t len, uint8_t out[32]);
void rafaelia_ipc_log(const char *component,
                      const rafaelia_ipc_message_t *msg,
                      const char *phase,
                      int status);

int rafaelia_ipc_runtime_init(rafaelia_ipc_runtime_t *rt,
                              const char *name,
                              rafaelia_ipc_handler_fn handler,
                              void *opaque);
void rafaelia_ipc_runtime_destroy(rafaelia_ipc_runtime_t *rt);
int rafaelia_ipc_submit_sync(rafaelia_ipc_runtime_t *rt,
                             rafaelia_ipc_task_t *task,
                             uint32_t timeout_ms);
void rafaelia_ipc_task_init(rafaelia_ipc_task_t *task,
                            const rafaelia_ipc_message_t *msg);

#endif

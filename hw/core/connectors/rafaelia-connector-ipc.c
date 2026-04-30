#include "hw/core/rafaelia-connector-ipc.h"

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
    uint32_t type;
    uint64_t message_id;
    uint64_t cycle_id;
    uint32_t source;
    uint32_t target;
    uint32_t priority;
    uint32_t capabilities;
    uint32_t ttl_cycles;
    uint32_t retry_budget;
    uint64_t timestamp_us;
    uint32_t payload_size;
} rafaelia_ipc_digest_header_t;

static const uint32_t blake3_iv[8] = {
    0x6A09E667U, 0xBB67AE85U, 0x3C6EF372U, 0xA54FF53AU,
    0x510E527FU, 0x9B05688CU, 0x1F83D9ABU, 0x5BE0CD19U
};

static const uint32_t blake3_msg_perm[16] = {
    2, 6, 3, 10, 7, 0, 4, 13, 1, 11, 12, 5, 9, 14, 15, 8
};

static inline uint32_t rotr32(uint32_t w, uint32_t c)
{
    return (w >> c) | (w << (32U - c));
}

static void blake3_g(uint32_t *state, uint32_t a, uint32_t b,
                     uint32_t c, uint32_t d, uint32_t mx, uint32_t my)
{
    state[a] = state[a] + state[b] + mx;
    state[d] = rotr32(state[d] ^ state[a], 16);
    state[c] = state[c] + state[d];
    state[b] = rotr32(state[b] ^ state[c], 12);
    state[a] = state[a] + state[b] + my;
    state[d] = rotr32(state[d] ^ state[a], 8);
    state[c] = state[c] + state[d];
    state[b] = rotr32(state[b] ^ state[c], 7);
}

static void blake3_round(uint32_t *state, const uint32_t *m)
{
    blake3_g(state, 0, 4, 8, 12, m[0], m[1]);
    blake3_g(state, 1, 5, 9, 13, m[2], m[3]);
    blake3_g(state, 2, 6, 10, 14, m[4], m[5]);
    blake3_g(state, 3, 7, 11, 15, m[6], m[7]);
    blake3_g(state, 0, 5, 10, 15, m[8], m[9]);
    blake3_g(state, 1, 6, 11, 12, m[10], m[11]);
    blake3_g(state, 2, 7, 8, 13, m[12], m[13]);
    blake3_g(state, 3, 4, 9, 14, m[14], m[15]);
}

static uint32_t load32(const uint8_t *p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static void store32(uint8_t *p, uint32_t x)
{
    p[0] = (uint8_t)x;
    p[1] = (uint8_t)(x >> 8);
    p[2] = (uint8_t)(x >> 16);
    p[3] = (uint8_t)(x >> 24);
}

void rafaelia_blake3_256(const uint8_t *data, size_t len, uint8_t out[32])
{
    uint32_t cv[8];
    uint8_t block[64] = { 0 };
    uint64_t counter = 0;
    uint32_t block_len;
    int i;

    memcpy(cv, blake3_iv, sizeof(cv));

    while (len > 0) {
        uint32_t state[16];
        uint32_t m[16];
        uint32_t perm[16];

        block_len = (len > sizeof(block)) ? sizeof(block) : (uint32_t)len;
        memset(block, 0, sizeof(block));
        memcpy(block, data, block_len);
        data += block_len;
        len -= block_len;

        for (i = 0; i < 16; i++) {
            m[i] = load32(block + 4 * i);
            perm[i] = (uint32_t)i;
        }

        memcpy(state, cv, sizeof(cv));
        memcpy(state + 8, blake3_iv, sizeof(blake3_iv));
        state[12] ^= (uint32_t)counter;
        state[13] ^= (uint32_t)(counter >> 32);
        state[14] ^= block_len;
        state[15] ^= (len == 0 ? 0x0BU : 0x01U);

        for (i = 0; i < 7; i++) {
            uint32_t schedule[16];
            int j;
            for (j = 0; j < 16; j++) {
                schedule[j] = m[perm[j]];
            }
            blake3_round(state, schedule);
            if (i != 6) {
                uint32_t next_perm[16];
                for (j = 0; j < 16; j++) {
                    next_perm[j] = perm[blake3_msg_perm[j]];
                }
                memcpy(perm, next_perm, sizeof(perm));
            }
        }

        for (i = 0; i < 8; i++) {
            cv[i] ^= state[i] ^ state[i + 8];
        }
        counter++;
    }

    for (i = 0; i < 8; i++) {
        store32(out + (size_t)i * 4, cv[i]);
    }
}

uint64_t rafaelia_ipc_now_us(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ULL + (uint64_t)ts.tv_nsec / 1000ULL;
}

void rafaelia_ipc_log(const char *component,
                      const rafaelia_ipc_message_t *msg,
                      const char *phase,
                      int status)
{
    char hex[65];
    size_t i;
    for (i = 0; i < 32; i++) {
        snprintf(&hex[i * 2], 3, "%02x", msg->digest[i]);
    }
    hex[64] = '\0';
    fprintf(stderr,
            "[rafaelia][%s][%s] id=%" PRIu64 " type=%d src=%d dst=%d prio=%d cap=0x%08x status=%d hash=%s\n",
            component, phase, msg->message_id, (int)msg->type,
            (int)msg->source, (int)msg->target, (int)msg->priority,
            msg->capabilities, status, hex);
}

void rafaelia_ipc_compute_digest(rafaelia_ipc_message_t *msg)
{
    uint8_t buf[sizeof(rafaelia_ipc_digest_header_t) + RAFAELIA_IPC_PAYLOAD_MAX + 32];
    rafaelia_ipc_digest_header_t hdr;
    size_t total_len;

    memset(&hdr, 0, sizeof(hdr));
    hdr.type = (uint32_t)msg->type;
    hdr.message_id = msg->message_id;
    hdr.cycle_id = msg->cycle_id;
    hdr.source = (uint32_t)msg->source;
    hdr.target = (uint32_t)msg->target;
    hdr.priority = (uint32_t)msg->priority;
    hdr.capabilities = msg->capabilities;
    hdr.ttl_cycles = msg->ttl_cycles;
    hdr.retry_budget = msg->retry_budget;
    hdr.timestamp_us = msg->timestamp_us;
    hdr.payload_size = msg->payload_size;

    memcpy(buf, msg->prev_digest, 32);
    memcpy(buf + 32, &hdr, sizeof(hdr));
    memcpy(buf + 32 + sizeof(hdr), msg->payload, msg->payload_size);
    total_len = 32 + sizeof(hdr) + msg->payload_size;
    rafaelia_blake3_256(buf, total_len, msg->digest);
}

int rafaelia_ipc_validate_chain(const rafaelia_ipc_runtime_t *rt,
                                const rafaelia_ipc_message_t *msg)
{
    static const uint8_t zero_digest[32] = { 0 };
    if (msg->ttl_cycles == 0) {
        return -ETIMEDOUT;
    }
    if (rt->chain_initialized) {
        if (memcmp(msg->prev_digest, rt->last_digest, 32) != 0) {
            return -EILSEQ;
        }
        if (msg->cycle_id <= rt->last_cycle_id) {
            return -EINVAL;
        }
    } else if (memcmp(msg->prev_digest, zero_digest, 32) != 0) {
        return -EILSEQ;
    }
    return 0;
}

int rafaelia_ipc_advance_cycle(rafaelia_ipc_runtime_t *rt,
                               const rafaelia_ipc_message_t *msg,
                               int handler_status)
{
    if (handler_status != 0 && msg->retry_budget > 0) {
        return 0;
    }
    memcpy(rt->last_digest, msg->digest, 32);
    rt->last_cycle_id = msg->cycle_id;
    rt->chain_initialized = true;
    return 0;
}

static int task_compare(const rafaelia_ipc_task_t *a, const rafaelia_ipc_task_t *b)
{
    if (a->message.priority != b->message.priority) {
        return (int)a->message.priority - (int)b->message.priority;
    }
    if (a->message.capabilities != b->message.capabilities) {
        return (a->message.capabilities > b->message.capabilities) ? 1 : -1;
    }
    if (a->sequence < b->sequence) {
        return 1;
    }
    if (a->sequence > b->sequence) {
        return -1;
    }
    return 0;
}

static void heap_swap(rafaelia_ipc_task_t **a, rafaelia_ipc_task_t **b)
{
    rafaelia_ipc_task_t *tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_push(rafaelia_ipc_runtime_t *rt, rafaelia_ipc_task_t *task)
{
    size_t i = rt->heap_len++;
    rt->heap[i] = task;

    while (i > 0) {
        size_t p = (i - 1) / 2;
        if (task_compare(rt->heap[i], rt->heap[p]) <= 0) {
            break;
        }
        heap_swap(&rt->heap[i], &rt->heap[p]);
        i = p;
    }
}

static rafaelia_ipc_task_t *heap_pop(rafaelia_ipc_runtime_t *rt)
{
    rafaelia_ipc_task_t *top;
    size_t i = 0;

    if (rt->heap_len == 0) {
        return NULL;
    }

    top = rt->heap[0];
    rt->heap[0] = rt->heap[--rt->heap_len];

    while (1) {
        size_t l = i * 2 + 1;
        size_t r = i * 2 + 2;
        size_t best = i;

        if (l < rt->heap_len && task_compare(rt->heap[l], rt->heap[best]) > 0) {
            best = l;
        }
        if (r < rt->heap_len && task_compare(rt->heap[r], rt->heap[best]) > 0) {
            best = r;
        }
        if (best == i) {
            break;
        }
        heap_swap(&rt->heap[i], &rt->heap[best]);
        i = best;
    }

    return top;
}

void rafaelia_ipc_task_init(rafaelia_ipc_task_t *task,
                            const rafaelia_ipc_message_t *msg)
{
    memset(task, 0, sizeof(*task));
    task->message = *msg;
    pthread_mutex_init(&task->done_mutex, NULL);
    pthread_cond_init(&task->done_cond, NULL);
}

static void task_complete(rafaelia_ipc_task_t *task, int status)
{
    pthread_mutex_lock(&task->done_mutex);
    task->status = status;
    task->completed = true;
    pthread_cond_signal(&task->done_cond);
    pthread_mutex_unlock(&task->done_mutex);
}

static void *ipc_worker_main(void *opaque)
{
    rafaelia_ipc_runtime_t *rt = opaque;

    for (;;) {
        rafaelia_ipc_task_t *task;
        int status;

        pthread_mutex_lock(&rt->mutex);
        while (rt->running && rt->heap_len == 0) {
            pthread_cond_wait(&rt->cond, &rt->mutex);
        }
        if (!rt->running && rt->heap_len == 0) {
            pthread_mutex_unlock(&rt->mutex);
            break;
        }
        task = heap_pop(rt);
        if (task != NULL && task->message.ttl_cycles > 0) {
            task->message.ttl_cycles--;
        }
        pthread_mutex_unlock(&rt->mutex);

        status = rafaelia_ipc_validate_chain(rt, &task->message);
        if (status != 0) {
            rafaelia_ipc_log(rt->name, &task->message, "reject", status);
            task_complete(task, status);
            continue;
        }
        rafaelia_ipc_compute_digest(&task->message);
        rafaelia_ipc_log(rt->name, &task->message, "dispatch", 0);
        status = rt->handler(&task->message, rt->opaque);
        if (status != 0 && task->message.retry_budget > 0) {
            task->message.retry_budget--;
        }
        rafaelia_ipc_advance_cycle(rt, &task->message, status);
        rafaelia_ipc_log(rt->name, &task->message, "complete", status);
        task_complete(task, status);
    }

    return NULL;
}

int rafaelia_ipc_runtime_init(rafaelia_ipc_runtime_t *rt,
                              const char *name,
                              rafaelia_ipc_handler_fn handler,
                              void *opaque)
{
    if (!rt || !handler) {
        return -EINVAL;
    }

    memset(rt, 0, sizeof(*rt));
    rt->handler = handler;
    rt->opaque = opaque;
    rt->name = name;
    rt->running = true;
    pthread_mutex_init(&rt->mutex, NULL);
    pthread_cond_init(&rt->cond, NULL);

    if (pthread_create(&rt->worker, NULL, ipc_worker_main, rt) != 0) {
        pthread_mutex_destroy(&rt->mutex);
        pthread_cond_destroy(&rt->cond);
        return -errno;
    }

    return 0;
}

void rafaelia_ipc_runtime_destroy(rafaelia_ipc_runtime_t *rt)
{
    if (!rt) {
        return;
    }

    pthread_mutex_lock(&rt->mutex);
    rt->running = false;
    pthread_cond_broadcast(&rt->cond);
    pthread_mutex_unlock(&rt->mutex);

    pthread_join(rt->worker, NULL);
    pthread_mutex_destroy(&rt->mutex);
    pthread_cond_destroy(&rt->cond);
}

int rafaelia_ipc_submit_sync(rafaelia_ipc_runtime_t *rt,
                             rafaelia_ipc_task_t *task,
                             uint32_t timeout_ms)
{
    int rc = 0;
    struct timespec ts;

    if (!rt || !task) {
        return -EINVAL;
    }

    pthread_mutex_lock(&rt->mutex);
    if (!rt->running || rt->heap_len >= RAFAELIA_IPC_QUEUE_CAPACITY) {
        pthread_mutex_unlock(&rt->mutex);
        return -ENOSPC;
    }
    task->sequence = rt->next_sequence++;
    heap_push(rt, task);
    pthread_cond_signal(&rt->cond);
    pthread_mutex_unlock(&rt->mutex);

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += timeout_ms / 1000;
    ts.tv_nsec += (long)(timeout_ms % 1000) * 1000000L;
    if (ts.tv_nsec >= 1000000000L) {
        ts.tv_sec += 1;
        ts.tv_nsec -= 1000000000L;
    }

    pthread_mutex_lock(&task->done_mutex);
    while (!task->completed) {
        int err = pthread_cond_timedwait(&task->done_cond, &task->done_mutex, &ts);
        if (err == ETIMEDOUT) {
            rc = -ETIMEDOUT;
            break;
        }
    }
    if (task->completed) {
        rc = task->status;
    }
    pthread_mutex_unlock(&task->done_mutex);

    pthread_cond_destroy(&task->done_cond);
    pthread_mutex_destroy(&task->done_mutex);
    return rc;
}

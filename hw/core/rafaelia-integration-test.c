#include "hw/core/rafaelia-connector-ipc.h"

#include <assert.h>
#include <errno.h>
#include <string.h>

static int ok_handler(const rafaelia_ipc_message_t *msg, void *opaque)
{
    (void)msg;
    (void)opaque;
    return 0;
}

static void init_msg(rafaelia_ipc_message_t *msg, uint64_t id, uint64_t cycle,
                     rafaelia_priority_t prio, const uint8_t prev[32], uint32_t ttl)
{
    memset(msg, 0, sizeof(*msg));
    msg->type = MSG_REQUEST;
    msg->message_id = id;
    msg->cycle_id = cycle;
    msg->source = REPO_QEMU_RAFAELIA;
    msg->target = REPO_USERLAND;
    msg->priority = prio;
    msg->capabilities = CAP_COMPUTE;
    msg->ttl_cycles = ttl;
    msg->retry_budget = 2;
    msg->payload_size = 4;
    memcpy(msg->payload, "ping", 4);
    memcpy(msg->prev_digest, prev, 32);
}

int main(void)
{
    rafaelia_ipc_runtime_t rt;
    rafaelia_ipc_message_t m1, m2, m3, m4;
    uint8_t zero[32] = { 0 };

    assert(rafaelia_ipc_runtime_init(&rt, "test", ok_handler, NULL) == 0);

    init_msg(&m1, 1, 1, PRIORITY_NORMAL, zero, 1);
    assert(rafaelia_ipc_validate_chain(&rt, &m1) == 0);
    rafaelia_ipc_compute_digest(&m1);
    assert(rafaelia_ipc_advance_cycle(&rt, &m1, 0) == 0);

    init_msg(&m2, 2, 2, PRIORITY_HIGH, m1.digest, 1);
    assert(rafaelia_ipc_validate_chain(&rt, &m2) == 0);

    memset(m3.prev_digest, 0xAB, 32);
    init_msg(&m3, 3, 3, PRIORITY_CRITICAL, m3.prev_digest, 1);
    assert(rafaelia_ipc_validate_chain(&rt, &m3) == -EILSEQ);

    init_msg(&m4, 4, 4, PRIORITY_LOW, m1.digest, 0);
    assert(rafaelia_ipc_validate_chain(&rt, &m4) == -ETIMEDOUT);

    rafaelia_ipc_runtime_destroy(&rt);
    return 0;
}

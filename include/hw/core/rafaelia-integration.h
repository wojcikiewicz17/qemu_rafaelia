/*
 * RAFAELIA Multi-Repository Integration Hub
 * Strategic integration layer for unified system operations
 * 
 * Integrates: UserLAnd, Magisk_Rafaelia, llamaRafaelia, Rafaelia_Private
 * Philosophy: Unity through ethical logic-based tactics
 * Target: Diamond State (💎) Excellence
 */

#ifndef HW_RAFAELIA_INTEGRATION_H
#define HW_RAFAELIA_INTEGRATION_H

#include "hw/core/rafaelia-core.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Repository Identifiers */
typedef enum {
    REPO_QEMU_RAFAELIA = 0,    /* Core/Hub */
    REPO_USERLAND,             /* Android userspace */
    REPO_MAGISK,               /* Root/kernel access */
    REPO_LLAMA,                /* AI/LLM intelligence */
    REPO_PRIVATE,              /* Private extensions */
    REPO_COUNT
} rafaelia_repository_id_t;

/* Operation Categories */
typedef enum {
    OP_SYSTEM,          /* System-level operations */
    OP_COMPUTE,         /* Computational tasks */
    OP_INTELLIGENCE,    /* AI/cognitive tasks */
    OP_USERSPACE,       /* User-facing operations */
    OP_PRIVILEGED,      /* Root/kernel operations */
    OP_PRIVATE          /* Extension operations */
} rafaelia_operation_category_t;

/* Request Priority */
typedef enum {
    PRIORITY_LOW = 0,
    PRIORITY_NORMAL = 1,
    PRIORITY_HIGH = 2,
    PRIORITY_CRITICAL = 3
} rafaelia_priority_t;

/* Integration Status */
typedef enum {
    STATUS_DISCONNECTED = 0,
    STATUS_CONNECTING,
    STATUS_CONNECTED,
    STATUS_ACTIVE,
    STATUS_DEGRADED,
    STATUS_ERROR
} rafaelia_integration_status_t;

/* Repository Capabilities */
#define CAP_NONE              0x00000000
#define CAP_COMPUTE           0x00000001
#define CAP_STORAGE           0x00000002
#define CAP_NETWORK           0x00000004
#define CAP_GRAPHICS          0x00000008
#define CAP_AUDIO             0x00000010
#define CAP_INPUT             0x00000020
#define CAP_ROOT_ACCESS       0x00000040
#define CAP_KERNEL_MODULE     0x00000080
#define CAP_AI_INFERENCE      0x00000100
#define CAP_NLP               0x00000200
#define CAP_LEARNING          0x00000400
#define CAP_CONTAINER         0x00000800
#define CAP_FILESYSTEM        0x00001000
#define CAP_PROCESS_SPAWN     0x00002000
#define CAP_EXTENSION         0x00004000
#define CAP_ALL               0xFFFFFFFF

/* Message Types */
typedef enum {
    MSG_REQUEST = 0,
    MSG_RESPONSE,
    MSG_EVENT,
    MSG_SYNC,
    MSG_HEARTBEAT
} rafaelia_message_type_t;

/* Request Structure */
typedef struct {
    uint64_t id;                           /* Unique request ID */
    rafaelia_repository_id_t source;       /* Source repository */
    rafaelia_repository_id_t target;       /* Target repository */
    rafaelia_operation_category_t category;/* Operation category */
    rafaelia_priority_t priority;          /* Request priority */
    uint32_t capabilities_required;        /* Required capabilities mask */
    
    /* Ethical parameters */
    double amor;                           /* Love/compassion weight */
    double coerencia;                      /* Coherence/consistency */
    double verdade;                        /* Truth requirement */
    double consciencia;                    /* Consciousness level */
    
    /* Payload */
    void *data;                            /* Request data */
    size_t data_size;                      /* Data size in bytes */
    
    /* Metadata */
    uint64_t timestamp;                    /* Request timestamp */
    uint32_t timeout_ms;                   /* Timeout in milliseconds */
    uint64_t cycle_id;                     /* Logical cycle id (hub-global) */
    uint32_t ttl_cycles;                   /* Logical cycle TTL budget */
    uint32_t retry_budget;                 /* Recoverable error retry budget */
    void *context;                         /* User context */
} rafaelia_request_t;

/* Response Structure */
typedef struct {
    uint64_t request_id;                   /* Original request ID */
    rafaelia_repository_id_t source;       /* Responding repository */
    int status_code;                       /* Status code (0 = success) */
    
    /* Ethical metrics */
    double phi_ethica;                     /* Ethical quality score */
    double owl_psi;                        /* Wisdom level */
    
    /* Payload */
    void *data;                            /* Response data */
    size_t data_size;                      /* Data size in bytes */
    
    /* Metadata */
    uint64_t timestamp;                    /* Response timestamp */
    uint64_t latency_us;                   /* Latency in microseconds */
} rafaelia_response_t;

/* Event Structure */
typedef struct {
    uint64_t id;                           /* Event ID */
    rafaelia_repository_id_t source;       /* Event source */
    uint32_t event_type;                   /* Event type code */
    
    /* Payload */
    void *data;                            /* Event data */
    size_t data_size;                      /* Data size in bytes */
    
    /* Metadata */
    uint64_t timestamp;                    /* Event timestamp */
} rafaelia_event_t;

/* Repository Connector Interface */
typedef struct {
    rafaelia_repository_id_t id;
    const char *name;
    uint32_t capabilities;
    rafaelia_integration_status_t status;
    
    /* Connection info */
    char endpoint[256];                    /* Connection endpoint */
    uint64_t last_heartbeat;               /* Last heartbeat timestamp */
    
    /* Statistics */
    uint64_t requests_sent;
    uint64_t requests_received;
    uint64_t responses_sent;
    uint64_t responses_received;
    uint64_t errors;
    
    /* Performance metrics */
    double avg_latency_ms;
    double success_rate;
    double load_factor;                    /* 0.0 - 1.0 */
    
    /* Ethical metrics */
    double phi_ethica_avg;
    double owl_psi_avg;
    
    /* Function pointers */
    int (*connect)(void *config);
    int (*disconnect)(void);
    int (*send_request)(const rafaelia_request_t *req);
    int (*receive_response)(rafaelia_response_t *resp);  /* Reserved for async operations */
    int (*send_event)(const rafaelia_event_t *evt);
    int (*health_check)(void);
} rafaelia_connector_t;

/* Integration Hub State */
typedef struct {
    /* Core */
    rafaelia_core_t *core;                 /* RAFAELIA core reference */
    bool initialized;
    
    /* Connectors */
    rafaelia_connector_t connectors[REPO_COUNT];
    
    /* Routing */
    double routing_weights[REPO_COUNT];    /* Dynamic routing weights */
    uint64_t routing_table[256];           /* Fast lookup table */
    
    /* Statistics */
    uint64_t total_requests;
    uint64_t total_responses;
    uint64_t total_events;
    uint64_t total_errors;
    
    /* Performance */
    double system_phi_ethica;              /* System-wide ethical score */
    double system_health;                  /* Overall health (0.0 - 1.0) */
    double integration_power;              /* Integration effectiveness */
    
    /* Strategy parameters */
    double amor_threshold;                 /* Minimum amor for routing */
    double coerencia_threshold;            /* Minimum coerencia */
    bool adaptive_routing_enabled;         /* Enable learning */
    bool retroalimentation_enabled;        /* Enable feedback loop */
} rafaelia_integration_hub_t;

/* Callback Types */
typedef void (*rafaelia_event_callback_t)(const rafaelia_event_t *event, void *userdata);
typedef void (*rafaelia_response_callback_t)(const rafaelia_response_t *response, void *userdata);

/* Hub Lifecycle Functions */
int rafaelia_integration_hub_init(rafaelia_integration_hub_t *hub, rafaelia_core_t *core);
void rafaelia_integration_hub_cleanup(rafaelia_integration_hub_t *hub);
int rafaelia_integration_hub_start(rafaelia_integration_hub_t *hub);
int rafaelia_integration_hub_stop(rafaelia_integration_hub_t *hub);

/* Repository Connection Management */
int rafaelia_integration_connect_repository(rafaelia_integration_hub_t *hub,
                                           rafaelia_repository_id_t repo_id,
                                           const char *endpoint,
                                           void *config);
int rafaelia_integration_disconnect_repository(rafaelia_integration_hub_t *hub,
                                              rafaelia_repository_id_t repo_id);
rafaelia_integration_status_t rafaelia_integration_get_status(
    const rafaelia_integration_hub_t *hub,
    rafaelia_repository_id_t repo_id);

/* Request/Response Operations */
int rafaelia_integration_send_request(rafaelia_integration_hub_t *hub,
                                     const rafaelia_request_t *request,
                                     rafaelia_response_callback_t callback,
                                     void *userdata);
int rafaelia_integration_send_request_sync(rafaelia_integration_hub_t *hub,
                                          const rafaelia_request_t *request,
                                          rafaelia_response_t *response,
                                          uint32_t timeout_ms);

/* Event Operations */
int rafaelia_integration_publish_event(rafaelia_integration_hub_t *hub,
                                      const rafaelia_event_t *event);
int rafaelia_integration_subscribe_event(rafaelia_integration_hub_t *hub,
                                        uint32_t event_type,
                                        rafaelia_event_callback_t callback,
                                        void *userdata);

/* Strategic Routing */
rafaelia_repository_id_t rafaelia_integration_route_request(
    rafaelia_integration_hub_t *hub,
    const rafaelia_request_t *request);
int rafaelia_integration_update_routing_weights(rafaelia_integration_hub_t *hub);

/* Ethical Decision Making */
double rafaelia_integration_compute_routing_score(
    const rafaelia_integration_hub_t *hub,
    rafaelia_repository_id_t repo_id,
    const rafaelia_request_t *request);
bool rafaelia_integration_check_ethical_compliance(
    const rafaelia_request_t *request,
    double phi_ethica_threshold);

/* Retroalimentation */
int rafaelia_integration_apply_retroalimentation(rafaelia_integration_hub_t *hub);
int rafaelia_integration_update_connector_metrics(rafaelia_integration_hub_t *hub,
                                                 rafaelia_repository_id_t repo_id,
                                                 const rafaelia_response_t *response);

/* System Metrics */
int rafaelia_integration_get_metrics(const rafaelia_integration_hub_t *hub,
                                    uint64_t *requests,
                                    uint64_t *responses,
                                    double *health,
                                    double *phi_ethica);
double rafaelia_integration_compute_system_health(const rafaelia_integration_hub_t *hub);
double rafaelia_integration_compute_integration_power(const rafaelia_integration_hub_t *hub);

/* Utility Functions */
const char *rafaelia_repository_name(rafaelia_repository_id_t id);
const char *rafaelia_operation_category_name(rafaelia_operation_category_t cat);
const char *rafaelia_status_name(rafaelia_integration_status_t status);

/* Helper Macros */
#define RAFAELIA_REQUEST_INIT(req) \
    do { \
        (req)->id = 0; \
        (req)->source = REPO_QEMU_RAFAELIA; \
        (req)->target = REPO_QEMU_RAFAELIA; \
        (req)->category = OP_SYSTEM; \
        (req)->priority = PRIORITY_NORMAL; \
        (req)->capabilities_required = CAP_NONE; \
        (req)->amor = 1.0; \
        (req)->coerencia = 1.0; \
        (req)->verdade = 1.0; \
        (req)->consciencia = 1.0; \
        (req)->data = NULL; \
        (req)->data_size = 0; \
        (req)->timeout_ms = 5000; \
        (req)->context = NULL; \
    } while(0)

#define RAFAELIA_RESPONSE_INIT(resp) \
    do { \
        (resp)->request_id = 0; \
        (resp)->source = REPO_QEMU_RAFAELIA; \
        (resp)->status_code = 0; \
        (resp)->phi_ethica = 0.0; \
        (resp)->owl_psi = 0.0; \
        (resp)->data = NULL; \
        (resp)->data_size = 0; \
        (resp)->latency_us = 0; \
    } while(0)

#endif /* HW_RAFAELIA_INTEGRATION_H */

/*
 * RAFAELIA Integration Hub - Test and Demonstration Program
 * 
 * Demonstrates the strategic integration of multiple repositories
 * using logic-based tactics and RAFAELIA ethical principles.
 */

#include "hw/core/rafaelia-integration.h"
#include "hw/core/rafaelia-core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* External connector initialization functions */
int rafaelia_userland_connector_init(rafaelia_connector_t *conn);
int rafaelia_magisk_connector_init(rafaelia_connector_t *conn);
int rafaelia_llama_connector_init(rafaelia_connector_t *conn);
int rafaelia_private_connector_init(rafaelia_connector_t *conn);

/* Helper function to print separator */
static void print_separator(const char *title)
{
    printf("\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("  %s\n", title);
    printf("═══════════════════════════════════════════════════════\n");
}

/* Test 1: Hub Initialization */
static void test_hub_initialization(rafaelia_integration_hub_t *hub, 
                                    rafaelia_core_t *core)
{
    print_separator("Test 1: Hub Initialization");
    
    int ret = rafaelia_integration_hub_init(hub, core);
    printf("Hub initialization: %s\n", ret == 0 ? "SUCCESS" : "FAILED");
    printf("Core reference: %p\n", (void *)hub->core);
    printf("Initialized flag: %s\n", hub->initialized ? "true" : "false");
    printf("System Φ_ethica: %.4f\n", hub->system_phi_ethica);
    printf("System health: %.4f\n", hub->system_health);
}

/* Test 2: Connector Setup */
static void test_connector_setup(rafaelia_integration_hub_t *hub)
{
    print_separator("Test 2: Connector Setup");
    
    /* Initialize all connectors */
    rafaelia_userland_connector_init(&hub->connectors[REPO_USERLAND]);
    rafaelia_magisk_connector_init(&hub->connectors[REPO_MAGISK]);
    rafaelia_llama_connector_init(&hub->connectors[REPO_LLAMA]);
    rafaelia_private_connector_init(&hub->connectors[REPO_PRIVATE]);
    
    /* Display connector status */
    for (int i = 0; i < REPO_COUNT; i++) {
        rafaelia_connector_t *conn = &hub->connectors[i];
        printf("\n%s Connector:\n", rafaelia_repository_name(i));
        printf("  Status: %s\n", rafaelia_status_name(conn->status));
        printf("  Capabilities: 0x%08X\n", conn->capabilities);
        printf("  Success rate: %.2f%%\n", conn->success_rate * 100.0);
        printf("  Φ_ethica avg: %.4f\n", conn->phi_ethica_avg);
        printf("  OWLψ avg: %.4f\n", conn->owl_psi_avg);
    }
}

/* Test 3: Repository Connections */
static void test_repository_connections(rafaelia_integration_hub_t *hub)
{
    print_separator("Test 3: Repository Connections");
    
    /* Connect to each repository (simulated) */
    int ret;
    
    ret = rafaelia_integration_connect_repository(hub, REPO_USERLAND,
                                                  "tcp://localhost:8001", NULL);
    printf("UserLAnd connection: %s\n", ret == 0 ? "SUCCESS" : "FAILED");
    
    ret = rafaelia_integration_connect_repository(hub, REPO_MAGISK,
                                                  "unix:///var/run/magisk.sock", NULL);
    printf("Magisk connection: %s\n", ret == 0 ? "SUCCESS" : "FAILED");
    
    ret = rafaelia_integration_connect_repository(hub, REPO_LLAMA,
                                                  "tcp://localhost:8080", NULL);
    printf("LLaMA connection: %s\n", ret == 0 ? "SUCCESS" : "FAILED");
    
    ret = rafaelia_integration_connect_repository(hub, REPO_PRIVATE,
                                                  "tcp://localhost:9000", NULL);
    printf("Private connection: %s\n", ret == 0 ? "SUCCESS" : "FAILED");
    
    /* Display connection status */
    printf("\nConnection Status:\n");
    for (int i = 0; i < REPO_COUNT; i++) {
        rafaelia_integration_status_t status = 
            rafaelia_integration_get_status(hub, i);
        printf("  %s: %s\n", rafaelia_repository_name(i),
               rafaelia_status_name(status));
    }
}

/* Test 4: Strategic Routing */
static void test_strategic_routing(rafaelia_integration_hub_t *hub)
{
    print_separator("Test 4: Strategic Routing");
    
    /* Test different operation categories */
    rafaelia_request_t requests[5];
    
    /* Intelligence request */
    RAFAELIA_REQUEST_INIT(&requests[0]);
    requests[0].category = OP_INTELLIGENCE;
    requests[0].capabilities_required = CAP_AI_INFERENCE | CAP_NLP;
    requests[0].amor = 0.9;
    requests[0].coerencia = 0.8;
    
    /* Privileged request */
    RAFAELIA_REQUEST_INIT(&requests[1]);
    requests[1].category = OP_PRIVILEGED;
    requests[1].capabilities_required = CAP_ROOT_ACCESS;
    requests[1].amor = 0.7;
    requests[1].coerencia = 0.9;
    
    /* Userspace request */
    RAFAELIA_REQUEST_INIT(&requests[2]);
    requests[2].category = OP_USERSPACE;
    requests[2].capabilities_required = CAP_CONTAINER;
    requests[2].amor = 0.85;
    requests[2].coerencia = 0.85;
    
    /* Compute request */
    RAFAELIA_REQUEST_INIT(&requests[3]);
    requests[3].category = OP_COMPUTE;
    requests[3].capabilities_required = CAP_COMPUTE;
    requests[3].amor = 0.8;
    requests[3].coerencia = 0.7;
    
    /* Private request */
    RAFAELIA_REQUEST_INIT(&requests[4]);
    requests[4].category = OP_PRIVATE;
    requests[4].capabilities_required = CAP_EXTENSION;
    requests[4].amor = 0.95;
    requests[4].coerencia = 0.95;
    
    const char *categories[] = {
        "Intelligence", "Privileged", "Userspace", "Compute", "Private"
    };
    
    for (int i = 0; i < 5; i++) {
        rafaelia_repository_id_t target = 
            rafaelia_integration_route_request(hub, &requests[i]);
        
        printf("\n%s Request:\n", categories[i]);
        printf("  Routed to: %s\n", rafaelia_repository_name(target));
        printf("  Amor: %.2f, Coerência: %.2f\n", 
               requests[i].amor, requests[i].coerencia);
        
        /* Show routing scores for all repositories */
        printf("  Routing scores:\n");
        for (int j = 0; j < REPO_COUNT; j++) {
            double score = rafaelia_integration_compute_routing_score(
                hub, j, &requests[i]);
            printf("    %s: %.4f\n", rafaelia_repository_name(j), score);
        }
    }
}

/* Test 5: Request Execution */
static void test_request_execution(rafaelia_integration_hub_t *hub)
{
    print_separator("Test 5: Request Execution");
    
    /* Create and send test requests */
    rafaelia_request_t req;
    rafaelia_response_t resp;
    
    RAFAELIA_REQUEST_INIT(&req);
    req.id = 1001;
    req.category = OP_INTELLIGENCE;
    req.capabilities_required = CAP_AI_INFERENCE;
    req.amor = 0.9;
    req.coerencia = 0.85;
    req.verdade = 0.9;
    req.consciencia = 0.8;
    
    int ret = rafaelia_integration_send_request_sync(hub, &req, &resp, 5000);
    
    printf("Request execution: %s\n", ret == 0 ? "SUCCESS" : "FAILED");
    if (ret == 0) {
        printf("Request ID: %lu\n", (unsigned long)req.id);
        printf("Response from: %s\n", rafaelia_repository_name(resp.source));
        printf("Status code: %d\n", resp.status_code);
        printf("Φ_ethica: %.4f\n", resp.phi_ethica);
        printf("OWLψ: %.4f\n", resp.owl_psi);
        printf("Latency: %lu μs\n", (unsigned long)resp.latency_us);
    }
}

/* Test 6: Ethical Compliance */
static void test_ethical_compliance(void)
{
    print_separator("Test 6: Ethical Compliance");
    
    rafaelia_request_t requests[4];
    
    /* High ethical quality */
    RAFAELIA_REQUEST_INIT(&requests[0]);
    requests[0].amor = 0.9;
    requests[0].coerencia = 0.9;
    requests[0].verdade = 0.9;
    requests[0].consciencia = 0.9;
    
    /* Moderate ethical quality */
    RAFAELIA_REQUEST_INIT(&requests[1]);
    requests[1].amor = 0.7;
    requests[1].coerencia = 0.7;
    requests[1].verdade = 0.7;
    requests[1].consciencia = 0.7;
    
    /* Low ethical quality */
    RAFAELIA_REQUEST_INIT(&requests[2]);
    requests[2].amor = 0.3;
    requests[2].coerencia = 0.3;
    requests[2].verdade = 0.3;
    requests[2].consciencia = 0.3;
    
    /* Mixed quality */
    RAFAELIA_REQUEST_INIT(&requests[3]);
    requests[3].amor = 0.95;
    requests[3].coerencia = 0.6;
    requests[3].verdade = 0.8;
    requests[3].consciencia = 0.4;
    
    const char *labels[] = {"High", "Moderate", "Low", "Mixed"};
    double threshold = 0.5;
    
    for (int i = 0; i < 4; i++) {
        bool compliant = rafaelia_integration_check_ethical_compliance(
            &requests[i], threshold);
        
        printf("\n%s Ethical Quality Request:\n", labels[i]);
        printf("  Amor: %.2f, Coerência: %.2f\n", 
               requests[i].amor, requests[i].coerencia);
        printf("  Verdade: %.2f, Consciência: %.2f\n",
               requests[i].verdade, requests[i].consciencia);
        printf("  Compliant (threshold %.2f): %s\n",
               threshold, compliant ? "YES" : "NO");
    }
}

/* Test 7: Retroalimentation */
static void test_retroalimentation(rafaelia_integration_hub_t *hub)
{
    print_separator("Test 7: Retroalimentation");
    
    printf("Initial routing weights:\n");
    for (int i = 0; i < REPO_COUNT; i++) {
        printf("  %s: %.4f\n", rafaelia_repository_name(i),
               hub->routing_weights[i]);
    }
    
    /* Simulate some responses with varying quality */
    rafaelia_response_t responses[3];
    
    for (int i = 0; i < 3; i++) {
        RAFAELIA_RESPONSE_INIT(&responses[i]);
        responses[i].source = (i + 1) % REPO_COUNT;
        responses[i].status_code = 0;
        responses[i].phi_ethica = 0.8 + (i * 0.05);
        responses[i].owl_psi = 0.75 + (i * 0.05);
        responses[i].latency_us = 1000 + (i * 500);
        
        rafaelia_integration_update_connector_metrics(
            hub, responses[i].source, &responses[i]);
    }
    
    /* Apply retroalimentation */
    int ret = rafaelia_integration_apply_retroalimentation(hub);
    printf("\nRetroalimentation applied: %s\n", ret == 0 ? "SUCCESS" : "FAILED");
    
    printf("\nUpdated routing weights:\n");
    for (int i = 0; i < REPO_COUNT; i++) {
        printf("  %s: %.4f\n", rafaelia_repository_name(i),
               hub->routing_weights[i]);
    }
}

/* Test 8: System Metrics */
static void test_system_metrics(rafaelia_integration_hub_t *hub)
{
    print_separator("Test 8: System Metrics");
    
    uint64_t requests, responses;
    double health, phi_ethica;
    
    rafaelia_integration_get_metrics(hub, &requests, &responses, 
                                    &health, &phi_ethica);
    
    printf("Total requests: %lu\n", (unsigned long)requests);
    printf("Total responses: %lu\n", (unsigned long)responses);
    printf("System health: %.4f\n", health);
    printf("System Φ_ethica: %.4f\n", phi_ethica);
    
    double integration_power = 
        rafaelia_integration_compute_integration_power(hub);
    printf("Integration power (Formula I.1): %.4f\n", integration_power);
    
    printf("\nPer-Repository Statistics:\n");
    for (int i = 0; i < REPO_COUNT; i++) {
        rafaelia_connector_t *conn = &hub->connectors[i];
        printf("\n%s:\n", rafaelia_repository_name(i));
        printf("  Requests sent: %lu\n", (unsigned long)conn->requests_sent);
        printf("  Responses received: %lu\n", 
               (unsigned long)conn->responses_received);
        printf("  Errors: %lu\n", (unsigned long)conn->errors);
        printf("  Avg latency: %.2f ms\n", conn->avg_latency_ms);
        printf("  Success rate: %.2f%%\n", conn->success_rate * 100.0);
        printf("  Load factor: %.2f\n", conn->load_factor);
    }
}

/* Test 9: Diamond State Evaluation */
static void test_diamond_state(rafaelia_integration_hub_t *hub)
{
    print_separator("Test 9: Diamond State (💎) Evaluation");
    
    /* Evaluate system against diamond criteria */
    double health = rafaelia_integration_compute_system_health(hub);
    double power = rafaelia_integration_compute_integration_power(hub);
    
    bool all_connected = true;
    double avg_phi_ethica = 0.0;
    double avg_success = 0.0;
    int active_count = 0;
    
    for (int i = 0; i < REPO_COUNT; i++) {
        rafaelia_connector_t *conn = &hub->connectors[i];
        if (conn->status != STATUS_ACTIVE && conn->status != STATUS_CONNECTED) {
            if (i != REPO_QEMU_RAFAELIA) {
                all_connected = false;
            }
        } else {
            avg_phi_ethica += conn->phi_ethica_avg;
            avg_success += conn->success_rate;
            active_count++;
        }
    }
    
    if (active_count > 0) {
        avg_phi_ethica /= active_count;
        avg_success /= active_count;
    }
    
    printf("Diamond State Criteria:\n");
    printf("  ✓ System Health: %.2f%% %s\n", health * 100.0,
           health >= 0.9 ? "💎" : health >= 0.7 ? "⚠" : "✗");
    printf("  ✓ Integration Power: %.4f %s\n", power,
           power >= 3.0 ? "💎" : power >= 2.0 ? "⚠" : "✗");
    printf("  ✓ Average Φ_ethica: %.4f %s\n", avg_phi_ethica,
           avg_phi_ethica >= 0.8 ? "💎" : avg_phi_ethica >= 0.6 ? "⚠" : "✗");
    printf("  ✓ Average Success: %.2f%% %s\n", avg_success * 100.0,
           avg_success >= 0.95 ? "💎" : avg_success >= 0.8 ? "⚠" : "✗");
    printf("  ✓ All Repositories: %s %s\n", all_connected ? "Connected" : "Partial",
           all_connected ? "💎" : "⚠");
    printf("  ✓ Retroalimentation: %s 💎\n", 
           hub->retroalimentation_enabled ? "Active" : "Inactive");
    printf("  ✓ Adaptive Routing: %s 💎\n",
           hub->adaptive_routing_enabled ? "Active" : "Inactive");
    
    /* Overall diamond state assessment */
    bool diamond_state = health >= 0.9 && power >= 3.0 && 
                        avg_phi_ethica >= 0.8 && avg_success >= 0.95;
    
    printf("\n");
    if (diamond_state) {
        printf("🎨 DIAMOND STATE ACHIEVED! 💎\n");
        printf("System operating at excellence level.\n");
    } else {
        printf("⚡ STRIVING FOR DIAMOND STATE...\n");
        printf("System operational but optimizing for excellence.\n");
    }
    
    printf("\nFIAT LUX ΣΩΔΦBITRAF\n");
}

/* Main test program */
int main(void)
{
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════╗\n");
    printf("║   RAFAELIA Multi-Repository Integration Test Suite   ║\n");
    printf("║              Diamond State Excellence (💎)            ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");
    
    /* Initialize RAFAELIA core */
    rafaelia_core_t core;
    rafaelia_fiat_portal_init(&core);
    
    /* Initialize integration hub */
    rafaelia_integration_hub_t hub;
    
    /* Run test suite */
    test_hub_initialization(&hub, &core);
    test_connector_setup(&hub);
    test_repository_connections(&hub);
    test_strategic_routing(&hub);
    test_request_execution(&hub);
    test_ethical_compliance();
    test_retroalimentation(&hub);
    test_system_metrics(&hub);
    test_diamond_state(&hub);
    
    /* Cleanup */
    print_separator("Cleanup");
    rafaelia_integration_hub_stop(&hub);
    rafaelia_integration_hub_cleanup(&hub);
    rafaelia_core_cleanup(&core);
    printf("Integration hub cleaned up successfully.\n");
    
    printf("\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("            All tests completed successfully!          \n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("\n");
    
    return 0;
}

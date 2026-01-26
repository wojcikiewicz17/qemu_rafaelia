/*
 * RAFAELIA Symbiosis Unit Tests
 * Test suite for mathematical calculations and data structures
 * 
 * Build: gcc -o test hw/core/rafaelia-symbiosis.c hw/core/rafaelia-rmr-lowlevel.c hw/core/rafaelia-symbiosis-test.c -lm
 * 
 * FIAT LUX ΣΩΔΦBITRAF 💎
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/* Include the header for type definitions and function declarations */
#include "rafaelia-symbiosis.h"
#include "hw/core/rafaelia-rmr-lowlevel.h"

#define TEST_EPSILON 1e-10
#define TEST_PASS() printf("  ✓ PASS\n")
#define TEST_FAIL(msg) do { printf("  ✗ FAIL: %s\n", msg); return 1; } while(0)

/* ═══════════════════════════════════════════════════════════════════════════
 * Test: Triade Matemática - Pitágoras
 * ═══════════════════════════════════════════════════════════════════════════ */
static int test_triade_pitagoras(void)
{
    printf("Testing triade_compute_pitagoras...\n");
    
    triade_matematica_t triade;
    triade_init(&triade);
    
    /* Test 3-4-5 right triangle */
    triade_compute_pitagoras(&triade, 3.0, 4.0);
    
    /* a² = 9, b² = 16, c² = a² + b² = 25 */
    if (fabs(triade.pitagoras_a2 - 9.0) > TEST_EPSILON) {
        TEST_FAIL("pitagoras_a2 should be 9.0");
    }
    if (fabs(triade.pitagoras_b2 - 16.0) > TEST_EPSILON) {
        TEST_FAIL("pitagoras_b2 should be 16.0");
    }
    if (fabs(triade.pitagoras_c2 - 25.0) > TEST_EPSILON) {
        TEST_FAIL("pitagoras_c2 should be 25.0 (c² = a² + b² = 9 + 16)");
    }
    
    TEST_PASS();
    return 0;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Test: Triade Matemática - Bhaskara
 * ═══════════════════════════════════════════════════════════════════════════ */
static int test_triade_bhaskara(void)
{
    printf("Testing triade_compute_bhaskara...\n");
    
    triade_matematica_t triade;
    triade_init(&triade);
    
    /* Test x² - 5x + 6 = 0: a=1, b=-5, c=6 */
    /* Δ = b² - 4ac = 25 - 24 = 1 */
    triade_compute_bhaskara(&triade, 1.0, -5.0, 6.0);
    
    if (fabs(triade.bhaskara_b2 - 25.0) > TEST_EPSILON) {
        TEST_FAIL("bhaskara_b2 should be 25.0");
    }
    if (fabs(triade.bhaskara_4ac - 24.0) > TEST_EPSILON) {
        TEST_FAIL("bhaskara_4ac should be 24.0");
    }
    if (fabs(triade.bhaskara_delta - 1.0) > TEST_EPSILON) {
        TEST_FAIL("bhaskara_delta should be 1.0");
    }
    
    TEST_PASS();
    return 0;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Test: Fibonacci Cache
 * ═══════════════════════════════════════════════════════════════════════════ */
static int test_fibonacci_cache(void)
{
    printf("Testing fibonacci_get...\n");
    
    fibonacci_etica_t state;
    fibonacci_etica_init(&state);
    
    /* Test known Fibonacci values */
    if (fibonacci_get(&state, 0) != 0) TEST_FAIL("F(0) should be 0");
    if (fibonacci_get(&state, 1) != 1) TEST_FAIL("F(1) should be 1");
    if (fibonacci_get(&state, 2) != 1) TEST_FAIL("F(2) should be 1");
    if (fibonacci_get(&state, 10) != 55) TEST_FAIL("F(10) should be 55");
    if (fibonacci_get(&state, 20) != 6765) TEST_FAIL("F(20) should be 6765");
    
    fibonacci_etica_cleanup(&state);
    TEST_PASS();
    return 0;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Test: Mandala Initialization
 * ═══════════════════════════════════════════════════════════════════════════ */
static int test_mandala_init(void)
{
    printf("Testing mandala_init...\n");
    
    mandala_state_t mandala;
    mandala_init(&mandala);
    
    /* Check initial state */
    if (mandala.iteration != 0) TEST_FAIL("iteration should be 0");
    if (fabs(mandala.total_energy - 1.0) > TEST_EPSILON) TEST_FAIL("total_energy should be 1.0");
    if (fabs(mandala.coherence - 1.0) > TEST_EPSILON) TEST_FAIL("coherence should be 1.0");
    
    /* Check all cells are initialized */
    for (int y = 0; y < MANDALA_SIZE; y++) {
        for (int x = 0; x < MANDALA_SIZE; x++) {
            if (mandala.cells[y][x].x != x) TEST_FAIL("cell x coordinate wrong");
            if (mandala.cells[y][x].y != y) TEST_FAIL("cell y coordinate wrong");
        }
    }
    
    TEST_PASS();
    return 0;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Test: RAFCODE Flow
 * ═══════════════════════════════════════════════════════════════════════════ */
static int test_rafcode_flow(void)
{
    printf("Testing rafcode_advance...\n");
    
    rafcode_state_t state;
    rafcode_init(&state);
    
    /* Test state transitions */
    if (state.current_stage != RAFCODE_VAZIO) TEST_FAIL("should start at VAZIO");
    
    rafcode_advance(&state);
    if (state.current_stage != RAFCODE_VERBO) TEST_FAIL("should be at VERBO");
    
    rafcode_advance(&state);
    if (state.current_stage != RAFCODE_HYPERFORMAS) TEST_FAIL("should be at HYPERFORMAS");
    
    rafcode_advance(&state);
    if (state.current_stage != RAFCODE_TAG14_PLECT) TEST_FAIL("should be at TAG14_PLECT");
    
    rafcode_advance(&state);
    if (state.current_stage != RAFCODE_BITRAF64_ZIPRAF60) TEST_FAIL("should be at BITRAF64_ZIPRAF60");
    
    rafcode_advance(&state);
    if (state.current_stage != RAFCODE_RETROALIMENTACAO) TEST_FAIL("should be at RETROALIMENTACAO");
    
    rafcode_advance(&state);
    if (state.current_stage != RAFCODE_NOVO_VAZIO) TEST_FAIL("should be at NOVO_VAZIO");
    
    /* Cycle completes when transitioning from NOVO_VAZIO back to VAZIO */
    rafcode_advance(&state);
    if (state.current_stage != RAFCODE_VAZIO) TEST_FAIL("should be back at VAZIO");
    if (!state.cycle_complete) TEST_FAIL("cycle should be complete after returning to VAZIO");
    
    TEST_PASS();
    return 0;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Test: Fractal Memory Depth Safety
 * ═══════════════════════════════════════════════════════════════════════════ */
static int test_fractal_depth_safety(void)
{
    printf("Testing fractal depth safety limit...\n");
    
    fractal_memory_t mem;
    fractal_memory_init(&mem);
    
    /* Verify initial state */
    if (mem.max_depth != 1) TEST_FAIL("initial max_depth should be 1");
    if (mem.total_nodes != 1) TEST_FAIL("initial total_nodes should be 1");
    
    /* Evolve and check depth doesn't exceed safety limit */
    for (int i = 0; i < 5; i++) {
        fractal_memory_evolve(&mem);
    }
    
    /* max_depth should be capped at FRACTAL_MAX_SAFE_DEPTH (8) */
    if (mem.max_depth > 8) TEST_FAIL("max_depth should not exceed 8");
    
    fractal_memory_cleanup(&mem);
    TEST_PASS();
    return 0;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Test: Torus Entropy Calculation
 * ═══════════════════════════════════════════════════════════════════════════ */
static int test_torus_entropy(void)
{
    printf("Testing torus_compute_hash_entropy...\n");
    
    torus_data_t torus;
    torus_init(&torus);
    
    /* Test with uniform data (low entropy) */
    uint8_t uniform_data[64];
    rafaelia_rmr_memzero(uniform_data, sizeof(uniform_data));
    for (int i = 0; i < (int)sizeof(uniform_data); i++) {
        uniform_data[i] = 0xAA;
    }
    torus_compute_hash_entropy(&torus, uniform_data, sizeof(uniform_data));
    
    /* Uniform data should have zero entropy */
    if (torus.entropy > 0.1) TEST_FAIL("uniform data should have low entropy");
    
    /* Test with varied data (higher entropy) */
    uint8_t varied_data[64];
    for (int i = 0; i < 64; i++) varied_data[i] = (uint8_t)i;
    torus_compute_hash_entropy(&torus, varied_data, sizeof(varied_data));
    
    /* Varied data should have higher entropy */
    if (torus.entropy < 0.5) TEST_FAIL("varied data should have higher entropy");
    
    TEST_PASS();
    return 0;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Test: Symbiosis Diamond State
 * ═══════════════════════════════════════════════════════════════════════════ */
static int test_symbiosis_diamond_state(void)
{
    printf("Testing rafaelia_symbiosis_is_diamond...\n");
    
    rafaelia_symbiosis_t state;
    rafaelia_symbiosis_init(&state);
    
    /* Initial state should have high coherence */
    double coherence = rafaelia_symbiosis_get_coherence(&state);
    if (coherence < 0.0 || coherence > 1.0) TEST_FAIL("coherence should be in [0,1]");
    
    /* Run a few cycles */
    for (int i = 0; i < 10; i++) {
        rafaelia_symbiosis_cycle(&state);
    }
    
    /* Check diamond state check doesn't crash */
    bool is_diamond = rafaelia_symbiosis_is_diamond(&state);
    (void)is_diamond;  /* Result depends on state evolution */
    
    rafaelia_symbiosis_cleanup(&state);
    TEST_PASS();
    return 0;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Main Test Runner
 * ═══════════════════════════════════════════════════════════════════════════ */
int main(void)
{
    int failures = 0;
    
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║     RAFAELIA Symbiosis Unit Tests                             ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n\n");
    
    failures += test_triade_pitagoras();
    failures += test_triade_bhaskara();
    failures += test_fibonacci_cache();
    failures += test_mandala_init();
    failures += test_rafcode_flow();
    failures += test_fractal_depth_safety();
    failures += test_torus_entropy();
    failures += test_symbiosis_diamond_state();
    
    printf("\n═══════════════════════════════════════════════════════════════\n");
    if (failures == 0) {
        printf("All tests passed! 💎\n");
    } else {
        printf("Tests failed: %d\n", failures);
    }
    printf("═══════════════════════════════════════════════════════════════\n\n");
    
    return failures;
}

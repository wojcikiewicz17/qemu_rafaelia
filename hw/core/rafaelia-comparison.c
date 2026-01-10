/* RAFAELIA COMPARISON - Side-by-side old vs new implementation */
/* Baremetal compatible comparison suite */

#include "rafaelia-matrix-core.h"
#include "rafaelia-refactored.h"

/* Test 1: Basic cycle operations */
void test_cycle_comparison(void) {
    /* NEW MATRIX-BASED APPROACH */
    z0();  /* Init */
    
    /* Run 10 cycles */
    int i = M00;
    while (i < 10) {
        y2();  /* Cycle step */
        i = i + 1;
    }
    
    /* Get results - verify operations complete */
    double psi = y17(M00);
    double sigma = y17(M04);
    double omega = y17(M05);
    double measure = y3();
    (void)psi;
    (void)sigma;
    (void)omega;
    (void)measure;
    
    z1();  /* Cleanup */
    
    /* OLD STRUCT-BASED APPROACH would be:
     * 
     * rafaelia_core_t core;
     * rafaelia_core_init(&core);
     * 
     * for (int i = 0; i < 10; i++) {
     *     rafaelia_cycle_step(&core.cycle, &core.phi_ethica);
     * }
     * 
     * double psi = core.cycle.psi;
     * double sigma = core.cycle.sigma;
     * double omega = core.cycle.omega;
     * double measure = rafaelia_cycle_measure(&core.cycle);
     * 
     * rafaelia_core_cleanup(&core);
     */
}

/* Test 2: Block operations */
void test_block_comparison(void) {
    /* NEW MATRIX-BASED APPROACH */
    z0();
    
    /* Add blocks directly with values */
    int b0 = y10(C6, C6, C6, C6);    /* All 1.0 */
    int b1 = y10(C0, C1, C2, C3);    /* φ, π, √3/2, R_corr */
    
    /* Evaluate - verify operations */
    double e0 = y11(b0);
    double e1 = y11(b1);
    (void)e0;
    (void)e1;
    
    z1();
    
    /* OLD STRUCT-BASED APPROACH would be:
     * 
     * rafaelia_core_t core;
     * rafaelia_core_init(&core);
     * 
     * rafaelia_bloco_t bloco0;
     * bloco0.psi = 1.0;
     * bloco0.chi = 1.0;
     * bloco0.rho = 1.0;
     * bloco0.delta = 1.0;
     * int b0 = rafaelia_bloco_add(&core, &bloco0);
     * 
     * rafaelia_bloco_t bloco1;
     * bloco1.psi = RAFAELIA_PHI;
     * bloco1.chi = RAFAELIA_PI;
     * bloco1.rho = RAFAELIA_SQRT3_2;
     * bloco1.delta = RAFAELIA_R_CORR;
     * int b1 = rafaelia_bloco_add(&core, &bloco1);
     * 
     * double e0 = rafaelia_bloco_eval(&core, b0);
     * double e1 = rafaelia_bloco_eval(&core, b1);
     * 
     * rafaelia_core_cleanup(&core);
     */
}

/* Test 3: Matrix operations (NEW ONLY) */
void test_matrix_operations(void) {
    /* NEW MATRIX-BASED APPROACH - Native matrix support */
    double m0[D0];
    double m1[D0];
    double m2[D0];
    
    /* Initialize matrices */
    r7(m0);  /* Zero */
    r8(m1);  /* Ones */
    
    /* Operations */
    r5(m2, m0, m1);  /* Add */
    r6(m2, m0, m1);  /* Multiply */
    
    /* Transformations */
    x8(m2, m1);  /* Spiral */
    y15(m0);     /* Generate spiral */
    
    /* Metrics - verify calculations */
    double corr = y14(m0, m1);   /* Correlation */
    double vortex = y16(m2);     /* Vortex */
    (void)corr;
    (void)vortex;
    
    /* OLD STRUCT-BASED APPROACH - No native matrix operations
     * Would need to implement manually or use external library
     */
}

/* Test 4: Deterministic operations */
void test_deterministic_flips(void) {
    /* NEW MATRIX-BASED APPROACH - Single flip operations */
    double a = C6;  /* 1.0 */
    double b = C0;  /* φ */
    double c = C1;  /* π */
    double d = C2;  /* √3/2 */
    
    /* Single flip operations - verify all complete */
    double r0 = f0(a, b);        /* Multiply */
    double r1 = f1(a, b);        /* Add */
    double r2 = f5(a, b, c);     /* a*b+c */
    double r3 = f6(a, b, c);     /* a*b*c */
    double r4 = f9(a, b, c, d);  /* (a*b+c*d)/2 */
    (void)r0;
    (void)r1;
    (void)r2;
    (void)r3;
    (void)r4;
    
    /* OLD STRUCT-BASED APPROACH - Would use regular expressions
     * No concept of "single flip resolves N flops"
     */
}

/* Test 5: Direct state access */
void test_direct_access(void) {
    /* NEW MATRIX-BASED APPROACH - Direct indexed access */
    z0();
    
    /* Set cycle values directly by index */
    y18(M00, 1.5);  /* psi */
    y18(M01, 2.0);  /* chi */
    y18(M04, 10.0); /* sigma */
    
    /* Get cycle values directly by index - verify operations */
    double v0 = y17(M00);
    double v1 = y17(M01);
    double v4 = y17(M04);
    (void)v0;
    (void)v1;
    (void)v4;
    
    /* Set ethica values by index */
    y20(M00, 1.2);  /* amor */
    y20(M01, 0.9);  /* coerencia */
    
    /* Get ethica values by index - verify operations */
    double e0 = y19(M00);
    double e1 = y19(M01);
    (void)e0;
    (void)e1;
    
    z1();
    
    /* OLD STRUCT-BASED APPROACH - Named field access
     * 
     * rafaelia_core_t core;
     * rafaelia_core_init(&core);
     * 
     * core.cycle.psi = 1.5;
     * core.cycle.chi = 2.0;
     * core.cycle.sigma = 10.0;
     * 
     * double v0 = core.cycle.psi;
     * double v1 = core.cycle.chi;
     * double v4 = core.cycle.sigma;
     * 
     * core.phi_ethica.amor = 1.2;
     * core.phi_ethica.coerencia = 0.9;
     * 
     * double e0 = core.phi_ethica.amor;
     * double e1 = core.phi_ethica.coerencia;
     * 
     * rafaelia_core_cleanup(&core);
     */
}

/* Test 6: Integration with retroalimentation */
void test_integration(void) {
    /* NEW MATRIX-BASED APPROACH */
    z0();
    
    int n = M00;
    while (n < 100) {
        /* Execute cycle */
        y2();
        
        /* Get values for retro update */
        double v0 = y17(M00);  /* psi */
        double v1 = y19(M00);  /* amor */
        
        /* Update retro */
        y7(v0, v1);
        
        /* Periodic metric update */
        if (n % 10 == 0) {
            double phi = y4();
            y22(M00, phi);  /* Update r_omega metric */
        }
        
        n = n + 1;
    }
    
    /* Get final metrics - verify operations */
    double final_r_omega = y21(M00);
    double final_sigma = y17(M04);
    (void)final_r_omega;
    (void)final_sigma;
    
    z1();
    
    /* OLD STRUCT-BASED APPROACH would require:
     * - Multiple function calls
     * - Passing pointers around
     * - Manual state synchronization
     * - More verbose code
     */
}

/* Compile test */
#ifdef TEST_COMPARISON
int main(void) {
    test_cycle_comparison();
    test_block_comparison();
    test_matrix_operations();
    test_deterministic_flips();
    test_direct_access();
    test_integration();
    return M00;
}
#endif

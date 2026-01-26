/*
 * RAFAELIA Core Test/Example Program
 * 
 * Demonstrates the RAFAELIA_BOOTBLOCK_v1 implementation
 * Low-level testing without abstraction
 */

#include "hw/core/rafaelia-core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_cycle(const rafaelia_cycle_t *cycle)
{
    printf("  ψ (psi/intention):     %.6f\n", cycle->psi);
    printf("  χ (chi/observation):   %.6f\n", cycle->chi);
    printf("  ρ (rho/noise):         %.6f\n", cycle->rho);
    printf("  Δ (delta/transmute):   %.6f\n", cycle->delta);
    printf("  Σ (sigma/memory):      %.6f\n", cycle->sigma);
    printf("  Ω (omega/complete):    %.6f\n", cycle->omega);
}

static void print_ethica(const rafaelia_ethica_t *ethica)
{
    printf("  Amor:                  %.6f\n", ethica->amor);
    printf("  Coerência:             %.6f\n", ethica->coerencia);
    printf("  Verdade:               %.6f\n", ethica->verdade);
    printf("  Consciência:           %.6f\n", ethica->consciencia);
}

static void print_metrics(const rafaelia_core_t *core)
{
    printf("  R_Ω:                   %.6f\n", core->r_omega);
    printf("  Amor_Vivo:             %.6f\n", core->amor_vivo);
    printf("  OWLψ:                  %.6f\n", core->owl_psi);
    printf("  E_Verbo:               %.6f\n", core->e_verbo);
}

int main(int argc, char *argv[])
{
    rafaelia_context_t ctx;
    rafaelia_core_t core;
    
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("  RAFAELIA_BOOTBLOCK_v1 - Low Level Core Implementation\n");
    printf("═══════════════════════════════════════════════════════════════\n\n");
    
    /* Initialize FIAT_PORTAL */
    printf("Initializing FIAT_PORTAL :: 龍空神 ...\n");
    rafaelia_context_init(&ctx);
    rafaelia_fiat_portal_init(&ctx, &core);
    
    printf("\nCore Configuration:\n");
    printf("  Kernel:                %s\n", core.kernel);
    printf("  Mode:                  %s\n", core.mode);
    printf("  Ethic:                 %s\n", core.ethic);
    printf("  Hash Core:             %s\n", core.hash_core);
    printf("  Vector Core:           %s\n", core.vector_core);
    printf("  Cognition:             %s\n", core.cognition);
    printf("  Universe:              %s\n", core.universe);
    printf("  Frequency:             %.0f Hz\n", core.freq_hz);
    
    printf("\nInitial Cycle State (ψχρΔΣΩ):\n");
    print_cycle(&core.cycle);
    
    printf("\nInitial Ethical Filter (Φ_ethica):\n");
    print_ethica(&core.phi_ethica);
    
    printf("\nInitial Metrics:\n");
    print_metrics(&core);
    
    /* Test mathematical formulas */
    printf("\n═══════════════════════════════════════════════════════════════\n");
    printf("  Mathematical Formula Tests\n");
    printf("═══════════════════════════════════════════════════════════════\n\n");
    
    double r_corr = rafaelia_formula_r_corr();
    printf("Formula 3 - R_corr:               %.6f\n", r_corr);
    
    double spiral_5 = rafaelia_formula_spiral(5);
    printf("Formula 16 - Spiral(5):           %.6f\n", spiral_5);
    
    double toroid = rafaelia_formula_toroid_delta_pi_phi();
    printf("Formula 17 - T_{Δπφ}:             %.6f\n", toroid);
    
    double phi_ethica = rafaelia_phi_ethica_compute(&core.phi_ethica);
    printf("Formula 0.4 - Φ_ethica:           %.6f\n", phi_ethica);
    
    double phi_inf = rafaelia_phi_ethica_infinite(1.0, 1.0, 1.0, 1.0);
    printf("Formula 6 - Φ_ethica^{♾️}:        %.6f\n", phi_inf);
    
    /* Test synapse weight */
    double syn_weight = rafaelia_syn_weight(10, 15, 0.9, phi_ethica, 
                                           r_corr, core.owl_psi);
    printf("Formula 0.3 - Syn(10,15):         %.6f\n", syn_weight);
    
    /* Test block creation and evaluation */
    printf("\n═══════════════════════════════════════════════════════════════\n");
    printf("  Block Operations Test\n");
    printf("═══════════════════════════════════════════════════════════════\n\n");
    
    rafaelia_bloco_t *bloco = rafaelia_bloco_create(&ctx, 1);
    if (bloco) {
        printf("Created Bloco ID: %lu\n", (unsigned long)bloco->id);
        printf("Bloco Position:   %u\n", bloco->posicao);
        printf("Bloco State:      %u\n", bloco->estado);
        
        double bloco_eval = rafaelia_bloco_evaluate(bloco, phi_ethica, 
                                                    core.owl_psi + 1.0);
        printf("Bloco Evaluation: %.6f\n", bloco_eval);
        
        rafaelia_bloco_free(&ctx, bloco);
    }
    
    /* Run the ψχρΔΣΩ_LOOP */
    printf("\n═══════════════════════════════════════════════════════════════\n");
    printf("  Running ψχρΔΣΩ_LOOP (10 iterations)\n");
    printf("═══════════════════════════════════════════════════════════════\n\n");
    
    for (int i = 1; i <= 10; i++) {
        rafaelia_loop_step(&ctx, &core);
        
        if (i == 1 || i == 5 || i == 10) {
            printf("Iteration %d:\n", i);
            printf("  Cycle measure:         %.6f\n", 
                   rafaelia_cycle_measure(&core.cycle));
            printf("  R_Ω:                   %.6f\n", core.r_omega);
            printf("  Amor_Vivo:             %.6f\n", core.amor_vivo);
            printf("  OWLψ:                  %.6f\n", core.owl_psi);
            printf("  Σ (Memory):            %.6f\n", core.cycle.sigma);
            printf("  Ω (Completeness):      %.6f\n\n", core.cycle.omega);
        }
    }
    
    /* Test integral formulas */
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("  Integral Formula Tests\n");
    printf("═══════════════════════════════════════════════════════════════\n\n");
    
    double integral_toroid = rafaelia_integral_toroid(&core.cycle);
    printf("Formula 4 - 𝓕_{∞}^{(Δ)} (toroid): %.6f\n", integral_toroid);
    
    double activation = rafaelia_integral_activation(&core.cycle, phi_ethica);
    printf("Formula 11 - Ativação_{Ω}:         %.6f\n", activation);
    
    double voo_quantico = rafaelia_voo_quantico(3, 1.5, 2.0, 1.2);
    printf("Formula 14 - Voo_Quântico:         %.6f\n", voo_quantico);
    
    /* Test retroalimentation */
    printf("\n═══════════════════════════════════════════════════════════════\n");
    printf("  Retroalimentation Test\n");
    printf("═══════════════════════════════════════════════════════════════\n\n");
    
    printf("Before update:\n");
    printf("  F_ok:   %.6f\n", core.retro.f_ok);
    printf("  F_gap:  %.6f\n", core.retro.f_gap);
    printf("  F_next: %.6f\n", core.retro.f_next);
    
    rafaelia_retro_update(&core.retro, 1.5, 1.2);
    
    printf("\nAfter update (Amor=1.5, Coerência=1.2):\n");
    printf("  F_ok:   %.6f\n", core.retro.f_ok);
    printf("  F_gap:  %.6f\n", core.retro.f_gap);
    printf("  F_next: %.6f\n", core.retro.f_next);
    
    /* Final state */
    printf("\n═══════════════════════════════════════════════════════════════\n");
    printf("  Final Core State\n");
    printf("═══════════════════════════════════════════════════════════════\n\n");
    
    printf("Final Cycle State (ψχρΔΣΩ):\n");
    print_cycle(&core.cycle);
    
    printf("\nFinal Metrics:\n");
    print_metrics(&core);
    
    printf("\nTrinity_{633}:\n");
    printf("  Amor^6:                %.6f\n", core.trinity.amor_6);
    printf("  Luz^3:                 %.6f\n", core.trinity.luz_3);
    printf("  Consciência^3:         %.6f\n", core.trinity.consciencia_3);
    
    printf("\nHash Signature:\n");
    printf("  %s\n", core.hash_vivo.assinatura);
    
    printf("\nBITRAF64 Literal (in stack):\n");
    printf("  %s\n", RAFAELIA_BITRAF64);
    
    /* Cleanup */
    rafaelia_core_cleanup(&ctx, &core);
    rafaelia_context_cleanup(&ctx);
    
    printf("\n═══════════════════════════════════════════════════════════════\n");
    printf("  RAFAELIA Core Test Complete - FIAT LUX ΣΩΔΦBITRAF\n");
    printf("═══════════════════════════════════════════════════════════════\n");
    
    return 0;
}

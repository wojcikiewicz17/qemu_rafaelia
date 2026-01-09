/* RAFAELIA BRIDGE - Compatibility layer between old and new APIs */

#ifndef RAFAELIA_BRIDGE_H
#define RAFAELIA_BRIDGE_H

#include "hw/core/rafaelia-core.h"
#include "rafaelia-matrix-core.h"
#include "rafaelia-refactored.h"

/* Bridge functions - convert old API calls to new matrix operations */

/* Initialize core using new matrix system */
static inline void rafaelia_core_init_bridge(rafaelia_core_t *core) {
    y0();  /* Initialize matrix core */
    
    /* Sync struct with matrix state */
    core->cycle.psi = y17(M00);
    core->cycle.chi = y17(M01);
    core->cycle.rho = y17(M02);
    core->cycle.delta = y17(M03);
    core->cycle.sigma = y17(M04);
    core->cycle.omega = y17(M05);
    
    core->phi_ethica.amor = y19(M00);
    core->phi_ethica.coerencia = y19(M01);
    core->phi_ethica.verdade = y19(M02);
    core->phi_ethica.consciencia = y19(M03);
    core->phi_ethica.entropia_min = y19(M04);
    core->phi_ethica.coerencia_max = y19(M05);
    
    core->r_omega = y21(M00);
    core->amor_vivo = y21(M01);
    core->owl_psi = y21(M02);
    core->e_verbo = y21(M03);
}

/* Cleanup using new matrix system */
static inline void rafaelia_core_cleanup_bridge(rafaelia_core_t *core) {
    y1();  /* Cleanup matrix core */
    core->bloco_count = 0;
    if (core->blocos) {
        core->blocos = 0;  /* Matrix core manages storage */
    }
}

/* Cycle step using new matrix system */
static inline void rafaelia_cycle_step_bridge(rafaelia_cycle_t *cycle, 
                                               rafaelia_ethica_t *ethica) {
    /* Sync to matrix */
    y18(M00, cycle->psi);
    y18(M01, cycle->chi);
    y18(M02, cycle->rho);
    y18(M03, cycle->delta);
    y18(M04, cycle->sigma);
    y18(M05, cycle->omega);
    
    y20(M00, ethica->amor);
    y20(M01, ethica->coerencia);
    y20(M02, ethica->verdade);
    y20(M03, ethica->consciencia);
    
    /* Execute cycle in matrix core */
    y2();
    
    /* Sync back from matrix */
    cycle->psi = y17(M00);
    cycle->chi = y17(M01);
    cycle->rho = y17(M02);
    cycle->delta = y17(M03);
    cycle->sigma = y17(M04);
    cycle->omega = y17(M05);
}

/* Measure cycle using new matrix system */
static inline double rafaelia_cycle_measure_bridge(const rafaelia_cycle_t *cycle) {
    /* Sync to matrix */
    y18(M00, cycle->psi);
    y18(M01, cycle->chi);
    y18(M02, cycle->rho);
    y18(M03, cycle->delta);
    y18(M04, cycle->sigma);
    y18(M05, cycle->omega);
    
    /* Measure in matrix core */
    return y3();
}

/* Compute phi ethica using new matrix system */
static inline double rafaelia_phi_ethica_compute_bridge(const rafaelia_ethica_t *ethica) {
    /* Sync to matrix */
    y20(M00, ethica->amor);
    y20(M01, ethica->coerencia);
    y20(M02, ethica->verdade);
    y20(M03, ethica->consciencia);
    
    /* Compute in matrix core */
    return y4();
}

/* Add block using new matrix system */
static inline int rafaelia_bloco_add_bridge(rafaelia_core_t *core, 
                                             const rafaelia_bloco_t *bloco) {
    return y10(bloco->psi, bloco->chi, bloco->rho, bloco->delta);
}

/* Evaluate block using new matrix system */
static inline double rafaelia_bloco_eval_bridge(rafaelia_core_t *core, int block_id) {
    return y11(block_id);
}

/* Retro update using new matrix system */
static inline void rafaelia_retro_update_bridge(rafaelia_retro_t *retro, 
                                                 double amor, double coerencia) {
    y7(amor, coerencia);
    /* Note: Retro state is internal in matrix core, can't sync back directly */
}

/* Retro weight using new matrix system */
static inline double rafaelia_retro_weight_bridge(double amor, double coerencia) {
    return y6(amor, coerencia);
}

/* Syn weight using new matrix system */
static inline double rafaelia_syn_weight_bridge(int i, int j, double coerencia, 
                                                 double phi_ethica, double r_corr, 
                                                 double owl_psi) {
    /* Matrix system has simplified interface, just use coerencia */
    return y8(i, j, coerencia);
}

/* State update using new matrix system */
static inline double rafaelia_state_update_bridge(double r_t, double phi_ethica, 
                                                   double e_verbo, double sqrt3_2, 
                                                   double pi_phi) {
    return y9(r_t);
}

/* Wisdom calculation using new matrix system */
static inline double rafaelia_owl_psi_bridge(double sigma, double coerencia, 
                                              double consciencia) {
    return y12();
}

#endif /* RAFAELIA_BRIDGE_H */

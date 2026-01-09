# Migration Guide: From Struct-Based to Matrix-Based RAFAELIA

## Overview

This guide explains how to migrate from the old struct-based RAFAELIA implementation to the new ultra-low-level matrix-based approach.

## Key Differences

### Old Approach (Struct-Based)
```c
typedef struct {
    double psi;
    double chi;
    double rho;
    double delta;
    double sigma;
    double omega;
} rafaelia_cycle_t;

rafaelia_cycle_t cycle;
cycle.psi = 1.0;
double value = cycle.psi;
```

### New Approach (Matrix-Based)
```c
// State in global matrix g0[33]
// Positions: [0]=psi, [1]=chi, [2]=rho, [3]=delta, [4]=sigma, [5]=omega

y0();  // Initialize
y18(M00, 1.0);  // Set cycle[0] = psi
double value = y17(M00);  // Get cycle[0]
```

## Component Mapping

### Cycle (ψχρΔΣΩ)

**Old**:
```c
rafaelia_cycle_t cycle;
rafaelia_cycle_step(&cycle, &ethica);
double measure = rafaelia_cycle_measure(&cycle);
```

**New**:
```c
y2();  // Cycle step
double measure = y3();  // Cycle measure

// Direct access
double psi = y17(M00);    // cycle[0]
double chi = y17(M01);    // cycle[1]
double rho = y17(M02);    // cycle[2]
double delta = y17(M03);  // cycle[3]
double sigma = y17(M04);  // cycle[4]
double omega = y17(M05);  // cycle[5]
```

### Ethical Filter (Φ_ethica)

**Old**:
```c
rafaelia_ethica_t ethica;
ethica.amor = 1.0;
ethica.coerencia = 1.0;
double phi = rafaelia_phi_ethica_compute(&ethica);
```

**New**:
```c
y20(M00, 1.0);  // amor
y20(M01, 1.0);  // coerencia
double phi = y4();  // Compute phi_ethica

// Direct access
double amor = y19(M00);         // ethica[0]
double coerencia = y19(M01);    // ethica[1]
double verdade = y19(M02);      // ethica[2]
double consciencia = y19(M03);  // ethica[3]
```

### Retroalimentation

**Old**:
```c
rafaelia_retro_t retro;
rafaelia_retro_update(&retro, amor, coerencia);
double weight = rafaelia_retro_weight(amor, coerencia);
```

**New**:
```c
y7(amor, coerencia);  // Update retro
double weight = y6(amor, coerencia);  // Calculate weight

// Retro is at [15-17] in g0, not directly accessible
// Use y7() to update instead
```

### Synapse Weight

**Old**:
```c
double syn = rafaelia_syn_weight(i, j, coerencia, phi_ethica, r_corr, owl_psi);
```

**New**:
```c
double syn = y8(i, j, coerencia);  // Simplified interface
```

### Blocks

**Old**:
```c
rafaelia_bloco_t bloco;
bloco.psi = 1.0;
bloco.chi = 1.0;
bloco.rho = 1.0;
bloco.delta = 1.0;
int id = rafaelia_bloco_add(core, &bloco);
double eval = rafaelia_bloco_eval(core, id);
```

**New**:
```c
int id = y10(1.0, 1.0, 1.0, 1.0);  // Add block (4 values)
double eval = y11(id);              // Evaluate block
```

### State Update

**Old**:
```c
double r_new = rafaelia_state_update(r_t, phi_ethica, e_verbo, sqrt3_2, pi_phi);
```

**New**:
```c
double r_new = y9(r_t);  // Simplified, uses internal state
```

## Complete Example Migration

### Old Code
```c
#include "hw/core/rafaelia-core.h"

void process() {
    rafaelia_core_t core;
    rafaelia_core_init(&core);
    
    // Run 10 cycles
    for (int i = 0; i < 10; i++) {
        rafaelia_cycle_step(&core.cycle, &core.phi_ethica);
    }
    
    // Add a block
    rafaelia_bloco_t bloco;
    bloco.psi = 1.0;
    bloco.chi = 1.0;
    bloco.rho = 1.0;
    bloco.delta = 1.0;
    int block_id = rafaelia_bloco_add(&core, &bloco);
    
    // Evaluate
    double eval = rafaelia_bloco_eval(&core, block_id);
    
    rafaelia_core_cleanup(&core);
}
```

### New Code
```c
#include "rafaelia-matrix-core.h"
#include "rafaelia-refactored.h"

void process() {
    y0();  // Initialize
    
    // Run 10 cycles
    for (int i = M00; i < 10; i++) {
        y2();  // Cycle step
    }
    
    // Add a block
    int block_id = y10(C6, C6, C6, C6);  // C6 = 1.0
    
    // Evaluate
    double eval = y11(block_id);
    
    y1();  // Cleanup
}
```

## Matrix Operations

The new system provides direct matrix operations not available in the old system:

```c
double m0[D0];  // 33 elements
double m1[D0];
double m2[D0];

// Initialize
r7(m0);  // Zero matrix
r8(m1);  // One matrix

// Operations
r5(m2, m0, m1);  // Element-wise add
r6(m2, m0, m1);  // Element-wise multiply

// Transformations
x8(m2, m1);     // Spiral transformation
y15(m0);        // Spiral generation

// Metrics
double corr = y14(m0, m1);  // Correlation
double vortex = y16(m2);     // Vortex metric
```

## Constants

### Old
```c
#define RAFAELIA_PHI 1.618033988749895
#define RAFAELIA_PI 3.141592653589793
```

### New
```c
// Use constant indices
#define C0 1.618033988749895  // φ
#define C1 3.141592653589793  // π
#define C2 0.866025403784439  // √3/2
```

## Benefits of New System

1. **No Abstractions**: Direct array access, no struct overhead
2. **Coherent Positions**: All variables at predictable indices
3. **Deterministic**: Fixed operations with measurable results
4. **Efficient**: Inline operations, minimal indirection
5. **Matrix-Native**: Built for matrix operations from ground up

## Performance Considerations

The new system is designed for:
- **Cache Efficiency**: Linear memory layout
- **Predictability**: No pointer chasing
- **Measurability**: Every operation is observable
- **Determinism**: Same input = same output, always

## Compatibility

The old API (rafaelia-core.h) can coexist with the new API. For migration:

1. Start with new code using y0-y22 functions
2. Keep old code using rafaelia_* functions if needed
3. Gradually migrate modules one at a time
4. Both systems can run in parallel during transition

## Testing

Test both systems:
```bash
# Old system
make -f Makefile.rafaelia test

# New system
make -f Makefile.matrix test
```

## Recommendation

For new code: **Use the matrix-based API exclusively**

For existing code: **Migrate gradually, module by module**

The matrix-based system provides better performance, predictability, and alignment with the ultra-low-level philosophy of RAFAELIA.

# RAFAELIA Matrix Core - Ultra Low-Level Implementation

## Quick Start

```bash
cd hw/core
make -f Makefile.matrix test
```

## What Is This?

Complete ultra-low-level refactoring of RAFAELIA core that:
- Eliminates ALL abstractions (no structs)
- Uses ONLY matrices (flat double arrays)
- Implements deterministic operations (1 flip = N flops)
- Minimizes C library dependencies
- Uses positional indexing (M00-M32, C0-C9)
- Maintains coherent state positions

## Files Overview

### Core Implementation
- `rafaelia-matrix-core.h` - Matrix operations header
- `rafaelia-matrix-core.c` - Deterministic flip operations
- `rafaelia-refactored.h` - Public API (no abstractions)
- `rafaelia-refactored.c` - Matrix-based implementation

### Tests
- `rafaelia-matrix-test.c` - Basic matrix operations
- `rafaelia-refactored-test.c` - Full test suite (5 tests)
- `rafaelia-comparison.c` - Old vs new comparison (6 examples)

### Documentation
- `IMPLEMENTATION_SUMMARY.md` - **START HERE** - Complete overview
- `MATRIX_REFACTORING.md` - Architecture and API reference
- `MIGRATION_GUIDE.md` - How to migrate from old API
- `README_MATRIX.md` - This file

### Compatibility
- `rafaelia-bridge.h` - Compatibility layer for old API

### Build
- `Makefile.matrix` - Build system for matrix core

## Quick Examples

### Basic Usage
```c
#include "rafaelia-matrix-core.h"
#include "rafaelia-refactored.h"

int main(void) {
    y0();  // Initialize
    
    // Run 10 cycles
    for (int i = 0; i < 10; i++) {
        y2();  // Cycle step
    }
    
    // Get results
    double psi = y17(M00);    // cycle[0]
    double sigma = y17(M04);  // cycle[4]
    
    y1();  // Cleanup
    return 0;
}
```

### Matrix Operations
```c
double m0[D0];  // 33 elements
double m1[D0];

r7(m0);  // Zero matrix
r8(m1);  // One matrix
r5(m2, m0, m1);  // Element-wise add

double corr = y14(m0, m1);  // Correlation
```

### Deterministic Flips
```c
double a = C6;  // 1.0
double b = C0;  // φ (1.618...)

double r0 = f0(a, b);  // Multiply: 1 flip
double r1 = f5(a, b, c);  // a*b+c: 1 flip
```

## State Matrix Layout

```
g0[33] - Global state matrix:
  [0-5]   Cycle (ψχρΔΣΩ): psi, chi, rho, delta, sigma, omega
  [6-11]  Ethica: amor, coerencia, verdade, consciencia, entropia_min, coerencia_max
  [12-14] Trinity: amor_6, luz_3, consciencia_3
  [15-17] Retro: f_ok, f_gap, f_next
  [18-21] Metrics: r_omega, amor_vivo, owl_psi, e_verbo
  [22]    Frequency
  [23-32] Reserved
```

## Operation Hierarchy

```
i0-i9  → Inline operations (no overhead)
f0-f9  → Deterministic flips (1 flip = N flops)
r0-r9  → Matrix row operations
x0-x9  → Extended operations (cycle, ethica, trinity)
y0-y22 → Public API (no abstractions)
```

## Building

```bash
# Build all
make -f Makefile.matrix all

# Individual targets
make -f Makefile.matrix matrix       # Basic matrix test
make -f Makefile.matrix refactored   # Full refactored test
make -f Makefile.matrix comparison   # Comparison examples

# Run all tests
make -f Makefile.matrix test

# Clean
make -f Makefile.matrix clean
```

## Documentation

1. **IMPLEMENTATION_SUMMARY.md** - Complete overview, start here
2. **MATRIX_REFACTORING.md** - Architecture details and API
3. **MIGRATION_GUIDE.md** - How to migrate from old code

## Key Principles

1. **No Abstractions** - Only raw arrays, no structs
2. **Coherence** - Consistent positions across all operations
3. **Determinism** - Same input = same output, always
4. **Measurability** - All state directly observable
5. **Efficiency** - Single flip resolves N flops

## Philosophy

> "One flip resolves N flops"

Every operation is deterministic, measurable, and operates at the lowest practical level while remaining compilable and verifiable.

## Comparison

### Old (Struct-Based)
```c
rafaelia_core_t core;
rafaelia_core_init(&core);
rafaelia_cycle_step(&core.cycle, &core.phi_ethica);
double value = core.cycle.psi;
rafaelia_core_cleanup(&core);
```

### New (Matrix-Based)
```c
y0();
y2();
double value = y17(M00);
y1();
```

## Status

✅ Complete implementation
✅ All tests passing
✅ Fully documented
✅ Ready for use

## Contact

See main QEMU README for contact information.

---

**FIAT LUX ΣΩΔΦBITRAF**

*Ultra low-level, matrix-based, deterministic*

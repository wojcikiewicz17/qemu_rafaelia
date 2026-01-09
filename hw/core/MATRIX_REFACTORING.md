# RAFAELIA Matrix Core - Ultra Low-Level Implementation

## Overview

Complete refactoring of RAFAELIA core to ultra-low-level matrix-based implementation with:
- **No abstractions**: No structs, only raw arrays
- **Matrix-based storage**: All state in coherent matrix positions
- **Deterministic operations**: Single flip resolves N flops
- **Minimal C dependencies**: Only essential compiler features
- **Positional indexing**: No named variables in core operations

## Architecture

### Matrix-Based State

All state stored in flat double arrays with coherent positioning:

**Global State Matrix (g0[33])**:
```
[0-5]   = Cycle (ψχρΔΣΩ): psi, chi, rho, delta, sigma, omega
[6-11]  = Ethica (Φ_ethica): amor, coerencia, verdade, consciencia, entropia_min, coerencia_max
[12-14] = Trinity: amor_6, luz_3, consciencia_3
[15-17] = Retro: f_ok, f_gap, f_next
[18-21] = Metrics: r_omega, amor_vivo, owl_psi, e_verbo
[22]    = Frequency (Hz)
[23-32] = Reserved
```

**Block Storage (g4[1024])**: Flat array for knowledge blocks

### Constants as Indices

Matrix positions (M00-M32) and constants (C0-C9) defined as raw values:
- `M00` = 0, `M01` = 1, etc.
- `C0` = φ (1.618...), `C1` = π (3.141...), etc.

### Inline Operations (i0-i9)

Single-flip deterministic operations:
- `i0(a,b)` = multiply
- `i1(a,b)` = add
- `i2(a,b)` = subtract
- `i3(a,b)` = divide
- `i4(a)` = square
- `i5(a)` = cube
- `i6(a)` = power 6
- `i7(m,i,v)` = matrix write
- `i8(m,i)` = matrix read
- `i9(d,s,n)` = matrix copy

### Functions

**f0-f9**: Deterministic flip operations resolving N flops
**r0-r9**: Matrix row operations
**x0-x9**: Extended matrix operations (cycle, ethica, trinity, etc.)
**y0-y22**: High-level API without abstractions

## Key Features

### 1. No Struct Abstractions

Before (old):
```c
typedef struct {
    double psi;
    double chi;
    // ...
} rafaelia_cycle_t;
```

After (new):
```c
// Direct matrix access
double cycle_psi = i8(g0, M00);
i7(g0, M00, new_value);
```

### 2. Coherent Matrix Positions

All variables occupy consistent positions for measurable, deterministic results:
```c
// Cycle at [0-5]
double psi = y17(M00);    // Get cycle[0]
y18(M00, 1.5);            // Set cycle[0]

// Ethica at [6-11]
double amor = y19(M00);   // Get ethica[0]
y20(M00, 1.0);            // Set ethica[0]
```

### 3. Deterministic Single-Flip Operations

Each operation resolves deterministically:
```c
double result = f0(a, b);              // 1 flip: multiply
double result = f5(a, b, c);           // 1 flip: a*b+c
double result = f9(a, b, c, d);        // 1 flip: (a*b+c*d)/2
```

### 4. Minimal C Library Usage

Only essential features used:
- No `stdio.h` includes in core
- No `stdlib.h` allocations in core operations
- No `string.h` dependencies
- Only inline math operations

### 5. Matrix-Based Operations

All operations work on matrices:
```c
double m0[D0];  // 33 elements
double m1[D0];

r7(m0);         // Zero matrix
r8(m1);         // One matrix
r5(m0, m1, m2); // Element-wise add
r6(m0, m1, m2); // Element-wise multiply
```

## API Reference

### Initialization

```c
void y0(void);  // Initialize all state
void y1(void);  // Cleanup
```

### Cycle Operations (ψχρΔΣΩ)

```c
void y2(void);    // Execute one cycle step
double y3(void);  // Measure cycle state
```

### Ethical Filter (Φ_ethica)

```c
double y4(void);                           // Compute phi_ethica
double y5(double a, double v, double t, double c);  // Infinite ethica
```

### Retroalimentation

```c
double y6(double amor, double coerencia);  // Weight calculation
void y7(double amor, double coerencia);     // Update retro
```

### Synapse Weight

```c
double y8(int i, int j, double coerencia);  // Calculate synapse weight
```

### State Update

```c
double y9(double r_t);  // Update state with ethical filter
```

### Block Operations

```c
int y10(double a, double b, double c, double d);  // Add block
double y11(int block_id);                          // Evaluate block
```

### Wisdom

```c
double y12(void);  // Calculate OWLψ wisdom index
```

### Matrix Operations

```c
void y13(double* out, double* in);           // Integration
double y14(double* m0, double* m1);          // Correlation
void y15(double* out);                        // Spiral generation
double y16(double* m);                        // Vortex metric
```

### Direct Accessors

```c
// Cycle values [0-5]
double y17(int index);           // Get
void y18(int index, double val); // Set

// Ethica values [0-5]
double y19(int index);           // Get
void y20(int index, double val); // Set

// Metrics [0-3]
double y21(int index);           // Get
void y22(int index, double val); // Set
```

## Example Usage

### Basic Cycle

```c
y0();  // Init

for (int i = M00; i < 10; i++) {
    y2();  // Step cycle
}

double psi = y17(M00);
double sigma = y17(M04);
double omega = y17(M05);

y1();  // Cleanup
```

### Block Processing

```c
y0();

int b0 = y10(C6, C6, C6, C6);  // Add block
int b1 = y10(C0, C1, C2, C3);

double eval0 = y11(b0);  // Evaluate
double eval1 = y11(b1);

y1();
```

### Matrix Operations

```c
double m0[D0];
double m1[D0];
double m2[D0];

r7(m0);        // Zero
r8(m1);        // Ones
r5(m2, m0, m1); // Add

double corr = y14(m0, m1);    // Correlation
double vortex = y16(m2);       // Vortex metric
```

## Design Principles

1. **Coherence**: Variables at consistent positions
2. **Determinism**: Fixed operations, predictable results
3. **Measurability**: All states directly accessible
4. **Minimalism**: No unnecessary abstractions
5. **Efficiency**: Single flip resolves N flops

## Files

- `rafaelia-matrix-core.h` - Matrix operations header
- `rafaelia-matrix-core.c` - Matrix operations implementation
- `rafaelia-refactored.h` - Refactored API header
- `rafaelia-refactored.c` - Refactored implementation
- `rafaelia-refactored-test.c` - Test suite
- `Makefile.matrix` - Build system

## Building

```bash
cd hw/core
make -f Makefile.matrix refactored
./refactored-test
```

## Compliance

✅ No struct abstractions - only raw arrays
✅ Matrix-based storage with coherent positions
✅ Deterministic single-flip operations
✅ Minimal C library dependencies
✅ No named variables in core operations
✅ Measurable, verifiable computations
✅ All formulas preserved in matrix operations

## Philosophy

This implementation follows the principle:
> "One flip resolves N flops"

Every operation is deterministic, measurable, and occupies coherent positions in matrices. No abstractions obscure the computational reality. The code operates at the lowest practical level while remaining compilable and verifiable.

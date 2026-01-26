/* RAFAELIA LOWLEVEL OPS - deterministic C/ASM primitives */
/* Baremetal compatible - no C library dependencies */

#include "rafaelia-lowlevel.h"

/* Core arithmetic - use inline assembly when available */
double rl_add(double a, double b) {
#if defined(__x86_64__)
    double r;
    __asm__ volatile ("addsd %1, %0" : "=x"(r) : "x"(b), "0"(a));
    return r;
#elif defined(__aarch64__)
    double r;
    __asm__ volatile ("fadd %d0, %d1, %d2" : "=w"(r) : "w"(a), "w"(b));
    return r;
#else
    return a + b;
#endif
}

double rl_sub(double a, double b) {
#if defined(__x86_64__)
    double r;
    __asm__ volatile ("subsd %1, %0" : "=x"(r) : "x"(b), "0"(a));
    return r;
#elif defined(__aarch64__)
    double r;
    __asm__ volatile ("fsub %d0, %d1, %d2" : "=w"(r) : "w"(a), "w"(b));
    return r;
#else
    return a - b;
#endif
}

double rl_mul(double a, double b) {
#if defined(__x86_64__)
    double r;
    __asm__ volatile ("mulsd %1, %0" : "=x"(r) : "x"(b), "0"(a));
    return r;
#elif defined(__aarch64__)
    double r;
    __asm__ volatile ("fmul %d0, %d1, %d2" : "=w"(r) : "w"(a), "w"(b));
    return r;
#else
    return a * b;
#endif
}

double rl_div(double a, double b) {
#if defined(__x86_64__)
    double r;
    __asm__ volatile ("divsd %1, %0" : "=x"(r) : "x"(b), "0"(a));
    return r;
#elif defined(__aarch64__)
    double r;
    __asm__ volatile ("fdiv %d0, %d1, %d2" : "=w"(r) : "w"(a), "w"(b));
    return r;
#else
    return a / b;
#endif
}

double rl_abs(double a) {
    return (a < 0.0) ? -a : a;
}

double rl_square(double a) {
    return rl_mul(a, a);
}

double rl_cube(double a) {
    double t0 = rl_mul(a, a);
    return rl_mul(t0, a);
}

double rl_pow6(double a) {
    double t0 = rl_mul(a, a);
    double t1 = rl_mul(t0, t0);
    double t2 = rl_mul(t1, t0);
    return t2;
}

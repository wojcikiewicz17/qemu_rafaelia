/* RAFAELIA LOWLEVEL OPS - deterministic C/ASM primitives */
/* Baremetal compatible - no C library dependencies */

#ifndef RAFAELIA_LOWLEVEL_H
#define RAFAELIA_LOWLEVEL_H

/* Low-level arithmetic primitives */
double rl_add(double, double);
double rl_sub(double, double);
double rl_mul(double, double);
double rl_div(double, double);

double rl_abs(double);

double rl_square(double);
double rl_cube(double);
double rl_pow6(double);

#endif

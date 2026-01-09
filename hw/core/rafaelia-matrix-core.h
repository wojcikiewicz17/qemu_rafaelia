/* RAFAELIA MATRIX CORE - Ultra low level, no abstractions */

#ifndef RAFAELIA_MATRIX_CORE_H
#define RAFAELIA_MATRIX_CORE_H

/* Matrix positions - coherent indexing */
#define M00 0
#define M01 1
#define M02 2
#define M03 3
#define M04 4
#define M05 5
#define M06 6
#define M07 7
#define M08 8
#define M09 9
#define M10 10
#define M11 11
#define M12 12
#define M13 13
#define M14 14
#define M15 15
#define M16 16
#define M17 17
#define M18 18
#define M19 19
#define M20 20
#define M21 21
#define M22 22
#define M23 23
#define M24 24
#define M25 25
#define M26 26
#define M27 27
#define M28 28
#define M29 29
#define M30 30
#define M31 31
#define M32 32

/* Constants as raw values */
#define C0 1.618033988749895
#define C1 3.141592653589793
#define C2 0.866025403784439
#define C3 0.963999
#define C4 0.758
#define C5 144000.0
#define C6 1.0
#define C7 0.0
#define C8 0.1
#define C9 2.0
#define C10 0.001
#define C11 10.0
#define C12 1.05

/* Matrix dimensions */
#define D0 33
#define D1 42
#define D2 128
#define D3 1024

/* Operations */
void r0(double*,double*);
void r1(double*,double*);
void r2(double*,double*);
void r3(double*,double*);
void r4(double*,double*);
void r5(double*,double*,double*);
void r6(double*,double*,double*);
void r7(double*);
void r8(double*);
void r9(double*,double*);

/* Single flip deterministic operations */
double f0(double,double);
double f1(double,double);
double f2(double,double);
double f3(double,double);
double f4(double,double);
double f5(double,double,double);
double f6(double,double,double);
double f7(double);
double f8(double);
double f9(double,double,double,double);

/* Inline operations - 1 flip N flops */
static inline double i0(double a, double b) { return a*b; }
static inline double i1(double a, double b) { return a+b; }
static inline double i2(double a, double b) { return a-b; }
static inline double i3(double a, double b) { return a/b; }
static inline double i4(double a) { return a*a; }
static inline double i5(double a) { return a*a*a; }
static inline double i6(double a) { return a*a*a*a*a*a; }
static inline void i7(double*m,int i,double v){if(i>=M00&&i<=M32){m[i]=v;}}
static inline double i8(double*m,int i){return(i>=M00&&i<=M32)?m[i]:C7;}
static inline void i9(double*d,double*s,int n){int i=M00;while(i<n&&i<=M32){d[i]=s[i];i=i+1;}}

/* Extended operations */
void x0(double*);
void x1(double*,double*);
void x2(double*,double*);
void x3(double*,double*);
void x4(double*,double*);
void x5(double*,double*);
void x6(double*,double*,double*);
void x7(double*,double*,double*);
void x8(double*,double*);
void x9(double*,double*);

#endif

/* RAFAELIA REFACTORED TEST - Deterministic validation */

#include "rafaelia-matrix-core.h"
#include "rafaelia-refactored.h"

/* Test without abstractions */
int t1(void) {
    y0();
    
    int i=M00;
    while(i<10){
        y2();
        i=i+1;
    }
    
    double v0=y17(M00);
    double v1=y17(M01);
    double v2=y17(M04);
    double v3=y4();
    double v4=y3();
    
    int b0=y10(v0,v1,v2,v3);
    double b1=y11(b0);
    
    double w0=y12();
    
    double m0[D0];
    double m1[D0];
    y15(m0);
    y15(m1);
    double c0=y14(m0,m1);
    double vm=y16(m0);
    
    y1();
    
    return M00;
}

/* Integration test */
int t2(void) {
    y0();
    
    int n=M00;
    while(n<100){
        y2();
        
        double v0=y17(M00);
        double v1=y19(M00);
        y7(v0,v1);
        
        if(n%10==0){
            double v2=y4();
            y22(M00,v2);
        }
        
        n=n+1;
    }
    
    double r0=y21(M00);
    double r1=y21(M01);
    
    y1();
    
    return M00;
}

/* Block test */
int t3(void) {
    y0();
    
    int b0=y10(C6,C6,C6,C6);
    int b1=y10(C0,C1,C2,C3);
    int b2=y10(C6,C7,C8,C9);
    
    double e0=y11(b0);
    double e1=y11(b1);
    double e2=y11(b2);
    
    y1();
    
    return M00;
}

/* Matrix operations test */
int t4(void) {
    double m0[D0];
    double m1[D0];
    double m2[D0];
    
    r7(m0);
    r8(m1);
    
    r5(m2,m0,m1);
    r6(m2,m0,m1);
    
    x8(m2,m1);
    double vm=y16(m2);
    
    return M00;
}

/* Deterministic flip test */
int t5(void) {
    double a0=C6;
    double a1=C0;
    double a2=C1;
    double a3=C2;
    
    double r0=f0(a0,a1);
    double r1=f1(a0,a1);
    double r2=f5(a0,a1,a2);
    double r3=f6(a0,a1,a2);
    double r4=f9(a0,a1,a2,a3);
    
    return M00;
}

#ifdef TEST_STANDALONE
int main(void) {
    int r0=t1();
    int r1=t2();
    int r2=t3();
    int r3=t4();
    int r4=t5();
    return r0+r1+r2+r3+r4;
}
#endif

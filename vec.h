#include<stdio.h>

// c = a + b
void vec_add(double* a, double* b, double* c);
// c = a - b
void vec_sub(double* a, double* b, double* c);
// a dot b
double vec_dot(double* a, double* b);
// b = norm(a)
double vec_norm(double* a);
// b = a
void vec_copy(double* a, double* b);
// b = unit(a)
void vec_unit(double* a, double* b);
// c = a*b
void vec_mult(double a, double* b, double* c);
// c = | a - b |
double vec_dist(double* a, double* b);


void vec_print(double* a);
void vec_write(double* a, FILE* fp);

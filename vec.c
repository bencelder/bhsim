#include "vec.h"

int i;

//double* vec_add(double* a, double* b, double* c){
double* vec_add(double* a, double* b){
    static double c[3];

    for (i = 0; i < 3; i++){
        c[i] = a[i] + b[i];
    }
    return c;
}

void vec_print(double* a){
    printf("%f\t%f\t%f\n", a[0], a[1], a[2]);
}

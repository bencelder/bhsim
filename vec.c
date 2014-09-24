#include<math.h>

#include "vec.h"


extern int N_dim;

void vec_add(double* a, double* b, double* c){
    int i;
    for (i = 0; i < N_dim; i++){
        c[i] = a[i] + b[i];
    }
}

void vec_sub(double* a, double* b, double* c){
    int i;
    for (i = 0; i < N_dim; i++){
        c[i] = a[i] - b[i];
    }
}

double vec_dot(double* a, double* b){
    double c = 0;
    int i;

    for (i = 0; i < N_dim; i++){
        c += a[i] * b[i];
    }
    return c;
}

double vec_norm(double* a){
    return sqrt(vec_dot(a, a));
}

void vec_copy(double* a, double* b){
    int i;
    for (i = 0; i < N_dim; i++){
        b[i] = a[i];
    }
}

void vec_mult(double a, double* b, double* c){
    int i;
    for (i = 0; i < N_dim; i++){
        c[i] = a * b[i];
    }
}

double vec_dist(double* a, double* b){
    double temp[] = {0., 0.};
    vec_sub(a, b, temp);
    return vec_norm(temp);
}

void vec_print(double* a){
    int i;
    for (i = 0; i < N_dim; i++){
        printf("%f\t", a[i]);
    }
    printf("\n");
}

void vec_write(double* a, FILE* fp){
    int i;
    for (i = 0; i < N_dim; i++){
        fprintf(fp, "%f\t", a[i]);
    }
    //printf("\n");
}

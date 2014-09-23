#include<stdio.h>

#include "vec.h"

int main(){
    /* Initialize the particles */

    /* advance */
    double a[] = {1., 2., 3.};
    double b[] = {1., 2., 3.};

    double* c = vec_add(a, b);
    vec_print(c);


    /* write snapshot to file */
    printf("Hello, world!\n");
}

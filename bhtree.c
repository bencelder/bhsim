#include "bhtree.h"
#include "vec.h"

Quad quad_init(double length, double* LL_corner, Quad quad){
    quad.length = length;
    vec_copy(LL_corner, quad.LL_corner);
    return quad;
}

bool quad_contains(double* vec, Quad quad){
    //vec_print(vec);
    //vec_print(quad.LL_corner);
    // optmization: check one at a time
    if (    vec[0] >= quad.LL_corner[0] &&
            vec[0] <  quad.LL_corner[0] + quad.length &&
            vec[1] >= quad.LL_corner[1] &&
            vec[1] <  quad.LL_corner[1] + quad.length){
        return true;
    }
    return false;
}

void quad_print(Quad q){
    vec_print(q.LL_corner);
    printf("%f\n", q.length);
}

Quad quad_SW(Quad q){
    Quad q2;
    q2 = quad_init( q.length / 2., q.LL_corner, q2 );
    return q2;
}

Quad quad_SE(Quad q){
    double newlength = q.length / 2.;
    double offset[] = { newlength, 0. };
    double new_corner[] = {0., 0.};
    vec_add( q.LL_corner, offset, new_corner );

    Quad q2;
    q2 = quad_init( newlength, new_corner, q2 );
    return q2;
}

#include "bhtree.h"
#include "vec.h"

Quad quad_init(double length, double* LL_corner, Quad quad){
    quad.length = length;
    vec_copy(LL_corner, quad.LL_corner);
    return quad;
}

bool quad_contains(double* vec, Quad quad){
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

Quad quad_NW(Quad q){
    double newlength = q.length / 2.;
    double offset[] = { 0., newlength };
    double new_corner[] = {0., 0.};
    vec_add( q.LL_corner, offset, new_corner );

    Quad q2;
    q2 = quad_init( newlength, new_corner, q2 );
    return q2;
}

Quad quad_NE(Quad q){
    double newlength = q.length / 2.;
    double offset[] = { newlength, newlength };
    double new_corner[] = {0., 0.};
    vec_add( q.LL_corner, offset, new_corner );

    Quad q2;
    q2 = quad_init( newlength, new_corner, q2 );
    return q2;
}

bool particle_in(Particle p, Quad q){
    return quad_contains(p.pos, q);
}

void particle_print( Particle p ){
    printf("Particle position: \t");
    vec_print(p.pos);
    printf("Particle mass    : \t");
    printf("%f\n", p.mass);
}

Particle particle_add(Particle p1, Particle p2){
    // add the masses, find the center of mass
    Particle p3;
    p3.mass = p1.mass + p2.mass;

    double temp1[] = {0.,0.};
    double temp2[] = {0.,0.};
    vec_mult( p1.mass, p1.pos, temp1);
    vec_mult( p2.mass, p2.pos, temp2);

    vec_add( temp1, temp2, p3.pos );
    vec_mult( 1./p3.mass, p3.pos, p3.pos );
    
    return p3;
}

void bhtree_print(BHTree bht){
    printf("Particle in BHT:\n");
    particle_print( bht.body );
    printf("Quad in BHT:\n");
    quad_print( bht.quad );
    // print children?
}

BHTree bhtree_new(Quad q){
    BHTree bht;
    bht.quad = q;
    bht.body.mass = 0.;
    bht.is_leaf = true;
    return bht;
}

void bhtree_insert(Particle p, BHTree* bht){
    double m = bht->body.mass;
    // if there are no particles
    if (m == 0.){
        // put this particle in
        bht->body = p;
    }
    // if it's a leaf
    else if (bht->is_leaf == true){
        // initialize the four quadrants
        BHTree SW = bhtree_new( quad_SW( bht->quad ) );
        bht->SW = &SW;
    }
}

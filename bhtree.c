#include<stdlib.h>

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

bool particle_equal(Particle p, Particle q){
    if (p.mass   != q.mass)   return false;
    if (p.pos[0] != q.pos[0]) return false;
    if (p.pos[1] != q.pos[1]) return false;
    return true;
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
    //printf("Particle in BHT:\n");
    particle_print( bht.body );

    printf("SW %p\n", bht.SW);
    if (bht.SW != NULL) bhtree_print(*(bht.SW));
    printf("NW %p\n", bht.NW);
    if (bht.NW != NULL) bhtree_print(*(bht.NW));
    printf("NE %p\n", bht.NE);
    if (bht.NE != NULL) bhtree_print(*(bht.NE));
    printf("SE %p\n", bht.SE);
    if (bht.SE != NULL) bhtree_print(*(bht.SE));
}

BHTree* bhtree_new(Quad q){
    BHTree* bht;
    bht = malloc( sizeof( BHTree ) );
    //bht = malloc( sizeof(bht) );
    bht->quad = q;
    bht->body.mass = 0.;
    bht->is_leaf = true;
    bht->SW = NULL;
    bht->NW = NULL;
    bht->NE = NULL;
    bht->SE = NULL;
    return bht;
}

void bhtree_put_into_correct_quad(Particle p, BHTree* bht){
    if (quad_contains(p.pos, bht->SW->quad)){
        bhtree_insert(p, bht->SW);
    }
    else if (quad_contains(p.pos, bht->NW->quad)){
        bhtree_insert(p, bht->NW);
    }
    else if (quad_contains(p.pos, bht->NE->quad)){
        bhtree_insert(p, bht->NE);
    }
    else if (quad_contains(p.pos, bht->SE->quad)){
        bhtree_insert(p, bht->SE);
    }
}

void bhtree_insert(Particle p, BHTree* bht){
    double m = bht->body.mass;
    // if there are no particles
    if (m == 0.){
        //printf("No particles in bht!\n");
        // put this particle in
        bht->body = p;
    }
    // if it's an internal node
    else if (bht->is_leaf == false){
        //printf("It's an internal node!\n");
        // update the COM and total mass
        bht->body = particle_add(p, bht->body);
        // put the particle into the correct quadrant
        bhtree_put_into_correct_quad(p, bht);
    }
    // if it's an external node
    else if (bht->is_leaf == true){
        //printf("It's a leaf!\n");
        // if it's a leaf, then bht->body represents
        // a single particle.  We'll initialize the 
        // quadrants and then insert both into the 
        // correct one(s).

        // initialize the four quadrants
        //BHTree* SW = malloc( sizeof( BHTree ) );
        //bht->SW = SW;
        //BHTree SW = bhtree_new( quad_SW( bht->quad ) );
        //bht->SW = &SW;
        //BHTree NW = bhtree_new( quad_NW( bht->quad ) );
        bht->SW = bhtree_new( quad_SW( bht->quad ) );
        bht->NW = bhtree_new( quad_NW( bht->quad ) );
        bht->NE = bhtree_new( quad_NE( bht->quad ) );
        bht->SE = bhtree_new( quad_SE( bht->quad ) );
        //bht->NW = &NW;
        //BHTree NE = bhtree_new( quad_NE( bht->quad ) );
        //bht->NE = &NE;
        //BHTree SE = bhtree_new( quad_SE( bht->quad ) );
        //bht->SE = &SE;

        // put the two particles into the correct node(s)
        bhtree_put_into_correct_quad(p, bht);
        bhtree_put_into_correct_quad(bht->body, bht);

        // update the COM and mass info
        bht->body = particle_add(p, bht->body);
        bht->is_leaf = false;
    }
}

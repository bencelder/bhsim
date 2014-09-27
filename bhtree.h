#include <stdbool.h>

typedef struct{
    double pos[2];
    double vel[2];
    double mass;
}Particle;

typedef struct{
    double length;
    double LL_corner[2];
}Quad;

typedef struct BHTree {
    Particle body;
    Quad     quad;
    bool     is_leaf;
    struct BHTree* NW;
    struct BHTree* NE;
    struct BHTree* SW;
    struct BHTree* SE;

}BHTree;

Quad quad_init(double length, double* LL_corner, Quad quad);

bool quad_contains(double* vec, Quad quad);
void quad_print(Quad q);

Quad quad_SW(Quad q);
Quad quad_SE(Quad q);
Quad quad_NW(Quad q);
Quad quad_NE(Quad q);

void particle_print( Particle p );

bool particle_in(Particle p, Quad q);
Particle particle_add(Particle p1, Particle p2);

void bhtree_print(BHTree);
// New BH-Tree with no bodies, representing the quadrant.
BHTree bhtree_new(Quad q);
void bhtree_insert(Particle p, BHTree* bht);

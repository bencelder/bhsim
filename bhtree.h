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

Quad quad_init(double length, double* LL_corner, Quad quad);

bool quad_contains(double* vec, Quad quad);
void quad_print(Quad q);

Quad quad_SW(Quad q);
Quad quad_SE(Quad q);
Quad quad_NW(Quad q);
Quad quad_NE(Quad q);

typedef struct{
    Particle body;
    double m;
}BHTree;


#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<limits.h>

#include "params.h"
#include "vec.h"


double T  = 1.;
double dT = 0.1;

typedef struct{
    double pos[2];
    double vel[2];
}Particle;

/* Returns a random double in the range [0, 1) */
double rand_num(){
    int r = rand();
    double c = (double) r / RAND_MAX;
    return c;
}

void init_particles(Particle* particles){
    //double r, theta;
    Particle p;
    
    int i;
    for (i = 0; i < N_part; i++){
        p = particles[i];
        particles[i].pos[0] = rand_num();
        particles[i].pos[1] = rand_num();
    }
    //printf("%f\n", rand_num());
}

void write_particles(Particle* particles){
    //Particle p;
    int i;
    FILE* fp;
    fp = fopen("data/start.dat", "w+");
    for (i = 0; i < N_part; i++){
        vec_write(particles[i].pos, fp);
        fprintf(fp, "\n");
    }
    fclose(fp);
}

int main(){
    printf("Max int: %d\n", INT_MAX);
    printf("Size of Particle: %lu\n", sizeof(Particle));

    // init random number generator
    //srand(1);
    srand(time(NULL));
    float t = 0;
    /* Initialize the particles */
    Particle* particles;
    //Particle particles[N_part];
    particles = malloc( N_part * sizeof(Particle) );

    printf("Size of Particles array: %lu\n", sizeof(particles));

    init_particles(particles);

    write_particles(particles);

    //double pos[] = {0., 0.};
    //double vel[] = {.1, .1};
    //double temp1[N_dim];

    //Particle p;
    //vec_copy(vel, p.pos);
    //vec_print(vel);
    //vec_print(p.pos);

    while (t < T){
        /* advance */
        //vec_mult(dT, vel, temp1);
        //vec_add(pos, temp1, pos);

        //vec_print(pos);
        t += dT;
    }
}

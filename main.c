#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>

#include "params.h"
#include "vec.h"


double T  = 10.;
double dT = 0.01;
int steps_per_frame = 10;

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
    double r, theta;
    int i;
    for (i = 0; i < N_part; i++){
        r = 0.8 * rand_num();
        theta = 2 * M_PI * rand_num();
        particles[i].pos[0] = r * cos(theta);
        particles[i].pos[1] = r * sin(theta);
        particles[i].vel[0] = 0.1 * r * cos(theta);
        particles[i].vel[1] = 0.1 * r * sin(theta);
    }
    //printf("%f\n", rand_num());
}

void write_particles(Particle* particles, char* fname){
    //Particle p;
    int i;
    FILE* fp;
    fp = fopen(fname, "w+");
    for (i = 0; i < N_part; i++){
        vec_write(particles[i].pos, fp);
        //vec_write(particles[i].vel, fp);
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void force(Particle p1, Particle p2, double* f){
    // F = G m1 m2 r / r^3 
    // F = G m1 m2 r / (r + a)^3 
    // where a is a convergence term
    //double a = 0.01;
    //double r = 1;
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

    //write_particles(particles, "data/start.dat");

    //double pos[] = {0., 0.};
    //double vel[] = {.1, .1};
    //double temp1[N_dim];

    //Particle p;
    //vec_copy(vel, p.pos);
    //vec_print(vel);
    //vec_print(p.pos);

    int i = 0;
    int steps_since_frame = steps_per_frame;
    while (t < T){

        /* advance */
        int j;
        double temp[] = {0., 0.};
        for (j = 0; j < N_part; j++){
            vec_mult(dT, particles[j].vel, temp);
            vec_add(particles[j].pos, temp, particles[j].pos);
        }

        /* update the velocities */

        /* write the output */
        if (steps_since_frame >= steps_per_frame){
            printf("%f\n", t);
            char savename[256];
            sprintf(savename, "data/%05d.dat", i);
            printf("%s\n", savename);
            write_particles(particles, savename);

            steps_since_frame = 0;
        }
        steps_since_frame++;

        t += dT;
        i ++;
    }
}

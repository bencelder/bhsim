#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>

#include "params.h"
#include "main.h"
#include "vec.h"
#include "bhtree.h"


double T  = 10.;
double dT = 0.01;
int fps = 24;

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
        particles[i].mass   = 1.;
        //particles[i].vel[0] = 0.1 * r * cos(theta);
        //particles[i].vel[1] = 0.1 * r * sin(theta);
    }
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
    double a = 0.01;
    double G = 2e-3 / N_part;
    double r[] = {0., 0.};
    double r_mag;
    vec_sub(p2.pos, p1.pos, r);
    r_mag = vec_norm(r);
    vec_mult(G * p1.mass * p2.mass / pow( r_mag + a, 3.), r, f);
}

void build_bht(Particle* particles, BHTree* bht){
    int i;
    for (i = 0; i < N_part; i++){
        bhtree_insert(particles[i], bht);
    }
}

void bht_net_force(Particle p, BHTree* bht){
    double theta = 0.5;
    double s_over_d;
    printf("%f\n", bht->body.mass);
    
}

void brute_net_force(Particle p, Particle* particles, double* f){
    int k;
    double temp[] = {0., 0.};
    for (k = 0; k < N_part; k++){
        // don't compute self-force
        if (particle_equal(p, particles[k])) continue;

        // get the force between these 2 particles
        force(p, particles[k], temp);
        vec_add(f, temp, f);
    }
}

int main(){
    double seconds_per_frame = 1. / fps;
    int steps_per_frame = seconds_per_frame / dT;

    // init random number generator
    //srand(1);
    srand(time(NULL));
    
    /* Initialize the particles */
    Particle* particles;
    // malloc puts the particle on the heap
    particles = malloc( N_part * sizeof(Particle) );

    printf("%lu b \t per particle\n", sizeof(Particle));
    printf("%lu kb \t total memory usage\n",
            N_part * sizeof(Particle) / 1024);

    init_particles(particles);

    //write_particles(particles, "data/start.dat");
    int i = 0;
    int ss_count = 0;
    int steps_since_frame = steps_per_frame;
    double t = 0;
    while (t < T){

        // build the Barnes-Hut tree
        Quad anchor;
        double anchor_coords[] = {-1., -1.};
        anchor = quad_init(2., anchor_coords, anchor);

        BHTree bht = bhtree_new( anchor );
        build_bht(particles, &bht);

        /* advance the positions */
        int j;
        double temp[] = {0., 0.};
        for (j = 0; j < N_part; j++){
            vec_mult(dT, particles[j].vel, temp);
            vec_add(particles[j].pos, temp, particles[j].pos);
        }

        /* update the velocities */

        for (j = 0; j < N_part; j++){
            double f[] = {0., 0.};
            brute_net_force(particles[j], particles, f);
            vec_mult(dT, f, temp);
            vec_add(particles[j].vel, temp, particles[j].vel);
        }


        /* write the output */
        if (steps_since_frame >= steps_per_frame){
            char savename[256];
            sprintf(savename, "data/%05d.dat", ss_count);
            //printf("%s\n", savename);
            write_particles(particles, savename);

            ss_count++;
            steps_since_frame = 0;
        }
        steps_since_frame++;

        t += dT;
        i ++;
    }
    return 0;
}

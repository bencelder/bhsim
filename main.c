#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>
#include<omp.h>

#include "params.h"
#include "main.h"
#include "vec.h"
#include "bhtree.h"


/* Returns a random double in the range [0, 1) */
double rand_num(){
    int r = rand();
    double c = (double) r / RAND_MAX;
    return c;
}

void init_particles(Particle* particles){
    int i;
    /*
    for (i = 0; i < N_part; i++){
        particles[i].pos[0] = -2. + 4*rand_num();
        particles[i].pos[1] = -2. + 4*rand_num();
        particles[i].mass   = 1.;
    }
    */
    double r, theta;
    for (i = 0; i < N_part; i++){
        r = 1.5 * 0.8 * rand_num();
        theta = 2 * M_PI * rand_num();
        particles[i].pos[0] = r * cos(theta);
        particles[i].pos[1] = r * sin(theta);
        particles[i].mass   = 1.;
        vec_orthog( particles[i].pos, particles[i].vel );
        vec_mult(0.02, particles[i].vel, particles[i].vel);
        //particles[i].vel[0] = 0.1 * r * cos(theta);
        //particles[i].vel[1] = 0.1 * r * sin(theta);
    }
    /*
    */
}

void write_particles(Particle* particles, char* fname){
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
    if (r_mag == 0.) vec_copy(r, f);
    vec_mult(G * p1.mass * p2.mass / pow( r_mag + a, 3.), r, f);
}

void build_bht(Particle* particles, BHTree* bht){
    int i;
    for (i = 0; i < N_part; i++){
        bhtree_insert(particles[i], bht);
    }
}

void bht_net_force(Particle p, BHTree* bht, double* f){
    double theta = 0.5;
    double s_over_d;
    double distance;
    double temp[] = {0., 0.};

    // if node doesn't contain a body,
    if (bht->body.mass == 0.){
        // do nothing
        return;
    }

    // don't compute force with self
    //if (particle_equal(p, bht->body)) return;

    // if a node is sufficiently far away,
    distance = vec_dist(p.pos, bht->body.pos);
    s_over_d = bht->quad.length / distance;
    if (s_over_d < theta){
        //printf("Approximating!\n");
        // approximate all particles in the node as
        // a point particle
        force(p, bht->body, temp);
        vec_add(f, temp, f);
        return;
    }
    // if a node is too close,
    if (s_over_d >= theta){
        // recurse!
        if (bht->SW != NULL) bht_net_force(p, bht->SW, f);
        if (bht->NW != NULL) bht_net_force(p, bht->NW, f);
        if (bht->NE != NULL) bht_net_force(p, bht->NE, f);
        if (bht->SE != NULL) bht_net_force(p, bht->SE, f);
    }
}

void bhtree_free(BHTree* bht){
    if (bht->SW != NULL) bhtree_free(bht->SW);
    if (bht->NW != NULL) bhtree_free(bht->NW);
    if (bht->NE != NULL) bhtree_free(bht->NE);
    if (bht->SE != NULL) bhtree_free(bht->SE);
    free(bht);
}

void brute_net_force(Particle p, Particle* particles, double* f){
    int k;
    double temp[] = {0., 0.};
    for (k = 0; k < N_part; k++){
        // don't compute self-force
        if (particle_equal(p, particles[k])) continue;
        //printf("%f\n", vec_dist(p.pos, particles[k].pos));

        // get the force between these 2 particles
        force(p, particles[k], temp);
        vec_add(f, temp, f);
    }
}

void print_human_readable(int s){
    printf("%f hours \t %f minutes \t %f seconds",
            1., 1., 1.);
}

int main(){
    double seconds_per_frame = 1. / fps;
    int steps_per_frame = seconds_per_frame / dT;

    bool* progress = malloc( sizeof(bool) * 100 );
    int i = 0;
    for (i = 0; i < 100; i++){
        progress[i] = true;
    }

    int start_time, current_time;
    start_time = time(NULL);
    printf("%d\n", start_time);

    // init random number generator
    srand(time(NULL));
    
    /* Initialize the particles */
    Particle* particles;
    // malloc puts the particle on the heap
    particles = malloc( N_part * sizeof(Particle) );

    printf("%lu b \t per particle\n", sizeof(Particle));
    printf("%lu kb \t total memory usage\n",
            N_part * sizeof(Particle) / 1024);

    init_particles(particles);

    int ss_count = 0;
    int steps_since_frame = steps_per_frame;
    double t = 0;

    // initialize the box used for the BH tree, and the
    // tree itself
    BHTree* bht;
    Quad anchor;
    double anchor_coords[] = {-boxsize/2., -boxsize/2.};
    anchor = quad_init(boxsize, anchor_coords, anchor);

    bht = bhtree_new( anchor );

    while (t < T){

        // build the Barnes-Hut tree
        bht = bhtree_new( anchor );
        build_bht(particles, bht);

        /* advance the positions */
        int j;
        double temp[] = {0., 0.};
        //#pragma omp parallel for private(j, temp)
        for (j = 0; j < N_part; j++){

            // save previous positions
            if (particles[j].pos_last4!= NULL)
                vec_copy(particles[j].pos_last4, particles[j].pos_last5);
            if (particles[j].pos_last3!= NULL)
                vec_copy(particles[j].pos_last3, particles[j].pos_last4);
            if (particles[j].pos_last2!= NULL)
                vec_copy(particles[j].pos_last2, particles[j].pos_last3);
            if (particles[j].pos_last != NULL)
                vec_copy(particles[j].pos_last, particles[j].pos_last2);
            vec_copy(particles[j].pos, particles[j].pos_last);

            // compute the new position
            vec_mult(dT, particles[j].vel, temp);
            vec_add(particles[j].pos, temp, particles[j].pos);
        }

        double delta_t, delta_t_min, delta_t_max, delta_t_sum;
        double delta_t_sum_sq, delta_t_std_dev;
        delta_t = 0.;
        delta_t_min = 1.;
        delta_t_max = 0.;
        delta_t_sum = 0.;
        delta_t_sum_sq  = 0.;

        /* update the velocities */
        //#pragma omp parallel for private(j, temp)
        for (j = 0; j < N_part; j++){
            double f[] = {0., 0.};
            //brute_net_force(particles[j], particles, f);
            bht_net_force(particles[j], bht, f);
            vec_mult(dT, f, temp);

            vec_add(particles[j].vel, temp, particles[j].vel);

            // delta t calculation
            double fourth_deriv[] = {0., 0.};
            /* // 1st order
            fourth_deriv[0] =
                (particles[j].pos[0] - 4. * particles[j].pos_last[0] +
                6. * particles[j].pos_last2[0] - 4. * particles[j].pos_last3[0] +
                particles[j].pos_last4[0]) / (dT*dT*dT*dT);
            fourth_deriv[1] =
                (particles[j].pos[1] - 4. * particles[j].pos_last[1] +
                6. * particles[j].pos_last2[1] - 4. * particles[j].pos_last3[1] +
                particles[j].pos_last4[1]) / (dT*dT*dT*dT);
            */  // 2nd order
            fourth_deriv[0] =
                (  3. * particles[j].pos[0]
                - 14. * particles[j].pos_last[0]
                + 26. * particles[j].pos_last2[0]
                - 24. * particles[j].pos_last3[0]
                + 11. * particles[j].pos_last4[0]
                -  2. * particles[j].pos_last5[0]) / (dT*dT*dT*dT);
            fourth_deriv[1] =
                (  3. * particles[j].pos[1]
                - 14. * particles[j].pos_last[1]
                + 26. * particles[j].pos_last2[1]
                - 24. * particles[j].pos_last3[1]
                + 11. * particles[j].pos_last4[1]
                -  2. * particles[j].pos_last5[1]) / (dT*dT*dT*dT);

            delta_t = sqrt( vec_norm(f) / (particles[j].mass * vec_norm( fourth_deriv )) );

            // for Euler steps
            double alpha = 0.1;
            delta_t = alpha * particles[j].mass
                * vec_norm(particles[j].vel)
                / vec_norm( f );
            if (delta_t < delta_t_min) delta_t_min = delta_t;
            if (delta_t > delta_t_max) delta_t_max = delta_t;

            delta_t_sum += delta_t;
            delta_t_sum_sq  += delta_t * delta_t;


            // Euler step comparison
            /*
            printf("%e \t %e\n",
                    dT*vec_norm(particles[j].vel),
                    dT*dT*vec_norm(f)/particles[j].mass/2.);

            printf("%e \t %e\n",
                    dT*dT*vec_norm(f)/particles[j].mass,
                    dT*dT*dT*dT*vec_norm(fourth_deriv)/12.);

            printf("\n");
            */
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

        // clear the BH tree
        bhtree_free(bht);

        // flush stdout (useful on nohup)
        fflush(stdout);

        int percent_done = (int) (100 * t/T);
        if (progress[percent_done]){
            printf("%d%% complete\t", percent_done);
            progress[percent_done] = false;

            current_time = time(NULL);
            double time_elapsed = current_time - start_time;
            double time_left = (100./percent_done - 1.) * time_elapsed;
            printf("ETA: %d seconds\n", (int) time_left);

            double delta_t_mean;

            delta_t_mean = delta_t_sum / N_part;

            delta_t_std_dev = sqrt(delta_t_sum_sq / N_part
                    - delta_t_mean*delta_t_mean);

            printf("mean: %f \t std dev: %f\n",
                    delta_t_mean, delta_t_std_dev);

            //printf("min: %f \t max: %f \t avg: %f \n",
                    //delta_t_min, delta_t_max, delta_t_sum / N_part);
        }
    }
    return 0;
}

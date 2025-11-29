#ifndef PARTICLE_H
#include "particles.h"
#define PARTICLE_H
#endif

double dt = (double)1/TARGET_FPS;
static const Color values[] = {DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE,
                    DARKBROWN, GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN,
                    LIGHTGRAY, PINK, YELLOW, GREEN, SKYBLUE, PURPLE, BEIGE};

int num_colors = sizeof(values)/sizeof(values[0]);

System* create () {
    System *l = (System *)malloc(sizeof(System));
    l->head = NULL;
    l->tail = NULL;
    return l;
}

void update_pos(System *s){
    for(Particles *p = s->head; p != NULL; p = p->next){
        p->p_pos.x += p->p_vel.x*dt;
        p->p_pos.y += p->p_vel.y*dt;
        p->p_pos.z += p->p_vel.z*dt;
    }
}

bool is_empty(System *s){
    if(s->head == NULL){
        return 1;
    }
    return 0;
}

void remove_particle(System *s){
    Particles *remove = s->head;
    s->head = s->head->next;
    free(remove);
    if(s->head == NULL){
        s->tail = NULL;
    }
}

void insert_particle(System *s, Box b){
    Particles *p = (Particles *)malloc(sizeof(Particles));
    p->next = NULL;
    p->radius = (rand() % (CIRCLE_RAD_MAX - CIRCLE_RAD_MIN))+CIRCLE_RAD_MIN;
    p->p_pos.x = rand() % ((int)b.x - 2*p->radius) + p->radius;
    p->p_pos.y = rand() % ((int)b.y - 2*p->radius) + p->radius;
    p->p_pos.z = rand() % ((int)b.z - 2*p->radius) + p->radius;
    p->p_vel.x = (-MAX_SPEED/2) + rand() % MAX_SPEED;
    p->p_vel.y = (-MAX_SPEED/2) + rand() % MAX_SPEED;
    p->p_vel.z = (-MAX_SPEED/2) + rand() % MAX_SPEED;
    p->color = values[rand()%num_colors];
    p->mass = ((float)4/3)*M_PI*p->radius*p->radius*p->radius; // Multiply by a mass constant
    if(s->head == NULL){
        s->head = p;
        s->tail = p;
    }
    else{
        s->tail->next = p;
        s->tail = p;
    }
}

void destroy (System *l) {
    if (l != NULL) {	
        Particles *h = l->head;	
        while (h != NULL) {
            Particles *t = h->next; 
            free (h); 
            h = t;
        }
    }
    free(l);
}

void draw_particles(System *s){
    for(Particles *l = s->head; l != NULL; l=l->next){
        DrawSphere(l->p_pos, l->radius, l->color);
        DrawSphereWires(l->p_pos, l->radius, 10, 10, BLACK);

    }
}

double kinect_energy(System *s){
    double total_energy = 0;
    for(Particles *p = s->head; p != NULL; p = p->next){
        //Dividido por 10 mil para não deixar o valor tão elevado
        double vel = (p->p_vel.x*p->p_vel.x + p->p_vel.y*p->p_vel.y + p->p_vel.z*p->p_vel.z)/10000;
        total_energy += vel*p->mass/2;
    }
    return total_energy;
}

void resolve_collision(Particles *p1, Particles *p2, double nx, double ny, double nz) {
    // Reference: https://physics.stackexchange.com/questions/681396/elastic-collision-3d-eqaution/681419#681419
    double dvx = p1->p_vel.x - p2->p_vel.x;
    double dvy = p1->p_vel.y - p2->p_vel.y;
    double dvz = p1->p_vel.z - p2->p_vel.z;
    double dot_product = (nx * dvx) + (ny * dvy) + (nz * dvz);
    if (dot_product <= 0) return;
    double mass_sum = p1->mass + p2->mass;
    double factor_p1 = (2.0 * p2->mass / mass_sum) * dot_product;
    p1->p_vel.x -= factor_p1 * nx;
    p1->p_vel.y -= factor_p1 * ny;
    p1->p_vel.z -= factor_p1 * nz;
    double factor_p2 = (2.0 * p1->mass / mass_sum) * dot_product;
    p2->p_vel.x += factor_p2 * nx;
    p2->p_vel.y += factor_p2 * ny;
    p2->p_vel.z += factor_p2 * nz;
}

void fix_contacts(System *s, Box b){
    for(Particles *p1 = s->head; p1 != NULL; p1 = p1->next){
        if((p1->p_pos.x+p1->radius) > b.x){
            p1->p_pos.x = b.x - p1->radius;
            p1->p_vel.x = -(p1->p_vel.x);
        }
        else if((p1->p_pos.x-p1->radius) < 0){
            p1->p_pos.x = 0 + p1->radius;
            p1->p_vel.x = -(p1->p_vel.x);
        }
        if((p1->p_pos.y+p1->radius) > b.y){
            p1->p_pos.y = b.y - p1->radius;
            p1->p_vel.y = -(p1->p_vel.y);
        }
        else if((p1->p_pos.y-p1->radius) < 0){
            p1->p_pos.y = 0 + p1->radius;
            p1->p_vel.y = -(p1->p_vel.y);
        }
        if((p1->p_pos.z+p1->radius) > b.z){
            p1->p_pos.z = b.z - p1->radius;
            p1->p_vel.z = -(p1->p_vel.z);
        }
        else if((p1->p_pos.z-p1->radius) < 0){
            p1->p_pos.z = 0 + p1->radius;
            p1->p_vel.z = -(p1->p_vel.z);
        }
        for(Particles *p2 = p1->next; p2 != NULL; p2 = p2->next){
            double dist_x = p2->p_pos.x - p1->p_pos.x;
            double dist_y = p2->p_pos.y - p1->p_pos.y;
            double dist_z = p2->p_pos.z - p1->p_pos.z;
            double sqr_dist = dist_x*dist_x + dist_y*dist_y + dist_z*dist_z;
            double sum_radius = p1->radius + p2->radius;
            if(sqr_dist < sum_radius*sum_radius){
                double dist = sqrt(sqr_dist);
                if(dist < 0.0001){
                    dist_x = 1.0;
                    dist_y = 0.0;
                    dist_z = 0.0;
                    dist = 1.0;
                }
                double overlap = sum_radius - dist;
                double norm_x = dist_x/dist;
                double norm_y = dist_y/dist;
                double norm_z = dist_z/dist;
                double total_mass = p1->mass + p2->mass;
                double ratio1 = p2->mass / total_mass;
                double ratio2 = p1->mass / total_mass;
                /*
                p1->p_pos.x -= norm_x * (overlap * 0.5);
                p1->p_pos.y -= norm_y * (overlap * 0.5);
                p1->p_pos.z -= norm_z * (overlap * 0.5);
                p2->p_pos.x += norm_x * (overlap * 0.5);
                p2->p_pos.y += norm_y * (overlap * 0.5);
                p2->p_pos.z += norm_z * (overlap * 0.5);
                */
                p1->p_pos.x -= norm_x * (overlap * ratio1);
                p1->p_pos.y -= norm_y * (overlap * ratio1);
                p1->p_pos.z -= norm_z * (overlap * ratio1);
                p2->p_pos.x += norm_x * (overlap * ratio2);
                p2->p_pos.y += norm_y * (overlap * ratio2);
                p2->p_pos.z += norm_z * (overlap * ratio2);
                // Update velocities
                resolve_collision(p1,p2, norm_x, norm_y, norm_z);
            }
        }
    }
}

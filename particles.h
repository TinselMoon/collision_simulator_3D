#include "config.h"

typedef struct particle{
    Vector3 p_pos;
    Vector3 p_vel;
    Color color;
    int radius;
    double mass;
    struct particle *next;
} Particles;

typedef struct{
    Particles *head;
    Particles *tail;
} System;

typedef struct {
    float x;
    float y;
    float z;
} Box;

System* create();
void insert_particle(System *s, Box b);
void destroy (System *l);
void draw_particles(System *s);
void update_pos(System *s);
void resolve_collision(Particles *p1, Particles *p2);
void remove_particle(System *s);
bool is_empty(System *s);
void fix_contacts(System *s, Box b);

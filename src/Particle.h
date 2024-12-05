#include <raylib.h>
#ifndef PARTICLE_H
#define PARTICLE_H

typedef struct
{
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    double density;
    double mass;
    double pressure; 
} Particle;

Particle InitParticle(Vector3 position, Vector3 velocity, Vector3 acceleration, double mass, double fluid_density, double pressure_strength);

#endif
#include <raylib.h>
#include "Particle.h"
#ifndef SPH_H
#define SPH_H


const double dt = 1.E-10;

Vector3 computeForce(Particle *p, 
                     Particle *particles, 
                     int n, 
                     double h, 
                     double fluid_density,
                     double fluid_viscosity,
                     double pressure_strength,
                     double surface_tension
                    );

void update_particle(Particle *particle,
                     Particle *particles,
                     int n,
                     int h,
                     double fluid_density,
                     double fluid_viscosity,
                     double pressure_strength,
                     double surface_tension
                    );

#endif
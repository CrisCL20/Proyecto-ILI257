#include "Particle.h"

Particle InitParticle(Vector3 position, Vector3 velocity, Vector3 acceleration, double mass, double fluid_density, double pressure_strength){
    Particle particle;
    particle.position = position;
    particle.velocity = velocity;
    particle.acceleration = acceleration;
    particle.mass = mass;
    particle.density = 1;
    particle.pressure = pressure_strength * (fluid_density - particle.density);
}

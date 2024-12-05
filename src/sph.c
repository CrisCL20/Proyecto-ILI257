#include "sph.h"
#include "Particle.h"

#include <raylib.h>
#include <raymath.h>
#include <math.h>

double Wpoly6(double r, int h) {
    return 315 / (64 * PI * pow(h,9)) * pow(h*h - r*r,3.);    
}

double Wspiky(double r, double h) {
    return 15 / (PI * pow(h,6)) * pow(h-r,3);
}

Vector3 GradWspiky(Vector3 rvec, double r, int h) {
    double s = -45 / (2 * PI * pow(h,6)) * pow(h-r,2) / r;
    return Vector3Scale(rvec,s);
}

double LaplacianWvisc(double r, int h) {
    return 45 / (PI * pow(h,6)) * (h - r);
}

Vector3 GradWpoly6(Vector3 rvec, double r, int h) {
    double s = 315 / (64 * PI * pow(h,9)) * -6 * pow(h*h - r*r,2);
    return  Vector3Scale(rvec, s);
}

double LaplacianWpoly6(double r, int h) {
    return 315 / (64 * PI * pow(h,9)) * -6 * (3 * pow(h,4) - 10 * pow(h*r,2) + 7*pow(r,4));
}



Vector3 computeForce(Particle *p, 
                     Particle *particles, 
                     int n, 
                     int h, 
                     double fluid_density,
                     double fluid_viscosity,
                     double pressure_strength,
                     double surface_tension
                    ) 
{

    // fuerza de gravedad
    Vector3 force_gravity = (Vector3) {0.f, -9.8f * p->mass, 0.f};

    // fuerza de la presión 
    Vector3 force_pressure = (Vector3) {0.f,0.f,0.f};

    // fuerza de la viscocidad
    Vector3 force_viscosity = (Vector3) {0.,0.,0.};

    // tension superficial
    Vector3 force_super = (Vector3) {0,0,0};

    Vector3 grad_cp = (Vector3) {0,0,0};
    double lapl_cp = 0;

    
    for(int j = 0; j < n; ++j) {
        if(particles + j == p) continue;
        Vector3  r_ij = Vector3Subtract(p->position, particles[j].position);
        double r = Vector3Length(r_ij);

        
        if (r < h && r > 0) {
            // fuerza por presión sobre la particula p
            Vector3 grad_spiky = GradWspiky(r_ij,r,h);
            Vector3 mass_spiky = Vector3Scale(grad_spiky, p -> mass * particles[j].mass * (p->pressure + particles[j].pressure));
            force_pressure = Vector3Add(force_pressure, mass_spiky);

            // fuerza por viscosidad
            double lapl_visc = LaplacianWvisc(r,h);
            Vector3 vel_diff = Vector3Subtract(particles[j].velocity, p->velocity);
            vel_diff = Vector3Scale(vel_diff, particles[j].mass * lapl_visc);
            force_viscosity = Vector3Add(force_viscosity,vel_diff);

            //coeficientes de la tensión superficial
            Vector3 mass_and_gradient = Vector3Scale(GradWpoly6(r_ij,r,h), particles[j].mass);
            grad_cp = Vector3Add(grad_cp,mass_and_gradient);
            lapl_cp += particles[j].mass * LaplacianWpoly6(r,h);

        }

    }

    force_pressure = Vector3Scale(force_pressure,-1);
    force_viscosity = Vector3Scale(force_viscosity, fluid_viscosity);
    
    Vector3 gradcp_norm = Vector3Normalize(grad_cp);
    force_super = Vector3Scale(gradcp_norm, -surface_tension * lapl_cp);

    Vector3 tmp = Vector3Add(force_pressure, force_viscosity);
    return Vector3Add(tmp, force_super);
}

void update_particle(Particle *particle,
                     Particle *particles,
                     int n,
                     int h,
                     double fluid_density,
                     double fluid_viscosity,
                     double pressure_strength,
                     double surface_tension
                    )
{
    Vector3 force = computeForce(particle,particles,n,h,fluid_density, fluid_viscosity,pressure_strength, surface_tension);
    Vector3 acc = Vector3Scale(force, dt/particle->mass);

    Vector3 vel_prev = particle->velocity;
    particle->velocity = Vector3Add(vel_prev,acc);

    vel_prev = Vector3Scale(vel_prev, dt);
    particle->position = Vector3Add(particle->position, vel_prev);
}

#include <raylib.h>
#include <pthread.h>
#include <stdlib.h>
#include "Particle.h"
#include "sph.h"

#define NPARTICLES 100

int main(void) {

    InitWindow(1440,720,"Fluid Dynamic");
    if (!IsWindowReady()) exit(0);
    
    Camera camera = {0};
    camera.position = (Vector3) {0,10,10.f};
    camera.target = (Vector3) {0,0,0};
    camera.up = (Vector3) {0.f,1.f,0.f};
    camera.projection = CAMERA_PERSPECTIVE;
    camera.fovy = 20.f;

    int cameraMode = CAMERA_THIRD_PERSON;

    Particle particles[NPARTICLES];

    for(int i = 0; i < NPARTICLES; ++i) {
        InitParticle((Vector3) {GetRandomValue(-5,5), GetRandomValue(-5,5), GetRandomValue(-5,5)},
                     (Vector3) {0,0,0},
                     (Vector3) {0,0,0},
                     1.,
                     1,
                     1
                    );
    }

    SetTargetFPS(60);
    DisableCursor();

    while (!WindowShouldClose()) {
        UpdateCamera(&camera,cameraMode);
        
        for(int i = 0; i < NPARTICLES; ++i) {
            // ver como calcular el espaciado h
        }
        
        BeginDrawing();
        ClearBackground((Color) {0,0,0});
        BeginMode3D(camera);
            /*
            Dibujar las particulas
            */
        
        EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return(0);
}

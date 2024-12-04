#include <raylib.h>
#include <pthread.h>
#include <stdlib.h>

int main(void) {
    InitWindow(1440,720,"Fluid Dynamic");
    if (!IsWindowReady()) exit(0);
    
    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color) {0,0,0});

        EndDrawing();
    }

    CloseWindow();
}

#include "raylib.h"

// Window Settings

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

// HUD Settings
#define TEXT_SIZE 10

// Simuation Settings
static int N = 10000;

void renderHUD(void) {
    const char* strFPS = TextFormat("FPS: %d", GetFPS());
    const char* strN = TextFormat("N: %d", N);
    DrawText(strFPS, 0, 0, TEXT_SIZE, WHITE);
    DrawText(strN, 0, TEXT_SIZE, TEXT_SIZE, WHITE); 
}

int main(void) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Gravitational N-Body Simulator");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);


        // Rendering
        renderHUD();

        EndDrawing();

    }

    CloseWindow();  

    return 0;
} 
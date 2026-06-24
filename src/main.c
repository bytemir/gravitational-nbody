#include "raylib.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Window Settings

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define TARGET_FPS 60

// HUD Settings
#define TEXT_SIZE 10

// Simulator Settings
#define PARTICLE_MASS 1000.0f
#define GRAVITATIONAL_CONSTANT 1.5f
#define SOFTENING_FACTOR 100.0f
#define PARTICLES_PER_CLICK 125
typedef struct {
    Vector2 Position;
    Vector2 Velocity;
    int Mass;
} Particle;


void RenderHUD(int n) {
    const char* strFPS = TextFormat("FPS: %d", GetFPS());
    const char* strN = TextFormat("N: %d", n);
    DrawText(strFPS, 0, 0, TEXT_SIZE, WHITE);
    DrawText(strN, 0, TEXT_SIZE, TEXT_SIZE, WHITE); 
}

void UpdateParticles(Particle* particles, int particleCount, float dt) {
    for (int i = 0; i < particleCount; i++) {
        Vector2 total_acceleration = { 0.0f, 0.0f };

        for (int j = 0; j < particleCount; j++) {
            if (i == j) continue;

            float dx = particles[j].Position.x - particles[i].Position.x;
            float dy = particles[j].Position.y - particles[i].Position.y;
            
            float distance2 = (dx * dx) + (dy * dy) + SOFTENING_FACTOR;
            float distance = sqrt(distance2);
            float distance3 = distance2 * distance;

            float magnitude = (GRAVITATIONAL_CONSTANT * particles[j].Mass) / distance3;

            total_acceleration.x += magnitude * dx;
            total_acceleration.y += magnitude * dy;
        }

        particles[i].Velocity.x += total_acceleration.x * dt;
        particles[i].Velocity.y += total_acceleration.y * dt;
    }

    for (int i = 0; i < particleCount; i++) {
        particles[i].Position.x += particles[i].Velocity.x * dt;
        particles[i].Position.y += particles[i].Velocity.y * dt;
        
        float speed = sqrtf((particles[i].Velocity.x * particles[i].Velocity.x) + 
                            (particles[i].Velocity.y * particles[i].Velocity.y));

        float t = speed / (speed + 50.0f);

        Color particleColor;
        particleColor.r = (unsigned char)(t * 255);
        particleColor.g = (unsigned char)((1.0f - t) * 200);
        particleColor.b = 255;
        particleColor.a = 120;

        DrawCircleV(particles[i].Position, 0.8f, particleColor);
    }
}


int main(void) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Gravitational N-Body Simulator");
    SetTargetFPS(TARGET_FPS);

    int particleCapacity = 1000;
    int particleCount = 0;

    Particle* particles = malloc(particleCapacity * sizeof(Particle));
    if (particles == NULL) {printf("Failed to allocate dynamic memory to particles."); return 1;}
    
    while (!WindowShouldClose()) {
        float dt = GetFrameTime(); 
        Vector2 MousePosition = GetMousePosition();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            for (int i = 0; i < PARTICLES_PER_CLICK; i++ ) {   
                if (particleCount >= particleCapacity) {
                    particleCapacity += 1000;
                    Particle* temp = realloc(particles, particleCapacity * sizeof(Particle));
                    
                    if (temp == NULL) { free(particles); return 1; }
                    particles = temp;
                }
                float offsetX = GetRandomValue(-10, 10);
                float offsetY = GetRandomValue(-10, 10);

                particles[particleCount] = (Particle) {
                    .Position= (Vector2) {MousePosition.x + offsetX, MousePosition.y + offsetY},
                    .Velocity = (Vector2) { 0.0f, 0.0f },
                    .Mass=PARTICLE_MASS
                };

                particleCount++;
            }
        }
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        // Rendering
        RenderHUD(particleCount);
        
        BeginBlendMode(BLEND_ADDITIVE);
        UpdateParticles(particles, particleCount, dt);
        EndBlendMode();
        
        EndDrawing();

    }

    CloseWindow();
    
    free(particles);

    return 0;
} 
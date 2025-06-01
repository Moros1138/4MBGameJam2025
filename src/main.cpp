#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_ITEMS 10
#define WORLD_SIZE 100.0f

typedef struct {
    Vector3 position;
    bool collected;
} Item;

typedef struct {
    Vector3 position;
    bool active;
} Wormhole;

Wormhole wormhole;
Item items[MAX_ITEMS];

void GenerateWorld()
{
    // Initialize items
    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].position = Vector3{
            (float)(rand() % (int)WORLD_SIZE - WORLD_SIZE/2),
            (float)(rand() % (int)WORLD_SIZE - WORLD_SIZE/2),
            (float)(rand() % (int)WORLD_SIZE - WORLD_SIZE/2)
        };
        items[i].collected = false;
    }
    
    // Initialize wormhole
    wormhole = { .position = Vector3{0.0f, 0.0f, 0.0f}, .active = false };
}

void Reset(Camera* camera)
{
    camera->position = Vector3{ 0.0f, 1.0f, 0.0f };
    camera->target = Vector3{ 0.0f, 1.0f, 1.0f };
    camera->up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera->fovy = 45.0f;
    camera->projection = CAMERA_PERSPECTIVE;
    GenerateWorld();
}

int main(void) {
    // Initialize window and camera
    InitWindow(800, 600, "Item Collection Game");

    SetTargetFPS(60);

    // Camera setup for first-person view
    Camera3D camera = { 0 };
    Reset(&camera);

    // Initialize random seed
    srand((unsigned int)time(NULL));

    // Mouse control setup
    SetMousePosition(GetScreenWidth()/2, GetScreenHeight()/2);
    HideCursor();
    DisableCursor();

    bool gameWon = false;
    float moveSpeed = 0.5f;
    const float scale = 2.0f;

    while (!WindowShouldClose()) {
        // Update camera rotation based on mouse movement
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        // Normalize camera direction
        Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));

        // Movement controls
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
            camera.position = Vector3Add(camera.position, Vector3Scale(forward, moveSpeed));
            camera.target = Vector3Add(camera.position, forward);
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
            camera.position = Vector3Subtract(camera.position, Vector3Scale(forward, moveSpeed));
            camera.target = Vector3Add(camera.position, forward);
        }

        // Check for item collection
        for (int i = 0; i < MAX_ITEMS; i++) {
            if (!items[i].collected && Vector3Distance(camera.position, items[i].position) < 1.0f) {
                items[i].collected = true;
            }
        }

        // Check if all items are collected
        bool allCollected = true;
        for (int i = 0; i < MAX_ITEMS; i++) {
            if (!items[i].collected) {
                allCollected = false;
                break;
            }
        }

        // Activate wormhole when all items are collected
        if (allCollected && !wormhole.active) {
            wormhole.position = Vector3{
                (float)(rand() % (int)WORLD_SIZE - WORLD_SIZE/2),
                (float)(rand() % (int)WORLD_SIZE/2),
                (float)(rand() % (int)WORLD_SIZE - WORLD_SIZE/2)
            };
            wormhole.active = true;
        }

        // Check for wormhole collision
        if (wormhole.active && Vector3Distance(camera.position, wormhole.position) < 2.0f) {
            Reset(&camera);
        }

        // Render
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);
        // Draw ground plane
        DrawPlane(Vector3{0.0f, -WORLD_SIZE/2, 0.0f}, Vector2{WORLD_SIZE, WORLD_SIZE}, DARKGRAY);

        // Draw items
        for (int i = 0; i < MAX_ITEMS; i++) {
            if (!items[i].collected) {
                DrawCube(items[i].position, 0.5f * scale, 0.5f * scale, 0.5f * scale, RED);
            }
        }

        // Draw wormhole
        if (wormhole.active) {
            DrawSphere(wormhole.position, 1.0f * scale, PURPLE);
        }

        EndMode3D();

        // Draw UI
        DrawText("Collect all red cubes!", 10, 10, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
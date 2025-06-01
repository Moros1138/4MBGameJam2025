#include "raylib.h"

int main()
{
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;
    
    InitWindow(screenWidth, screenHeight, "Code with Moros | 4MB Game Jame 2025");
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(PINK);
            DrawText("Hello,World", 10, 10, 20, PURPLE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
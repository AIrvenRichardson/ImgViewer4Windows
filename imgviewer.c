#include "include/raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"

int main(int argc, char *argv[])
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    int screenWidth = 1000;
    int screenHeight = 500;
    InitWindow(screenWidth, screenHeight, "I HATE WINDOWS!!!!");
    SetTargetFPS(60);

    Image image = { 0 };

    if (argc > 1)
    {
        image = LoadImage(argv[1]);
    }
    

    Texture2D drawTarget = { 0 };
    bool newImage = true;
    float imageScale = 0.5f;
    float xoffset = 0.0f;
    float yoffset = 0.0f;

    while (!WindowShouldClose())
    {
        if (IsWindowResized() && !IsWindowFullscreen())
        {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
        } 

        if (IsImageValid(image) && newImage)
        {
            drawTarget = LoadTextureFromImage(image);
            newImage = false;    
        }

        imageScale += (float)GetMouseWheelMove()*0.05f;   // Image scale control
        if (imageScale <= 0.1f) imageScale = 0.1f;
        else if (imageScale >= 5) imageScale = 5;

        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
        {
            Vector2 md = GetMouseDelta();
            xoffset += md.x;
            yoffset += md.y;
        }

        //Draw Phase
        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            if (drawTarget.id > 0) 
            {
                DrawTextureEx(drawTarget, (Vector2){ screenWidth/2 - (float)drawTarget.width*imageScale/2 + xoffset , screenHeight/2 - (float)drawTarget.height*imageScale/2 + yoffset}, 0.0f, imageScale, WHITE);
            }
            else
            {
                DrawText("No Image/Image was invalid...", screenWidth/2 - 160, screenHeight/2, 20, BLACK);
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

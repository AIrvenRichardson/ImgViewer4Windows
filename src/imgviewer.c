#include "include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"

int minimum(int a, int b)
{
    return (a < b) ? a : b;
}

int main(int argc, char *argv[])
{
    //Configure the window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    int screenWidth = 1000;
    int screenHeight = 500;
    InitWindow(screenWidth, screenHeight, "I HATE WINDOWS!!!!");
    SetTargetFPS(60);

    //Make room for an image
    Image image = { 0 };

    if (argc > 1) image = LoadImage(argv[1]); 

    //A ton of variables for the image
    Texture2D drawTarget = { 0 };
    float imageScale = 1.0f;
    float xoffset = 0.0f;
    float yoffset = 0.0f;

    //Animation Controls
    unsigned int nextFrameDataOffset = 0;
    int animFrames = 0;
    int currentAnimFrame = 0;
    int frameDelay = 3;
    int frameCounter = 0;
    
    if (image.format == 7) image = LoadImageAnim(argv[1], &animFrames); //Reload image as an animation if it's a gif (7)

    if (IsImageValid(image))
    {
        drawTarget = LoadTextureFromImage(image);
        SetWindowSize(minimum(image.width, GetMonitorWidth(0)), minimum(image.height,GetMonitorHeight(0)));
    }

    while (!WindowShouldClose())
    {
        //UPDATE
        if (image.format == 7)
        {
            frameCounter++;
            if (frameCounter >= frameDelay)
            {
                currentAnimFrame++;
                if (currentAnimFrame >= animFrames) currentAnimFrame = 0;
                nextFrameDataOffset = image.width*image.height*4*currentAnimFrame;
                UpdateTexture(drawTarget, ((unsigned char *)image.data) + nextFrameDataOffset);
                frameCounter = 0;
            }
        }

        if (IsWindowResized() && !IsWindowFullscreen())
        {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
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

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
    char* errmsg = "";

    if (argc > 1) image = LoadImage(argv[1]); 

    //A ton of variables for the image
    Texture2D drawTarget = { 0 };
    float imageScale = 1.0f;
    float lastScale = 0.0f;
    float xoffset = 0.0f;
    float yoffset = 0.0f;

    //Animation Controls
    unsigned int nextFrameDataOffset = 0;
    int animFrames = 0;
    int currentAnimFrame = 0;
    int frameDelay = 5;
    int frameCounter = 0;

    if (image.format == 7)
    {
        if (GetFileLength(argv[1]) > 10000000){
            errmsg = "This file is too large! (>10mb)";   
        }
        else
        {
            image = LoadImageAnim(argv[1], &animFrames); //Reload image as an animation if it's a gif (7)
        }
    }

    if (IsImageValid(image))
    {
        drawTarget = LoadTextureFromImage(image);
        SetWindowSize(minimum(image.width+50, GetMonitorWidth(0)-50), minimum(image.height+25,GetMonitorHeight(0)-25));
    }

    while (!WindowShouldClose())
    {
        //UPDATE
        lastScale = imageScale;

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

        if (imageScale - lastScale > 0)
        {
            float xrat = screenWidth/2 - GetMouseX();
            float yrat = screenHeight/2 - GetMouseY();
            xoffset += (imageScale - lastScale) * xrat;
            yoffset += (imageScale - lastScale) * yrat;
        }
        else if (imageScale - lastScale < 0)
        {
            float xrat = -1*screenWidth/2 + GetMouseX();
            float yrat = -1*screenHeight/2 + GetMouseY();
            xoffset -= (imageScale - lastScale) * xrat;
            yoffset -= (imageScale - lastScale) * yrat;           
        }

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
                DrawText(errmsg, screenWidth/2 - 160, screenHeight/2, 20, BLACK); 
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

#include "ui.h"

int main() {
    InitWindow(500, 800, "RPN Calculator");
    SetTargetFPS(60);
    setFont();
 

    while (!WindowShouldClose()) {
        BeginDrawing();
        
        ClearBackground(BLACK);
        DrawTextEx(myFont, "RPN Calculator", {130, 5}, 40, 2, WHITE);
        keyBoardEvent();
        DrawRectangleRec(display, LIGHTGRAY);
        DrawRectangleRec(displayRes, LIGHTGRAY);
        DrawRectangleLinesEx(display, 2, DARKGRAY);
        DrawRectangleLinesEx(displayRes, 2, DARKGRAY);
        drawScrollableText(displayFont, inputText, display, 30, BLACK);
        drawButtons(inputText);

        displayResult(result);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

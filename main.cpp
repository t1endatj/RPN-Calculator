#include "ui.h"

int main() {
    InitWindow(500, 800, "RPN Calculator");
    SetTargetFPS(60);
    setFont();
 

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextEx(myFont, "RPN Calculator", {130, 5}, 40, 2, WHITE);
        deleteKey();
        DrawRectangleRec(display, LIGHTGRAY);
        DrawRectangleLinesEx(display, 2, DARKGRAY);
        DrawScrollableText(displayFont, inputText, display, 30, BLACK);
        DrawButtons(inputText);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

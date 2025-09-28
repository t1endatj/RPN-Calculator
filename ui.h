#pragma once
#include "raylib.h"
#include <iostream>
#include <string>
using namespace std;


// =========== Variables ===========

Font myFont;
Font labelFont;
Font displayFont;

string inputText = "";

Rectangle display = {25, 50, 450, 50};

const int buttonWidth = 80;
const int buttonHeight = 60;
const int startX = 40;
const int startY = 150;
const int gap = 10;

const char* labels[5][5] = {
    {"Pow", "Sqrt", "Inv", "Neg", "Abs"},
    {"7", "8", "9", "DEL", "AC"},
    {"4", "5", "6", "x", "/"},
    {"1", "2", "3", "+", "-"},
    {"0", ".", "Space", "%", "="}
};



// =========== UI/UX Functions ===========
void setFont() {
    myFont = LoadFontEx("./Poppins-Medium.otf", 40, 0, 0);
    labelFont = LoadFontEx("./Poppins-Medium.otf", 25, 0, 0);
    displayFont = LoadFontEx("./Poppins-Medium.otf", 30, 0, 0);
    SetTextureFilter(myFont.texture, TEXTURE_FILTER_POINT);
}

// Xoá dữ liệu đầu vào 
void deleteInput(string &inputText){
    if (inputText.size() >= 3 && inputText.substr(inputText.size() - 3) == "Abs") inputText.erase(inputText.size() - 3);
    else if (inputText.size() >= 3 && inputText.substr(inputText.size() - 3) == "Inv") inputText.erase(inputText.size() - 3);
    else if (inputText.size() >= 4 && inputText.substr(inputText.size() - 4) == "Sqrt") inputText.erase(inputText.size() - 4);
    else if (inputText.size() >= 3 && inputText.substr(inputText.size() - 3) == "Pow") inputText.erase(inputText.size() - 3);
    else if (inputText.size() >= 3 && inputText.substr(inputText.size() - 3) == "Neg") inputText.erase(inputText.size() - 3);
    else inputText.pop_back(); 
}

// Tương tác bằng bàn phím
void keyBoardEvent() {
    int key = GetCharPressed();
    if (key > 0 && key < 256) inputText += (char)key;
    if (IsKeyPressed(KEY_BACKSPACE) && !inputText.empty()) deleteInput(inputText);
}



// Hàm di chuyển text khi vượt ngoài ô hiển thị
void DrawScrollableText(Font font, const string &text, Rectangle box, int fontSize, Color color) {
    Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 2);
    float offset = 0;
    if (textSize.x > box.width - 10) offset = textSize.x - (box.width - 10);

    Vector2 pos = {box.x + 5 - offset, box.y + (box.height - fontSize)/2.0f};

    BeginScissorMode((int)box.x, (int)box.y, (int)box.width, (int)box.height);
    DrawTextEx(font, text.c_str(), pos, fontSize, 2, color);
    EndScissorMode();
}

// Hàm vẽ các nút máy tính
void DrawButtons(string &inputText) {
    Vector2 mousePos = GetMousePosition();

    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            string label = labels[row][col];
            if (label.empty()) continue;

            int x = startX + col * (buttonWidth + gap) - 10;
            int y = startY + row * (buttonHeight + gap) + 280;

            DrawRectangle(x, y, buttonWidth, buttonHeight, {238, 238, 238, 255});
            DrawRectangleLines(x, y, buttonWidth, buttonHeight, DARKGRAY);

            Vector2 textWidth = MeasureTextEx(labelFont, label.c_str(), 25.0f, 0.0f);
            DrawTextEx(
                labelFont,
                label.c_str(),
                {(float)x + (buttonWidth - textWidth.x) / 2,
                 (float)y + (buttonHeight - textWidth.y) / 2},
                25,
                0,
                {0, 0, 0, 255});

            Rectangle buttonRect = {(float)x, (float)y, (float)buttonWidth, (float)buttonHeight};
            
            // Tương tác bằng sự kiện click chuột
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(mousePos, buttonRect)) {
                if (label == "AC") inputText = "";
                else if(label == "DEL" && !inputText.empty()) deleteInput(inputText);
                else if(label == "Space") inputText += " ";
                else if(label == "=" || IsKeyPressed(KEY_ENTER)){
                    // stack.h
                }  
                else if(label != "DEL") inputText += label;
            }
        }
    }
}

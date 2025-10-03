#pragma once
#include "raylib.h"
#include "rpn.h"
#include "algorithms.h"
#include <iostream>
#include <string>
using namespace std;


// ============================================================================
// ==================== CONSTANTS & CONFIGURATION ===========================
// ============================================================================

const int buttonWidth = 80;
const int buttonHeight = 60;
const int startX = 40;
const int startY = 400; 
const int gap = 12;
const int maxHistory = 10;

const char* labels[5][5] = {
    {"Pow", "Sqrt", "Inv", "Neg", "Abs"},
    {"7", "8", "9", "DEL", "AC"},
    {"4", "5", "6", "*", "/"},
    {"1", "2", "3", "+", "-"},
    {"0", ".", "Space", "Ans", "="}
};


// ============================================================================
// ========================== GLOBAL VARIABLES ===============================
// ============================================================================

// Font Resources
Font myFont;
Font labelFont;
Font displayFont;
Font miniFont;
Font hFont;

// Application State
string inputText = "";
string result = "";
string searchText = "";
string ans = "";

// History Management
int historyCount = 0;
int filteredCount = 0;
bool isSearching = false;
bool searchMode = false;

// UI Layout Rectangles
Rectangle display = {25, 10, 450, 50};
Rectangle displayRes = {25, 70, 450, 50};
Rectangle displayH = {25, 160, 450, 257};
Rectangle searchBox = {25, 130, 260, 20};
Rectangle sortButton = {295, 130, 60, 20};
Rectangle clearButton = {365, 130, 50, 20};
Rectangle filterButton = {425, 130, 50, 20};

// Data Storage
History history[maxHistory];
History filteredHistory[maxHistory];

// ============================================================================
// ======================== UTILITY FUNCTIONS ================================
// ============================================================================


void setFont() {
    myFont = LoadFontEx("./Poppins-Medium.otf", 40, 0, 0);
    labelFont = LoadFontEx("./Poppins-Medium.otf", 25, 0, 0);
    displayFont = LoadFontEx("./Poppins-Medium.otf", 30, 0, 0);
    miniFont = LoadFontEx("./Poppins-Medium.otf", 14, 0, 0);
    hFont = LoadFontEx("./Poppins-Medium.otf", 12, 0, 0);
    SetTextureFilter(myFont.texture, TEXTURE_FILTER_POINT);
}

/**
 * Add calculation to history
 * @param expression - The mathematical expression
 * @param resultValue - The calculated result
 */
void addToHistory(const string& expression, double resultValue) {
    for (int i = maxHistory - 1; i > 0; i--) {
        history[i] = history[i - 1];
    }
    history[0] = History(expression, resultValue);
    if (historyCount < maxHistory) {
        historyCount++;
    }
}

/**
 * Delete last token from input string
 * Handles both single characters and function names
 * @param inputText - Reference to input string to modify
 */
void deleteToken(string &inputText) {
    if (inputText.size() >= 3 && inputText.substr(inputText.size() - 3) == "Abs") 
        inputText.erase(inputText.size() - 3);
    else if (inputText.size() >= 3 && inputText.substr(inputText.size() - 3) == "Inv") 
        inputText.erase(inputText.size() - 3);
    else if (inputText.size() >= 4 && inputText.substr(inputText.size() - 4) == "Sqrt") 
        inputText.erase(inputText.size() - 4);
    else if (inputText.size() >= 3 && inputText.substr(inputText.size() - 3) == "Pow") 
        inputText.erase(inputText.size() - 3);
    else if (inputText.size() >= 3 && inputText.substr(inputText.size() - 3) == "Neg") 
        inputText.erase(inputText.size() - 3);
    else if (inputText.size() >= 3 && inputText.substr(inputText.size() - 3) == "Ans") 
        inputText.erase(inputText.size() - 3);
    else if (!inputText.empty()) 
        inputText.pop_back(); 
}

// ============================================================================
// ======================= CALCULATION ENGINE ================================
// ============================================================================

/**
 * Main calculation function
 */
void Calculator() {
    try {
        string res = evalRPN(inputText);
        result = res;
        ans = result;
        
        try {
            double numResult = stod(res);
            addToHistory(inputText, numResult);
        } catch (const exception&) {
            // Ignore conversion errors for non-numeric results
        }
    } catch (const exception& e) {
        result = e.what();
    }
}

// ============================================================================
// ======================= INPUT HANDLING ====================================
// ============================================================================

/**
 * Handle keyboard input events
 * Manages both calculator input and search functionality
 */
void keyBoardEvent() {
    Vector2 mousePos = GetMousePosition();  
    
    // Check click on search box
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePos, searchBox)) {
            searchMode = true;
        } else {
            searchMode = false;
        }
    }
    
    // Handle character input
    int key = GetCharPressed();
    if (key > 0 && key < 256) {
        if (searchMode) {
            searchText += (char)key;
        } else {
            inputText += (char)key;
        }
    }
    
    // Handle backspace
    if (IsKeyPressed(KEY_BACKSPACE)) {
        if (searchMode && !searchText.empty()) {
            searchText.pop_back();
        } else if (!inputText.empty()) {
            deleteToken(inputText);
        }
    }
    
    // Handle escape key
    if (IsKeyPressed(KEY_ESCAPE)) {
        searchMode = false;
        isSearching = false;
    }
   
    // Handle enter key
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
        if (!searchMode && !inputText.empty()) {
            Calculator();
        }
    }
}

// ============================================================================
// ========================== UI RENDERING ===================================
// ============================================================================

/**
 * Draw scrollable text within a rectangle
 * Automatically scrolls text if it exceeds the box width
 */
void drawScrollableText(Font font, const string &text, Rectangle box, int fontSize, Color color) {
    Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 2);
    float offset = 0;
    
    if (textSize.x > box.width - 10) {
        offset = textSize.x - (box.width - 10);
    }

    Vector2 pos = {box.x + 5 - offset, box.y + (box.height - fontSize) / 2.0f};

    BeginScissorMode((int)box.x, (int)box.y, (int)box.width, (int)box.height);
    DrawTextEx(font, text.c_str(), pos, fontSize, 2, color);
    EndScissorMode();
}

/**
 * Display calculation result
 * @param inputText - The text to display
 */
void displayResult(string inputText) {
    drawScrollableText(displayFont, inputText, displayRes, 30, BLACK);
}

/**
 * Draw history panel with search and control buttons
 * Handles history display, sorting, filtering, and clearing
 */
void drawHistory() {
    // Draw search box
    DrawRectangleRec(searchBox, WHITE);
    drawScrollableText(miniFont, searchText, searchBox, 14, BLACK);
    
    // Draw control buttons
    DrawRectangleRec(sortButton, LIGHTGRAY);
    DrawTextEx(miniFont, "Sort", {313, 133}, 14, 1, BLACK);
    
    DrawRectangleRec(clearButton, LIGHTGRAY);
    DrawTextEx(miniFont, "Clear", {378, 133}, 14, 1, BLACK);
    
    DrawRectangleRec(filterButton, isSearching ? YELLOW : LIGHTGRAY);
    DrawTextEx(miniFont, "Filter", {438, 133}, 14, 1, BLACK);
    
    // Draw history background
    DrawRectangleRec(displayH, LIGHTGRAY);
    
    // Display history entries
    Vector2 mousePos = GetMousePosition();
    History* displayArray = isSearching ? filteredHistory : history;
    int displayCount = isSearching ? filteredCount : historyCount;
    
    int maxDisplay = 10; 
    for (int i = 0; i < displayCount && i < maxDisplay; i++) {
        float yPos = 172 + i * 25;
        string displayText = displayArray[i].expression + " = " + to_string(displayArray[i].result);
        
        // Truncate if too long
        if (displayText.length() > 45) {
            displayText = displayText.substr(0, 42) + "...";
        }

        // Hover effect
        Rectangle lineRect = {30, yPos - 5, 420, 20};
        
        if (CheckCollisionPointRec(mousePos, lineRect)) {
            DrawRectangleRec(lineRect, YELLOW);
        }
        
        DrawTextEx(hFont, displayText.c_str(), {35, yPos}, 12, 1, BLACK);
        
        // Click to copy expression to input
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
            CheckCollisionPointRec(mousePos, lineRect)) {
            inputText = displayArray[i].expression;
        }
    }
    
    // Handle control button clicks
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        
        // Sort button
        if (CheckCollisionPointRec(mousePos, sortButton)) {
            if (isSearching) {
                sortHistory(filteredHistory, filteredCount);
            } else {
                sortHistory(history, historyCount);
            }
        }
        
        // Clear button
        if (CheckCollisionPointRec(mousePos, clearButton)) {
            historyCount = 0;
            filteredCount = 0;
            searchText = "";
            isSearching = false;
        }
        
        // Filter button
        if (CheckCollisionPointRec(mousePos, filterButton)) {
            if (!searchText.empty()) {
                filteredCount = filterHistory(history, historyCount, filteredHistory, searchText);
                isSearching = true;
            } else {
                isSearching = false;
            }
        }
    }
}

/**
 * Draw calculator buttons and handle button interactions
 * @param inputText - Reference to input string to modify
 */
void drawButtons(string &inputText) {
    Vector2 mousePos = GetMousePosition();

    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            string label = labels[row][col];
            if (label.empty()) continue;

            int x = startX + col * (buttonWidth + gap) - 13;
            int y = startY + row * (buttonHeight + gap) + 30;

            // Draw button background and border
            DrawRectangle(x, y, buttonWidth, buttonHeight, {238, 238, 238, 255});
            DrawRectangleLines(x, y, buttonWidth, buttonHeight, DARKGRAY);

            // Center text 
            Vector2 textWidth = MeasureTextEx(labelFont, label.c_str(), 25.0f, 0.0f);
            DrawTextEx(
                labelFont,
                label.c_str(),
                {(float)x + (buttonWidth - textWidth.x) / 2,
                 (float)y + (buttonHeight - textWidth.y) / 2},
                25,
                0,
                {0, 0, 0, 255}
            );

            Rectangle buttonRect = {(float)x, (float)y, (float)buttonWidth, (float)buttonHeight};
            
            // Handle button clicks
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(mousePos, buttonRect)) {
                
                if (label == "AC") {
                    inputText = "";
                    result = "";
                }
                else if (label == "DEL" && !inputText.empty()) {
                    deleteToken(inputText);
                }
                else if (label == "Space") {
                    inputText += " ";
                }
                else if (label == "=") {
                    Calculator();   
                }
                else if (label == "Ans") {
                    inputText += ans;
                }
                else if (label != "DEL") {
                    inputText += label;
                }
            }
        }
    }
}

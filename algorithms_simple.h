#pragma once
#include <string>
using namespace std;

// Cấu trúc lưu trữ một phép tính
struct HistoryEntry {
    string expression;
    double result;
    
    HistoryEntry() : expression(""), result(0.0) {}
    HistoryEntry(const string& expr, double res) : expression(expr), result(res) {}
};

// Thuật toán sắp xếp theo kết quả tăng dần (Bubble Sort)
void sortHistoryByResult(HistoryEntry history[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (history[j].result > history[j + 1].result) {
                // Hoán đổi
                HistoryEntry temp = history[j];
                history[j] = history[j + 1];
                history[j + 1] = temp;
            }
        }
    }
}

// Thuật toán tìm kiếm chuỗi con (String Search)
bool containsSubstring(const string& text, const string& pattern) {
    if (pattern.empty()) return true;
    if (text.length() < pattern.length()) return false;
    
    // Thuật toán tìm kiếm chuỗi con đơn giản
    for (size_t i = 0; i <= text.length() - pattern.length(); i++) {
        bool found = true;
        for (size_t j = 0; j < pattern.length(); j++) {
            if (text[i + j] != pattern[j]) {
                found = false;
                break;
            }
        }
        if (found) return true;
    }
    return false;
}

// Lọc lịch sử theo chuỗi tìm kiếm
int filterHistory(const HistoryEntry source[], int sourceCount, 
                  HistoryEntry filtered[], const string& searchText) {
    int filteredCount = 0;
    
    for (int i = 0; i < sourceCount; i++) {
        // Tìm kiếm trong biểu thức hoặc kết quả (chuyển thành string)
        string resultStr = to_string(source[i].result);
        
        if (containsSubstring(source[i].expression, searchText) || 
            containsSubstring(resultStr, searchText)) {
            filtered[filteredCount] = source[i];
            filteredCount++;
        }
    }
    
    return filteredCount;
}
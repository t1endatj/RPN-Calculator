#pragma once
#include <string>
using namespace std;

// Struct lưu trữ một phép tính
struct History {
    string expression;
    double result;

    History() {
        expression = "";
        result = 0.0;
    }

    History(const string& expr, double res) {
        expression = expr;
        result = res;
    }
};


// Quick sort 
void quickSortNoP(History nums[], int l, int r) {
    double pivot = nums[(l + r) / 2].result; 
    int i = l;
    int j = r;

    while (i <= j) {
        while (nums[i].result < pivot) i++;
        while (nums[j].result > pivot) j--;
        if (i <= j) {
            swap(nums[i], nums[j]);  
            i++;
            j--;
        }
    }

    if (l < j) quickSortNoP(nums, l, j);
    if (i < r) quickSortNoP(nums, i, r);
}

void sortHistory(History history[], int count) {
    quickSortNoP(history, 0, count - 1);
}

// Thuật toán tìm kiếm chuỗi con (String Search)
#include <string>
using namespace std;

bool containsSubstring(const string& text, const string& pattern) {
    int n = text.size();
    int m = pattern.size();
    if (m == 0) return true;
    if (n < m) return false;

    // Bảng "bad character" mặc định cho ASCII (256 ký tự)
    int badChar[256];
    for (int i = 0; i < 256; i++) badChar[i] = -1;

    for (int i = 0; i < m; i++) {
        badChar[(unsigned char)pattern[i]] = i; 
    }

    int shift = 0; // vị trí bắt đầu trong text
    while (shift <= n - m) {
        int j = m - 1;

        // So sánh pattern từ phải sang trái
        while (j >= 0 && pattern[j] == text[shift + j]) {
            j--;
        }

        // Nếu j < 0 => khớp hoàn toàn
        if (j < 0) {
            return true;
        } else {
            // Dùng bad character rule để tính bước nhảy
            int skip = j - badChar[(unsigned char)text[shift + j]];
            if (skip < 1) skip = 1;
            shift += skip;
        }
    }
    return false;
}


// Lọc lịch sử theo chuỗi tìm kiếm
int filterHistory(const History source[], int sourceCount, History filtered[], const string& searchText) {
        int filteredCount = 0;    
        for (int i = 0; i < sourceCount; i++) {
            // Tìm kiếm trong biểu thức hoặc kết quả 
            string resultStr = to_string(source[i].result);
            if (containsSubstring(source[i].expression, searchText) || 
                containsSubstring(resultStr, searchText)) {
                filtered[filteredCount] = source[i];
                filteredCount++;
            }
        }
    return filteredCount;
}
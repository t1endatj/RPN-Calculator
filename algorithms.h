#pragma once
#include <string>
using namespace std;

// STRUCT LỊCH SỬ
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


// QUICK SORT
void quickSort(History nums[], int l, int r) {
    if (l >= r) return;
    double pivot = nums[(l + r) / 2].result;
    int i = l;
    int j = r;
    
    while (i <= j) {
        while (nums[i].result < pivot) i++;
        while (nums[j].result > pivot) j--;
        if (i <= j) {
            History temp = nums[i];
            nums[i] = nums[j];
            nums[j] = temp;
            i++;
            j--;
        }
    }
    
    
    if (l < j) quickSort(nums, l, j);
    if (i < r) quickSort(nums, i, r);
}

void sortHistory(History history[], int count) {
    if (history == nullptr || count <= 0) return;
    quickSort(history, 0, count - 1);
}



// THUẬT TOÁN TÌM KIẾM BOYAR MOORE
bool boyarMooreSearch(const string& text, const string& pattern) {
    int n = text.size();
    int m = pattern.size();
    if (m == 0) return true;  
    if (n < m) return false;  


    int L[256];
    for (int i = 0; i < 256; i++) L[i] = -1;
    for (int j = 0; j < m; j++) {
        L[(unsigned char)pattern[j]] = j;
    }

    int i = m - 1;
    int j = m - 1; 

    while (i < n) {
        if (text[i] == pattern[j]) {
            if (j == 0) {
                return true; 
            } else {
                i--;
                j--;
            }
        } else {
            int l = L[(unsigned char)text[i]];
            int skip = j < (1 + l) ? j : (1 + l); 
            i = i + m - skip;
            j = m - 1;
        }
    }
    return false;
}


// HÀM LỌC PHÉP TOÁN
int historyFilterd(const History source[], int sourceCount, History filtered[], const string& searchText) {
        int filteredCount = 0;    
        for (int i = 0; i < sourceCount && filteredCount < 30; i++) { 
            string resultStr = formatNum(source[i].result);
            if (boyarMooreSearch(source[i].expression, searchText) || 
                boyarMooreSearch(resultStr, searchText)) {
                filtered[filteredCount] = source[i];
                filteredCount++;
            }
        }
    return filteredCount;
}
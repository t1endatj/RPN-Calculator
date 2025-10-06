#pragma once
#include <string>
using namespace std;

// Struct to store a calculation
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


// Insertion sort
void insertionSort(History nums[], int n) {
    for (int i = 1; i < n; i++) {
        History key = nums[i]; 
        int j = i - 1;
        while (j >= 0 && nums[j].result > key.result) {
            nums[j + 1] = nums[j];
            j--;
        }
        nums[j + 1] = key;
    }
}

void sortHistory(History history[], int count) {
    if (history == nullptr || count <= 0) return;
    insertionSort(history, count);
}



// Boyar moore string search
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


// Filter history by search string
int filterHistory(const History source[], int sourceCount, History filtered[], const string& searchText) {
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
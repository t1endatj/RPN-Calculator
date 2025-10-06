#pragma once
#include <stdexcept>
#include <string>
using namespace std;

// ĐỊNH NGHĨA STACK
struct Stack {
    string values[200];
    int top = -1;

    void push(const string& v) {
        if (top >= 199) throw overflow_error("Stack Overflow");
        values[++top] = v;
    }

    string pop() {
        if (top < 0) throw underflow_error("Stack Underflow");
        return values[top--];
    }


    bool empty() const { return top < 0; }
    int size() const { return top + 1; }
};


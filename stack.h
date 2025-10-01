#pragma once
#include <stdexcept>
#include <optional>
#include <iostream>
using namespace std;

struct Stack {
    double values[200];
    int top = -1;

    void push(double v) {
        if (top >= 199) throw overflow_error("Stack overflow");
        values[++top] = v;
    }

    optional<double> pop() {
        if (top < 0) return {};
        return values[top--];
    }

    double peek() const {
        if (top < 0) throw underflow_error("Stack empty");
        return values[top];
    }

    bool empty() const { return top < 0; }
    int size() const { return top + 1; }
};

struct StackStr {
    string values[200];
    int top = -1;

    void push(const string& v) {
        if (top >= 199) throw overflow_error("StackStr overflow");
        values[++top] = v;
    }

    optional<string> pop() {
        if (top < 0) return {};
        return values[top--];
    }

    string peek() const {
        if (top < 0) throw underflow_error("StackStr empty");
        return values[top];
    }

    bool empty() const { return top < 0; }
    int size() const { return top + 1; }
};
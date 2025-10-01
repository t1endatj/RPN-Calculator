#pragma once
#include <stdexcept>
#include <string>
using namespace std;

double mysqrt(double x) {
    if (x < 0) throw invalid_argument("sqrt of negative");
    double guess = x > 1 ? x : 1;
    for (int i = 0; i < 50; i++) {
        guess = 0.5 * (guess + x / guess);
    }
    return guess;
}

double mypow(double a, double b) {
    if (b == (int)b) {
        double res = 1;
        int n = (int)b;
        bool neg = n < 0;
        if (neg) n = -n;
        for (int i = 0; i < n; i++) res *= a;
        if (neg) return 1.0 / res;
        return res;
    }
    throw invalid_argument("pow with non-integer exponent not supported");
}

double inv(double x) {
    if (x == 0) throw invalid_argument("Division by zero in inv()");
    return 1.0 / x;
}

double neg(double x) { return -x; }

double myabs(double x) { return x < 0 ? -x : x; }

double applyFunction(const string& func, double x, double y = 0) {
    if (func == "Sqrt") return mysqrt(x);
    if (func == "Pow") return mypow(x, y);
    if (func == "Inv") return inv(x);
    if (func == "Neg") return neg(x);
    if (func == "Abs") return myabs(x);
    throw invalid_argument("Unknown function: " + func);
}

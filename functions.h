#pragma once
#include <string>
#include <stdexcept>
using namespace std;

double myAbs(double v) { return v < 0.0 ? -v : v; }


const double LN2 = 0.693147180559945309417232121458176568;

double myLn(double v) {
    if (v <= 0.0) throw invalid_argument("Log of non-positive number");
    int n = 0;
    double x = v;
    while (x >= 2.0) { x *= 0.5; ++n; }
    while (x < 1.0) { x *= 2.0; --n; }
    double t = (x - 1.0) / (x + 1.0);
    double t2 = t * t;
    double term = t;
    double sum = term;
    for (int k = 3; ; k += 2) {
        term *= t2;
        double delta = term / (double)k;
        sum += delta;
        if (myAbs(delta) < 1e-16) break;
        if (k > 20001) break;
    }
    return 2.0 * sum + n * LN2;
}

double myExp(double x) {
    long long n = (long long)(x / LN2);
    double r = x - n * LN2;
    double term = 1.0;
    double sum = 1.0;
    for (int k = 1; ; ++k) {
        term *= r / (double)k;
        sum += term;
        if (myAbs(term) < 1e-16) break;
        if (k > 20000) break;
    }
    double pow2 = 1.0;
    if (n > 0) {
        for (long long i = 0; i < n; ++i) pow2 *= 2.0;
    } else if (n < 0) {
        for (long long i = 0; i < -n; ++i) pow2 *= 0.5;
    }
    return sum * pow2;
}

bool isInteger(double x) {
    double r = (double)((long long)(x));
    return myAbs(x - r) < 1e-12;
}

double myPow(double base, double exponent) {
    if (base == 0.0) {
        if (exponent > 0.0) return 0.0;
        throw invalid_argument("0 can't be raised to 0 or negative.");
    }
    if(base < 0.0 && exponent == 0.5){
        throw invalid_argument("A negative number can’t be squared");
    }
    if (isInteger(exponent)) {
        long long e = (long long)exponent;
        bool neg = e < 0;
        unsigned long long ue = (unsigned long long)(neg ? -e : e);
        double res = 1.0;
        double b = base;
        while (ue) {
            if (ue & 1ULL) res *= b;
            b *= b;
            ue >>= 1;
        }
        return neg ? (1.0 / res) : res;
    }

    double lnbase = myLn(base);
    return myExp(exponent * lnbase);
}

double applyFunction(const string& func, double x, double y = 0.0) {
    if (func == "Sqrt") return myPow(x, 0.5);
    if (func == "Pow")  return myPow(x, y);
    if (func == "Inv") {
        if (x == 0.0) throw invalid_argument("Division by zero");
        return 1.0 / x;
    }
    if (func == "Neg") return -x;
    if (func == "Abs") return myAbs(x);
    throw invalid_argument("Unknown function: " + func);
}

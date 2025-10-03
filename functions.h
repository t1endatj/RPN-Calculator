#pragma once
#include <string>
#include <stdexcept>
using namespace std;

double absd(double v) { return v < 0.0 ? -v : v; }

double msqrt(double x) {
    if (x < 0.0) throw invalid_argument("Sqrt of negative number");
    if (x == 0.0) return 0.0;
    double r = x < 1.0 ? 1.0 : x;
    for (int i = 0; i < 200; ++i) {
        double nr = 0.5 * (r + x / r);
        if (absd(nr - r) <= 1e-15) break;
        r = nr;
    }
    return r;
}

const double LN2 = 0.693147180559945309417232121458176568;

double mln(double v) {
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
        if (absd(delta) < 1e-16) break;
        if (k > 20001) break;
    }
    return 2.0 * sum + n * LN2;
}

double mexp(double x) {
    long long n = (long long)(x / LN2);
    double r = x - n * LN2;
    double term = 1.0;
    double sum = 1.0;
    for (int k = 1; ; ++k) {
        term *= r / (double)k;
        sum += term;
        if (absd(term) < 1e-16) break;
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

bool isIntegerReal(double x) {
    double r = (double)((long long)(x));
    return absd(x - r) < 1e-12;
}

double mpow(double base, double exponent) {
    if (base == 0.0) {
        if (exponent > 0.0) return 0.0;
        throw invalid_argument("0 cannot be raised to non-positive power");
    }
    if (base < 0.0 && !isIntegerReal(exponent))
        throw invalid_argument("Negative base with non-integer exponent not supported");

    if (isIntegerReal(exponent)) {
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

    double lnbase = mln(base);
    return mexp(exponent * lnbase);
}

double applyFunctionDouble(const string& func, double x, double y = 0.0) {
    if (func == "Sqrt") return msqrt(x);
    if (func == "Pow")  return mpow(x, y);
    if (func == "Inv") {
        if (x == 0.0) throw invalid_argument("Division by zero");
        return 1.0 / x;
    }
    if (func == "Neg") return -x;
    if (func == "Abs") return x < 0.0 ? -x : x;
    throw invalid_argument("Unknown function: " + func);
}

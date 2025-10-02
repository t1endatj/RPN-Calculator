#pragma once
#include <string>
#include <stdexcept>
#include "BigNum.h"
using namespace std;

inline string absBigNum(const string& s) {
    if (!s.empty() && s[0] == '-') return s.substr(1);
    return s;
}

inline string sqrtBigNum(const string& s, int prec = 20) {
    string num = normalize(s);
    if (num == "0") return "0";
    if (isNegative(num)) throw invalid_argument("Sqrt of negative number");

    int wp = max(8, prec + 8);

    auto [intPart, fracPart] = splitDecimal(num);
    intPart = removeLeadingZeros(intPart);
    int lenInt = (int)intPart.size();
    string x;
    if (compare(num, "1") < 0) {
        x = "1";
    } else {
        int e = (lenInt + 1) / 2;
        x = "1";
        x.append(e, '0');
    }

    string eps;
    if (prec <= 0) eps = "1";
    else {
        eps = "0.";
        for (int i = 1; i < prec; i++) eps.push_back('0');
        eps.push_back('1');
    }

    string prev;
    for (int iter = 0; iter < 500; iter++) {
        prev = x;
        string num_div_x = divLong(num, x, wp);
        string sum_x = sum(x, num_div_x);
        x = half(sum_x, wp);
        x = normalize(x);

        string delta = diff(x, prev);
        if (!delta.empty() && delta[0] == '-') delta = delta.substr(1);

        if (compare(delta, eps) <= 0) break;
    }

    return roundTo(x, prec);
}

inline string applyFunction(const string& func, const string& x, const string& y = "0") {
    if (func == "Sqrt")
        return sqrtBigNum(x, 20);

    if (func == "Pow") {
        int exp = 0;
        try { exp = stoi(y); } catch(...) { throw invalid_argument("Pow requires integer exponent"); }
        if (exp == 0) return "1";
        bool negExp = exp < 0;
        if (negExp) exp = -exp;
        string res = "1";
        for (int i = 0; i < exp; i++) res = multi(res, x);
        if (negExp) res = divisor("1", res, 20);
        return normalize(res);
    }

    if (func == "Inv")
        return divisor("1", x, 20);

    if (func == "Neg") {
        string nx = normalize(x);
        if (!nx.empty() && nx[0] == '-') return normalize(nx.substr(1));
        return string("-") + nx;
    }

    if (func == "Abs")
        return normalize(absBigNum(x));

    throw invalid_argument("Unknown function: " + func);
}

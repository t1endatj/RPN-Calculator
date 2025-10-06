#pragma once
#include <stdexcept>
#include <string>
#include "stack.h"
#include "functions.h"
using namespace std;

bool isFunction(const string& token) {
    return token == "Sqrt" || token == "Pow" || token == "Neg" || token == "Abs" || token == "Inv";
}

bool isDigit(char c) { return (c >= '0' && c <= '9') || c == '.'; }
bool isSpace(char c) { return c==' ' || c=='\t' || c=='\n' || c=='\r'; }
bool isAlpha(char c) { return (c>='a'&&c<='z')||(c>='A'&&c<='Z'); }

string formatNum(double v) {
    string s = to_string(v);
    size_t pos = s.find('.');
    if (pos == string::npos) return s;
    while (!s.empty() && s.back() == '0') s.pop_back();
    if (!s.empty() && s.back() == '.') s.pop_back();
    if (s == "-0") return "0";
    return s;
}

string evalRPN(const string& rpn) {
    Stack st;
    string token;
    
    for (size_t i=0; i<rpn.size();) {
        if (isSpace(rpn[i])) { ++i; continue; } 
        if (isDigit(rpn[i]) || (rpn[i] == '-' && i+1 < rpn.size() && isDigit(rpn[i+1]))) {
            token.clear();
            if (rpn[i] == '-') {
                token.push_back(rpn[i++]); 
            }
            while (i<rpn.size() && isDigit(rpn[i])) token.push_back(rpn[i++]);
            st.push(token);
            continue;
        }

        token.clear();
        if (isAlpha(rpn[i])) {
            while (i<rpn.size() && isAlpha(rpn[i])) token.push_back(rpn[i++]);
        } else token.push_back(rpn[i++]);

        
        if (token=="+" || token=="-" || token=="*" || token=="/") {
            if (st.size() < 2) throw invalid_argument("Insufficient operands for operator: " + token);
            double b = stod(st.pop()), a = stod(st.pop());
            double res = 0;
            if (token=="+") res=a+b;
            else if (token=="-") res=a-b;
            else if (token=="*") res=a*b;
            else if (token=="/") {
                if (b==0.0) throw invalid_argument("Division by zero");
                res=a/b;
            }
            st.push(formatNum(res));
        }
        else if (isFunction(token)) {
            if (token=="Pow") {
                double exp = stod(st.pop());
                double base = stod(st.pop());
                st.push(formatNum(applyFunction(token, base, exp)));
            } else {
                double a = stod(st.pop());
                st.push(formatNum(applyFunction(token, a)));
            }
        }
        else throw invalid_argument("Unknown operator or function: " + token);
    }

    if (st.empty()) throw runtime_error("No result on stack");
    string result = st.pop();
    if (!st.empty()) throw runtime_error("Extra operands on stack");
    return result;
}

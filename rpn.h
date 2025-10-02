#pragma once
#include <stdexcept>
#include <string>
#include "stack.h"
#include "BigNum.h"
#include "functions.h"

using namespace std;

bool isFunction(const string& token) {
    return token == "Sqrt" || token == "Pow" || token == "Neg" || token == "Abs" || token == "Inv";
}

bool isDigit(char c) { return c >= '0' && c <= '9'; }
bool isSpace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
bool isAlpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

string evalRPN(const string& rpn) {
    Stack st;
    string token;
    for (size_t i = 0; i < rpn.size();) {
        if (isSpace(rpn[i])) { ++i; continue; }

        if (isDigit(rpn[i]) || (rpn[i]=='-' && i+1<rpn.size() && (isDigit(rpn[i+1])||rpn[i+1]=='.'))) {
            token.clear(); token.push_back(rpn[i++]);
            while (i < rpn.size() && (isDigit(rpn[i]) || rpn[i]=='.')) token.push_back(rpn[i++]);
            st.push(normalize(token));
            continue;
        }

        token.clear();
        if (isAlpha(rpn[i])) { while (i<rpn.size() && isAlpha(rpn[i])) token.push_back(rpn[i++]); }
        else token.push_back(rpn[i++]);

        if (token == "+" || token == "-" || token == "*" || token == "/") {
            string b = st.pop(), a = st.pop();
            if (token=="+") st.push(sum(a,b));
            else if (token=="-") st.push(diff(a,b));
            else if (token=="*") st.push(multi(a,b));
            else st.push(divisor(a,b,20));
        } 
        else if (isFunction(token)) {
            if (token == "Pow") {
                string a = st.pop(); 
                string b = st.pop(); 
                st.push(applyFunction(token, b, a));
            } else {
                string a = st.pop();
                st.push(applyFunction(token, a));
            }
        } 
        else throw invalid_argument("Unknown operator or function: " + token);
    }

    if (st.empty()) throw runtime_error("No result on stack");
    string result = st.pop();
    if (!st.empty()) throw runtime_error("Extra operands on stack");
    return normalize(result);
}


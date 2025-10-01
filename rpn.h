#pragma once
#include <stdexcept>
#include <string>
#include <cctype>
#include "stack.h"
#include "functions.h"  

using namespace std;
int precedence(const string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "^") return 3;
    return 0;
}
bool isFunction(const string& token) {
    return token == "Sqrt" || token == "Pow" || token == "Inv" || token == "Neg" || token == "Abs";
}




double evalRPN(const string& rpn) {
    Stack st;
    string token;
    for (size_t i = 0; i < rpn.size();) {
        if (isspace(rpn[i])) { i++; continue; }

        if (isdigit(rpn[i]) || rpn[i] == '.' || 
            (rpn[i] == '-' && i + 1 < rpn.size() && isdigit(rpn[i+1]))) {
            token.clear();
            token.push_back(rpn[i++]);
            while (i < rpn.size() && (isdigit(rpn[i]) || rpn[i] == '.')) {
                token.push_back(rpn[i++]);
            }
            st.push(stod(token));
        }
        else {
            token.clear();
            if (isalpha(rpn[i])) {
                while (i < rpn.size() && isalpha(rpn[i])) token.push_back(rpn[i++]);
            } else {
                token.push_back(rpn[i++]);
            }

            if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") {
                auto b = st.pop(), a = st.pop();
                if (token == "+") st.push(a.value() + b.value());
                else if (token == "-") st.push(a.value() - b.value());
                else if (token == "*") st.push(a.value() * b.value());
                else if (token == "/") st.push(a.value() / b.value());
                else if (token == "^") st.push(applyFunction("Pow", a.value(), b.value()));
            } 
            else if (isFunction(token)) {
                if (token == "Pow") {
                    auto b = st.pop(), a = st.pop();
                    st.push(applyFunction("Pow", a.value(), b.value()));
                } else {
                    auto a = st.pop();
                    st.push(applyFunction(token, a.value()));
                }
            }
            else {
                throw invalid_argument("Unknown token: " + token);
            }
        }
    }
    if (st.size() != 1) throw invalid_argument("Invalid RPN expression");
    return st.pop().value();
}


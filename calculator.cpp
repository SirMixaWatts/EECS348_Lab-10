#include "calculator.hpp"
#include <string>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>

using namespace std;

// Helper: split number into sign, integer part, fractional part
void split_double(const string &num, char &sign, string &intPart, string &fracPart) {
    size_t i = 0;
    sign = '+';
    if (num[i] == '+' || num[i] == '-') {
        sign = num[i];
        i++;
    }

    size_t dot = num.find('.', i);
    if (dot == string::npos) {
        intPart = num.substr(i);
        fracPart = "";
    } else {
        intPart = num.substr(i, dot - i);
        fracPart = num.substr(dot + 1);
    }

    // Remove leading zeros from intPart
    intPart.erase(0, intPart.find_first_not_of('0'));
    if (intPart.empty()) intPart = "0";
}

// Helper: pad two strings to same length
void normalize(string &a, string &b, bool leftPad = true) {
    int diff = a.length() - b.length();
    if (diff > 0) {
        b = (leftPad ? string(diff, '0') + b : b + string(diff, '0'));
    } else if (diff < 0) {
        a = (leftPad ? string(-diff, '0') + a : a + string(-diff, '0'));
    }
}

// Helper: add two digit strings (no decimal point)
string add_parts(const string &a, const string &b, int &carry) {
    string result;
    carry = 0;
    for (int i = a.size() - 1; i >= 0; i--) {
        int sum = (a[i] - '0') + (b[i] - '0') + carry;
        carry = sum / 10;
        result += (sum % 10) + '0';
    }
    reverse(result.begin(), result.end());
    return result;
}

// Helper: subtract b from a (a > b guaranteed), return result, no negative
string subtract_parts(const string &a, const string &b, int &borrow) {
    string result;
    borrow = 0;
    for (int i = a.size() - 1; i >= 0; i--) {
        int diff = (a[i] - '0') - (b[i] - '0') - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result += diff + '0';
    }
    reverse(result.begin(), result.end());
    return result;
}

// Compare magnitudes (ignoring sign)
bool is_greater(const string &intA, const string &fracA, const string &intB, const string &fracB) {
    string a = intA + fracA;
    string b = intB + fracB;
    normalize(a, b);
    return a > b;
}

string add_doubles_as_strings(const string &a, const string &b) {
    char signA, signB;
    string intA, fracA, intB, fracB;

    split_double(a, signA, intA, fracA);
    split_double(b, signB, intB, fracB);

    normalize(fracA, fracB, false);
    normalize(intA, intB, true);

    string resultInt, resultFrac;
    string finalResult;
    int carry = 0, borrow = 0;

    if (signA == signB) {
        // Same sign = addition
        resultFrac = add_parts(fracA, fracB, carry);
        resultInt = add_parts(intA, intB, carry);
        if (carry) resultInt = "1" + resultInt;
        finalResult = resultInt;
        if (!fracA.empty()) finalResult += "." + resultFrac;
        if (signA == '-') finalResult = "-" + finalResult;
    } else {
        // Different signs = subtraction
        bool A_is_larger = is_greater(intA, fracA, intB, fracB);
        if (A_is_larger) {
            resultFrac = subtract_parts(fracA, fracB, borrow);
            if (borrow) {
                intA = to_string(stoi(intA) - 1);
            }
            resultInt = subtract_parts(intA, intB, borrow);
            finalResult = resultInt;
            if (!fracA.empty()) finalResult += "." + resultFrac;
            if (signA == '-') finalResult = "-" + finalResult;
        } else {
            resultFrac = subtract_parts(fracB, fracA, borrow);
            if (borrow) {
                intB = to_string(stoi(intB) - 1);
            }
            resultInt = subtract_parts(intB, intA, borrow);
            finalResult = resultInt;
            if (!fracB.empty()) finalResult += "." + resultFrac;
            if (signB == '-') finalResult = "-" + finalResult;
        }
    }

    // Trim leading zeros
    if (finalResult[0] == '+' || finalResult[0] == '-') {
        char s = finalResult[0];
        finalResult = s + finalResult.substr(1).erase(0, finalResult.find_first_not_of("0", 1));
    } else {
        finalResult.erase(0, finalResult.find_first_not_of('0'));
    }

    // Trim trailing zeros in decimal part
    size_t dot = finalResult.find('.');
    if (dot != string::npos) {
        while (!finalResult.empty() && finalResult.back() == '0') finalResult.pop_back();
        if (finalResult.back() == '.') finalResult.pop_back();
    }

    if (finalResult.empty() || finalResult == "-") return "0";
    return finalResult;
}

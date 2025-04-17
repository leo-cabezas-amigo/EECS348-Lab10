/*
-------------------------------------------------------------------------------------
File name       = doublestr.cpp
Creation date   = 4/14/2025
-------------------------------------------------------------------------------------
Author          = Leo Cabezas Amigo (KUID 3109786)
Class           = EECS 348
Lab #           = 10
-------------------------------------------------------------------------------------
*/

#include "doublestr.hpp"

const std::set<char> num_chars = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
const std::set<char> nz_num_chars = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
const std::set<char> wspc = {' ', '\t', '\n', '\r', '\v', '\f'};
const std::set<char> sign_chars = {'+', '-'};

DoubleStr::DoubleStr(){
    this->double_str = std::string();
    this->is_valid = false;
    return;
}

DoubleStr::DoubleStr(std::string double_str){
    this->double_str = double_str;
    
    if (this->validate_number()){
        this->is_valid = true;
    } else {
        this->is_valid = false;
    }
    return;
}

bool DoubleStr::validate_number(){
    if (this->double_str.length() == 0){
        return false;
    }

    size_t dot_cnt = 0;
    size_t num_before_dot = 0, num_after_dot = 0;
    for (size_t i = 0; i < this->double_str.length(); i++){
        if (sign_chars.find(this->double_str[i]) != sign_chars.end()){
            if (i != 0){return false;}
        } else if (this->double_str[i] == '.'){
            dot_cnt++;
        } else if (num_chars.find(this->double_str[i]) != num_chars.end()){
            if (dot_cnt == 0){num_before_dot++;}
            else {num_after_dot++;}
        } else {
            return false;
        }
    }

    bool CASE1 = (dot_cnt > 1);
    bool CASE2 = (num_before_dot == 0);
    bool CASE3 = (dot_cnt == 1) && (num_after_dot == 0);

    if (CASE1 || CASE2 || CASE3){
        return false;
    } else {
        return true;
    }
}

DoubleStr DoubleStr::operator + (const DoubleStr& other) const {
    DoubleStr result;
    if (!is_valid || !other.is_valid) {
        result.double_str = "Error: Invalid operands";
        result.is_valid = false;
        return result;
    }

    bool neg1 = (double_str[0] == '-');
    bool neg2 = (other.double_str[0] == '-');
    std::string n1 = neg1 ? double_str.substr(1) : double_str;
    std::string n2 = neg2 ? other.double_str.substr(1) : other.double_str;

    if (!neg1 && !neg2) {
        // Both numbers are positive: Add them.
        result.double_str = add_unsigned_strings(n1, n2);
    } else if (neg1 && neg2) {
        // Both numbers are negative: Add their magnitudes and add a negative sign.
        result.double_str = "-" + add_unsigned_strings(n1, n2);
    } else if (neg1) { // -n1 + n2  => n2 - n1
        // First number is negative, second is positive: Subtract magnitudes.
        if (is_greater_or_equal_unsigned(n2, n1)) {
            result.double_str = subtract_unsigned_strings(n2, n1);
        } else {
            result.double_str = "-" + subtract_unsigned_strings(n1, n2);
        }
    } else {         // n1 + (-n2) => n1 - n2
        // First number is positive, second is negative: Subtract magnitudes.
        if (is_greater_or_equal_unsigned(n1, n2)) {
            result.double_str = subtract_unsigned_strings(n1, n2);
        } else {
            result.double_str = "-" + subtract_unsigned_strings(n2, n1);
        }
    }
    result.is_valid = true;
    return result;
}

std::string DoubleStr::add_unsigned_strings(std::string num1, std::string num2) const {
    std::string int1 = split_decimal_int(num1);
    std::string dec1 = split_decimal_frac(num1);
    std::string int2 = split_decimal_int(num2);
    std::string dec2 = split_decimal_frac(num2);


    // 2. Normalize the decimal parts (pad with trailing zeros).
    int max_len = 0;
    std::string norm_dec1 = normalize_decimal_length(dec1, dec2, max_len);
    std::string norm_dec2 = normalize_decimal_length(dec2, dec1, max_len);

    // 3. Add the decimal parts, handling carry.
    std::string sum_dec = "";
    int carry = 0;
    for (int i = max_len - 1; i >= 0; --i) {
        int d1 = (i < norm_dec1.length()) ? (norm_dec1[i] - '0') : 0;
        int d2 = (i < norm_dec2.length()) ? (norm_dec2[i] - '0') : 0;
        int sum = d1 + d2 + carry;
        sum_dec = std::to_string(sum % 10) + sum_dec; // Prepend the digit
        carry = sum / 10;
    }

    // 4. Add the integer parts, handling carry.
    std::string sum_int = "";
    int i = int1.length() - 1;
    int j = int2.length() - 1;
    while (i >= 0 || j >= 0 || carry) {
        int d1 = (i >= 0) ? (int1[i--] - '0') : 0;
        int d2 = (j >= 0) ? (int2[j--] - '0') : 0;
        int sum = d1 + d2 + carry;
        sum_int = std::to_string(sum % 10) + sum_int; // Prepend the digit
        carry = sum / 10;
    }

    // 5. Combine the integer and decimal parts.
    std::string result = sum_int;
    if (!sum_dec.empty() && !all_zeros(sum_dec)) {
        result += "." + sum_dec;
    }
    return result;
}

std::string DoubleStr::subtract_unsigned_strings(std::string num1, std::string num2) const {
    // 1. Split the numbers into integer and decimal parts.
    std::string int1 = split_decimal_int(num1);
    std::string dec1 = split_decimal_frac(num1);
    std::string int2 = split_decimal_int(num2);
    std::string dec2 = split_decimal_frac(num2);

    // 2. Normalize the decimal parts.
    int max_len = 0;
    std::string norm_dec1 = normalize_decimal_length(dec1, dec2, max_len);
    std::string norm_dec2 = normalize_decimal_length(dec2, dec1, max_len);

    // 3. Subtract the decimal parts, handling borrow.
    std::string diff_dec = "";
    int borrow = 0;
    for (int i = max_len - 1; i >= 0; --i) {
        int d1 = (i < norm_dec1.length()) ? (norm_dec1[i] - '0') : 0;
        int d2 = (i < norm_dec2.length()) ? (norm_dec2[i] - '0') : 0;
        int diff = d1 - d2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        diff_dec = std::to_string(diff) + diff_dec; // Prepend the digit
    }

    // 4. Subtract the integer parts, handling borrow.
    std::string diff_int = "";
    int i = int1.length() - 1;
    int j = int2.length() - 1;
    while (i >= 0 || j >= 0 || borrow) {
        int d1 = (i >= 0) ? (int1[i--] - '0') : 0;
        int d2 = (j >= 0) ? (int2[j--] - '0') : 0;
        int diff = d1 - d2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        diff_int = std::to_string(diff) + diff_int; // Prepend the digit
    }

    // 5. Combine the integer and decimal parts and remove leading zeros.
    std::string result = remove_leading_zeros(diff_int);
    if (!diff_dec.empty() && !all_zeros(diff_dec)) {
        result += "." + diff_dec;
    }
    return result;
}

// (The other helper functions remain the same, but are included for completeness)
int DoubleStr::string_max(int a, int b) const {
    return (a > b) ? a : b;
}

bool DoubleStr::all_zeros(const std::string& str) const {
    for (char c : str) {
        if (c != '0') return false;
    }
    return true;
}

std::string DoubleStr::split_decimal_int(const std::string& num) const {
    size_t decimalPos = num.find('.');
    if (decimalPos == std::string::npos) {
        return num;
    } else {
        return num.substr(0, decimalPos);
    }
}

std::string DoubleStr::split_decimal_frac(const std::string& num) const {
    size_t decimalPos = num.find('.');
    if (decimalPos == std::string::npos) {
        return "";
    } else {
        return num.substr(decimalPos + 1);
    }
}

std::string DoubleStr::normalize_decimal_length(std::string dec1, std::string dec2, int& max_len) const {
    max_len = string_max(dec1.length(), dec2.length());
    while (dec1.length() < max_len) dec1 += '0';
    while (dec2.length() < max_len) dec2 += '0';
    return dec1;
}

std::string DoubleStr::remove_leading_zeros(std::string num) const {
    size_t first_digit = 0;
    while (first_digit < num.length() - 1 && num[first_digit] == '0') {
        first_digit++;
    }
    return num.substr(first_digit);
}

bool DoubleStr::is_greater_or_equal_unsigned(std::string num1, std::string num2) const {
    std::string int1 = split_decimal_int(num1);
    std::string dec1 = split_decimal_frac(num1);
    std::string int2 = split_decimal_int(num2);
    std::string dec2 = split_decimal_frac(num2);

    if (int1.length() > int2.length()) return true;
    if (int1.length() < int2.length()) return false;
    if (int1 > int2) return true;
    if (int1 < int2) return false;

    int max_len = string_max(dec1.length(), dec2.length());
    std::string norm_dec1 = normalize_decimal_length(dec1, dec2, max_len);
    std::string norm_dec2 = normalize_decimal_length(dec2, dec1, max_len);

    for (int i = 0; i < max_len; ++i) {
        int d1 = (i < norm_dec1.length()) ? (norm_dec1[i] - '0') : 0;
        int d2 = (i < norm_dec2.length()) ? (norm_dec2[i] - '0') : 0;
        if (d1 > d2) return true;
        if (d1 < d2) return false;
    }
    return true;
}
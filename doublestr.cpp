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

const std::set<char> num_chars = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};    // Valid digits.
const std::set<char> nz_num_chars = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};      // Valid non-zero digits.
const std::set<char> wspc = {' ', '\t', '\n', '\r', '\v', '\f'};                        // White space characters.
const std::set<char> sign_chars = {'+', '-'};                                           // Sign characters.

DoubleStr::DoubleStr(){    // Default constructor.
    this->double_str = std::string();   // Initializes an empty string.
    this->is_valid = false;             // Sets default validity to false.
    return;
}

DoubleStr::DoubleStr(std::string double_str){   // Parameterized constructor.
    this->double_str = double_str;  // Set the input string as the number
    
    if (this->validate_number()) {
        this->is_valid = true;  // Set to true if the number is valid.
    } else {
        this->is_valid = false;  // Set to false if the number is invalid.
    }
    return;
}

bool DoubleStr::validate_number() const {   // Determines if a string is a valid double type representation.
    if (this->double_str.length() == 0){
        return false;  // Return false if string is empty.
    }

    size_t dot_cnt = 0;         // Number of decimal point symbols ('.') present.
    size_t num_before_dot = 0;  // Number of digits before the decimal point.
    size_t num_after_dot = 0;   // Number of digits after the decimal point.

    for (size_t i = 0; i < this->double_str.length(); i++) {
        if (sign_chars.find(this->double_str[i]) != sign_chars.end()){  // If a sign character.
            if (i != 0) { return false; }  // Sign character can only be placed at the start of the string.
        } 
        else if (this->double_str[i] == '.') {  // If a decimal point.
            dot_cnt++;  // Increment decimal point counter.
        } 
        else if (num_chars.find(this->double_str[i]) != num_chars.end()){  // If a number character.
            if (dot_cnt == 0) { num_before_dot++; }     // Increment num_before_dot if decimal dot not found yet.
            else { num_after_dot++; }                   // Increment num_after_dot if decimal dot already found.
        } else { return false; }  // If not a sign character, a dot, or a number, then an invalid character is present.
    }

    bool CASE1 = (dot_cnt > 1);             // If more than one decimal point is present.
    bool CASE2 = (num_before_dot == 0);     // If no digits before the decimal point.
    bool CASE3 = (dot_cnt == 1) && (num_after_dot == 0);  // If a decimal point is present but there's no digits after it.

    if (CASE1 || CASE2 || CASE3) {  // Return false if any of these cases are triggered.
        return false;
    } else {
        return true;  // Else, the number is valid.
    }
}

DoubleStr DoubleStr::operator + (const DoubleStr& other) {  // Implements addition for DoubleStr objects.
    DoubleStr result;
    if (!this->is_valid || !other.is_valid) {  // Checks if the operand are valid.
        std::cerr << "Error in 'DoubleStr::operator+()': Invalid operand(s).\n";
        return DoubleStr();  // Return an empty DoubleStr object if either operand is invalid.
    }

    bool neg1 = (this->double_str[0] == '-');  // Checks if the first number is negative.
    bool neg2 = (other.double_str[0] == '-');  // Checks if the second number is negative.

    // Gets the magnitudes of the numbers.
    std::string n1 = (neg1 || (this->double_str[0] == '+')) ? this->double_str.substr(1) : this->double_str;
    std::string n2 = (neg2 || (other.double_str[0] == '+')) ? other.double_str.substr(1) : other.double_str;

    if ((!neg1) && (!neg2)){    // Case 1: Both numbers are positive.
        result.double_str = add_unsigned_strings(n1, n2);
    } else if (neg1 && neg2){   // Case 2: Both numbers are negative.
        result.double_str = "-" + add_unsigned_strings(n1, n2);
    } else if (neg1){           // Case 3: First number is negative, second is positive.
        if (is_greater_or_equal_unsigned(n2, n1)){
            result.double_str = subtract_unsigned_strings(n2, n1);  // Subtract n1 from n2 if n2 >= n1.
        } else {        // Otherwise, subtract n2 from n1 and add the minus sign.
            result.double_str = "-" + subtract_unsigned_strings(n1, n2);
        }
    } else {    // Case 4: First number is positive, second is negative.
        if (is_greater_or_equal_unsigned(n1, n2)){
            result.double_str = subtract_unsigned_strings(n1, n2);  // Subtract n2 from n1 if n1 >= n2.
        } else {    // Otherwise, subtract n1 from n2 and add the minus sign.
            result.double_str = "-" + subtract_unsigned_strings(n2, n1);
        }
    }

    result.is_valid = true;  // Set the result as valid.
    return result;
}

std::string DoubleStr::add_unsigned_strings(std::string num1, std::string num2) const { // Adds two string-formatted unsigned numbers.
    // Split each number into integer and fractional parts.
    std::string int1 = split_decimal_int(num1);
    std::string int2 = split_decimal_int(num2);
    std::string dec1 = split_decimal_frac(num1);
    std::string dec2 = split_decimal_frac(num2);

    // Normalize the lengths of the decimal parts to ensure proper addition.
    normalize_decimal_length(dec1, dec2);

    // Add fractional parts, handling carry.
    std::string result_dec = "";
    int carry = 0;
    for (int i = dec1.length() - 1; i >= 0; i--) {
        int sum = (dec1[i] - '0') + (dec2[i] - '0') + carry;    // Add corresponding digits with carry.
        result_dec = char((sum % 10) + '0') + result_dec;       // Append result digit to result_dec.
        carry = sum / 10;  // Update carry for next iteration.
    }

    // Add integer parts, handling carry.
    std::string result_int = "";
    int i = int1.length() - 1;
    int j = int2.length() - 1;
    while (i >= 0 || j >= 0 || carry) {             // Adds integer parts while handling carry.
        int d1 = (i >= 0) ? int1[i--] - '0' : 0;    // Gets digit from first number, or 0 if out of range.
        int d2 = (j >= 0) ? int2[j--] - '0' : 0;    // Gets digit from second number, or 0 if out of range.
        int sum = d1 + d2 + carry;                  // Adds digits and carry.
        
        result_int = char((sum % 10) + '0') + result_int;   // Append result digit to result_int.
        carry = sum / 10;   // Update carry for next iteration.
    }

    result_int = remove_leading_zeros(result_int);      // Removes leading zeros from integer part.
    result_dec = remove_trailing_zeros(result_dec);     // Removes trailing zeros from fractional part.

    if (!result_dec.empty())                    // If there is a fractional part,
        return result_int + "." + result_dec;   // returns integer and fractional parts combined.

    return result_int + ".0";  // If no fractional part, return the integer part + ".0".
}

std::string DoubleStr::subtract_unsigned_strings(std::string n1, std::string n2) const {    // Subtracts two string-formatted unsigned numbers.
    // Splits each number into integer and fractional parts.
    std::string int1 = split_decimal_int(n1);
    std::string int2 = split_decimal_int(n2);
    std::string dec1 = split_decimal_frac(n1);
    std::string dec2 = split_decimal_frac(n2);

    // Normalizes the lengths of the decimal parts to ensure proper subtraction.
    normalize_decimal_length(dec1, dec2);

    // Subtracts fractional parts, handling borrow.
    std::string result_dec = "";
    int borrow = 0;
    for (int i = dec1.size() - 1; i >= 0; i--) {
        int d1 = dec1[i] - '0' - borrow;    // Subtracts with borrow from previous iteration.
        int d2 = dec2[i] - '0';             // Gets digit from second number.
        
        if (d1 < d2) {  // If the first digit is smaller, borrows from the next place.
            d1 += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result_dec = char((d1 - d2) + '0') + result_dec;  // Appends result digit to result_dec.
    }

    std::string result_int = "";
    int i = int1.length() - 1;
    int j = int2.length() - 1;
    while (i >= 0 || j >= 0) {      // Subtracts integer parts while handling borrow.
        int d1 = (i >= 0) ? int1[i--] - '0' : 0;  // Gets digit from first number, or 0 if out of range.
        int d2 = (j >= 0) ? int2[j--] - '0' : 0;  // Gets digit from second number, or 0 if out of range.
        d1 -= borrow;   // Applies borrow from previous subtraction.
        if (d1 < d2) {  // If the first digit is smaller, borrows from the next place.
            d1 += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result_int = char((d1 - d2) + '0') + result_int;    // Appends result digit to result_int.
    }

    result_int = remove_leading_zeros(result_int);      // Removes leading zeros from integer part.
    result_dec = remove_trailing_zeros(result_dec);     // Removes trailing zeros from fractional part.

    if (!result_dec.empty())                    // If there is a fractional part,
        return result_int + "." + result_dec;   // returns the integer and fractional parts combined.

    return result_int + ".0";  // If no fractional part, returns just the integer part + ".0".
}

bool DoubleStr::is_greater_or_equal_unsigned(std::string n1, std::string n2) const {
    std::string i1 = split_decimal_int(n1);     // Extracts integer part from n1.
    std::string i2 = split_decimal_int(n2);     // Extracts integer part from n2.
    i1 = remove_leading_zeros(i1);              // Removes leading zeros before the comparison.
    i2 = remove_leading_zeros(i2);

    if (i1.length() != i2.length()){        // If integer lengths differ, the longer one is larger.
        return i1.length() > i2.length();
    }
    if (i1 != i2){      // If lengths equal but values differ, compare lexicographically.
        return i1 > i2;
    }
    std::string f1 = split_decimal_frac(n1);    // Extracts fractional part from n1.
    std::string f2 = split_decimal_frac(n2);    // Extracts fractional part from n2.
    normalize_decimal_length(f1, f2);           // Pads with zeros to match fractional lengths.

    return f1 >= f2;    // Lexicographically compares fractional parts.
}

void DoubleStr::normalize_decimal_length(std::string& dec1, std::string& dec2) const {  // Pads the shortest string with zeros at the end.
    while (dec1.size() < dec2.size()) dec1 += '0'; 
    while (dec2.size() < dec1.size()) dec2 += '0';
}

std::string DoubleStr::split_decimal_int(const std::string& num) const {
    size_t dot = num.find('.'); // Finds position of the decimal point.
    return num.substr(0, dot);  // Returns substring before the decimal point.
}

std::string DoubleStr::split_decimal_frac(const std::string& num) const {
    size_t dot = num.find('.');     // Finds position of the decimal point.
    return (dot == std::string::npos) ? "" : num.substr(dot + 1); // If no decimal point, returns an empty string; else, returns the fractional substring.
}

std::string DoubleStr::remove_leading_zeros(std::string num) const {
    size_t i = 0;
    while (i < num.size() - 1 && num[i] == '0') i++;    // Skips all leading zeroes.
    return num.substr(i);   // Returns substring without leading zeroes.
}

std::string DoubleStr::remove_trailing_zeros(const std::string& num) const {
    size_t end = num.size();
    while (end > 0 && num[end - 1] == '0') end--;   // Trims trailing zeros from the end
    return num.substr(0, end);  // Returns substring without trailing zeroes.
}

bool DoubleStr::all_zeros(const std::string& str) const {
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] != '0') {    // If any character isn't '0', return false.
            return false;
        }
    }
    return true;    // All characters are '0'.
}

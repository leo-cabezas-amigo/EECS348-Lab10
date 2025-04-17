/*
-------------------------------------------------------------------------------------
File name       = doublestr.hpp
Creation date   = 4/14/2025
-------------------------------------------------------------------------------------
Author          = Leo Cabezas Amigo (KUID 3109786)
Class           = EECS 348
Lab #           = 10
-------------------------------------------------------------------------------------
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

#ifndef __DOUBLESTR_HPP__
#define __DOUBLESTR_HPP__

extern const std::set<char> num_chars;     // All digit characters ('0' to '9').
extern const std::set<char> nz_num_chars;  // Non-zero digit characters ('1' to '9').
extern const std::set<char> wspc;          // Whitespace characters.
extern const std::set<char> sign_chars;    // '+' and '-' sign characters.

class DoubleStr {
    public:
        std::string double_str;
        bool is_valid;

        DoubleStr();

        DoubleStr(std::string double_str);

        bool validate_number();

        DoubleStr operator + (const DoubleStr& other) const;

        DoubleStr operator - (std::string& odouble_str);

    private:
        std::string add_unsigned_strings(std::string num1, std::string num2) const;
        std::string subtract_unsigned_strings(std::string num1, std::string num2) const;
        bool is_greater_or_equal_unsigned(std::string num1, std::string num2) const;
        std::string split_decimal_int(const std::string& num) const;
        std::string split_decimal_frac(const std::string& num) const;
        std::string normalize_decimal_length(std::string dec1, std::string dec2, int& max_len) const;
        std::string remove_leading_zeros(std::string num) const;
        int string_max(int a, int b) const;
        bool all_zeros(const std::string& str) const;
};

#endif // __DOUBLESTR_HPP__
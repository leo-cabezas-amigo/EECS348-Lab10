/*
-------------------------------------------------------------------------------------
File name       = main.cpp
Creation date   = 4/14/2025
-------------------------------------------------------------------------------------
Author          = Leo Cabezas Amigo (KUID 3109786)
Class           = EECS 348
Lab #           = 10
-------------------------------------------------------------------------------------
*/

#include "doublestr.hpp"

std::vector<DoubleStr> load_numbers(std::ifstream& file){   // Reads double numbers from a file and returns a vector of DoubleStr instances.
    std::vector<DoubleStr> numbers;
    std::string raw_line, proc_line;
    
    while (std::getline(file, raw_line)){   // Reads file line-by-line.
        for (size_t i = 0; i < raw_line.size(); i++){
            if (wspc.find(raw_line[i]) == wspc.end()){   
                proc_line += raw_line[i];   // Only appends non-whitespace characters of raw_line to proc_line.
            }
        }
        
        numbers.push_back(DoubleStr(proc_line));    // Pushes the DoubleStr version of proc_line into the vector.
        proc_line.clear();
    }
    return numbers;     // Returns a vector containing all numbers read from the file, in DoubleStr format.
}

void test_DoubleStr(std::vector<DoubleStr>& numbers, const std::string& const_num_str){
    DoubleStr const_num(const_num_str);
    DoubleStr add_result;
    
    if (!const_num.is_valid){   // Only allow valid constant numbers to add to every line number.
        std::cerr << "Error in 'test_DoubleStr()': Invalid 'const_num_str' input string.\n";
        return;
    }

    std::cout << "\n=============== TESTING DOUBLESTR CLASS ===============\n";
    for (size_t i = 0; i < numbers.size(); i++){
        if (!numbers[i].is_valid){
            std::cout << "ATTENTION: Number at line [" << i << "] is not a valid double.\n";
        } else {
            add_result = numbers[i] + const_num;
            std::cout << "Line [" << i << "]: (" << numbers[i].double_str << ") + (" << const_num.double_str << ") = " << add_result.double_str << "\n";
        }
    }
    std::cout << "=============== TEST ENDED SUCCESSFULLY ===============\n\n";
    return;
}

int main(int argc, char** argv){
    std::ifstream file;
    if (argc > 1){  // If a filepath is specified (argv[1]).
        file.open(argv[1]);     // Opens file specified by runtime args.
        if (!file.is_open()){
            std::cerr << "Error in 'main()': Could not open specified file.\n";
            return 1;
        }
    } else {        // If no filepath is specified.
        file.open("Lab10_Test_File.txt");
        if (!file.is_open()){
            std::cerr << "Error in 'main()': Could not open default file 'Lab10_Test_File.txt'.\n";
            return 1;
        }
    }

    const std::string const_num = "-123.456";
    std::vector<DoubleStr> numbers = load_numbers(file);
    test_DoubleStr(numbers, const_num);

    file.close();
    return 0;
}
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

/*
void load_numbers(std::ifstream& file){
    std::string raw_line, proc_line;
    
    while (std::getline(file, raw_line)){
        for (size_t i = 0; i < raw_line.size(); i++) {
            if (wspc.find(raw_line[i]) == wspc.end()) {
                proc_line += raw_line[i];
            }
        }
        this->numbers.push_back(proc_line);
    }
    return;
}
*/
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

    DoubleStr db1("+00000070.124315313");
    DoubleStr db2("-00000046.124315313");
    DoubleStr addRes = db1 + db2;
    std::cout << "Addition result = '" << addRes.double_str << "'\n";

    file.close();
    return 0;
}
#include <iostream>
#include "analyzer.h"

int main(int argc, char** argv)
{
        if (argc != 3) {
                std::cerr << "Usage:\n";
                std::cerr << "\t" << *argv << " -f <filename>\n";
                std::cerr << "\t" << *argv << " -d <dir>\n";
                return 1;
        }

        std::string input_type = *(argv + 1);
        std::string input_path = *(argv + 2);

        if (input_type != "-f" && input_type != "-d") {
                std::cerr << "Usage:\n";
                std::cerr << "\t" << *argv << " -f <filename>\n";
                std::cerr << "\t" << *argv << " -d <dir>\n";
                return 1;
        }

        try {
                Analyzer a (input_path, input_type);
                a.begin();

        } catch (const std::domain_error &err) {
                std::cerr << err.what() << std::endl;
                return 1;
        }

        std::cout << "[INFO] Compilation completed" << std::endl;

        return 0;
}

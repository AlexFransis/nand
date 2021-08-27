#include <iostream>
#include <fstream>
#include <cassert>
#include <iterator>
#include <stdexcept>
#include <vector>
#include <filesystem>
#include "translator.h"

int main(int argc, char** argv)
{
        if (argc != 3) {
                std::cerr << "Usage:\n";
                std::cerr << "\t" << *argv << " -f <filename>\n";
                std::cerr << "\t" << *argv << " -d <dir>\n";
                return 1;
        }

        std::string input_type = *(argv + 1);
        std::string input = *(argv + 2);

        if (input_type != "-f" && input_type != "-d") {
                std::cerr << "Usage:\n";
                std::cerr << "\t" << *argv << " -f <filename>\n";
                std::cerr << "\t" << *argv << " -d <dir>\n";
                return 1;
        }

        VMTranslator translator (input, input_type == "-d" ? INPUT_TYPE::D : INPUT_TYPE::F);

        try {
                translator.translate();
        } catch (const std::domain_error &err) {
                std::cerr << err.what() << std::endl;
        }

        return 0;
}

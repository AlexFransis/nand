#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
        if (argc != 3) {
                std::cerr << "Usage:\n";
                std::cerr << "\t" << *argv << " -f <filename>\n";
                std::cerr << "\t" << *argv << " -d <dir>\n";
                return 1;
        }

        std::string input_type = *(argv + 1);
        assert(input_type == "-f" || input_type == "-d");


        std::string input = *(argv + 2);
        if (input_type == "-d") {
                const fs::path p = "test";
                if (p.empty()) {
                        std::cerr << "Invalid directory path " << p << std::endl;
                        return 1;
                }
        }

        if (input_type == "-f") {

        }

        return 0;
}

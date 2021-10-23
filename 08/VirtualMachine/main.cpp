#include <iostream>
#include "translator.h"
#include "file_handler.h"

namespace fs = std::filesystem;

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


        try {
                FileHandler fh (input, input_type == "-d" ? INPUT_TYPE::DIR : INPUT_TYPE::FILE);
                std::pair<std::vector<fs::path>, fs::path> io = fh.get_io_paths();
                Translator translator (io.first, io.second);
                translator.begin();
        } catch (const std::domain_error &err) {
                std::cerr << err.what() << std::endl;
                return 1;
        }

        std::cout << "[INFO] Translation completed" << std::endl;

        return 0;
}

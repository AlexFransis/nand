#include <fstream>
#include <string>
#include "src/assembler.h"


int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1;
	}

	std::string in_ext = ".asm";
	std::string file = argv[1];
	std::ifstream f_stream (file);
        std::size_t pos = file.find(in_ext);

        if (!f_stream || pos == std::string::npos) {
                std::cerr << "Invalid file: " << argv[1] << std::endl;
                return 1;
        }

        std::string file_name = file.substr(0, pos);
        Assembler assembler (file_name, f_stream);

        try {
                assembler.translate();
        }
        catch (...) {
                std::cerr << "Could not assemble file: <" << file_name << "> " << std::endl;
        }

	return 0;
}

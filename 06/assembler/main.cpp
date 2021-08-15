#include <iostream>
#include <fstream>
#include <string>
#include "src/parser.h"


int main(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1;
	}

	std::string file = argv[1];
	std::ifstream f_stream (file);
	std::size_t found = file.find(".asm");

	if (!f_stream || (found == std::string::npos)) {
		std::cerr << "Invalid file: " << argv[1] << std::endl;
		return 1;
	}
	
	if (f_stream.is_open()) {
		Parser parser (f_stream);
		parser.parse();
	}

	return 0;
}


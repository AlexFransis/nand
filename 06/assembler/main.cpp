#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include "src/parser.h"
#include "src/code.h"
#include "src/symbol_table.h"


int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1;
	}

	std::string in_ext = ".asm";
	std::string out_ext = ".hack";
	std::string file_name = argv[1];
	std::ifstream f_stream (file_name);
	std::size_t pos = file_name.find(in_ext);

	if (!f_stream || (pos == std::string::npos)) {
		std::cerr << "Invalid file: " << argv[1] << std::endl;
		return 1;
	}


	if (f_stream.is_open()) {
		std::ofstream out_file;
		out_file.open(file_name.substr(0, pos) + out_ext);
		Parser parser (f_stream);
		Code code;
                SymbolTable symbol_table;

                // first pass
                while (parser.has_more_commands()) {
                        parser.advance();
			std::string curr_line = parser.current_line();

			Command c_type = parser.command_type(curr_line);
                        if (c_type == L_COMMAND) {
                                std::string curr_sym = parser.symbol(curr_line);
                                symbol_table.add(curr_sym, parser.lc());
                        }
                }

                // reset stream
                f_stream.clear();
                f_stream.seekg(0);

                // second pass
		while (parser.has_more_commands()) {
			parser.advance();
			std::string curr_line = parser.current_line();

			Command c_type = parser.command_type(curr_line);
			switch (c_type) {
                                case A_COMMAND: {
                                        std::string symbol = parser.symbol(curr_line);
                                        uint16_t addr;
                                        if (std::isdigit(symbol.front())) {
                                                addr = code.immediate(symbol);
                                        }
                                        else {
                                                if (!symbol_table.contains(symbol)) {
                                                        symbol_table.add(symbol);
                                                }
                                                addr = symbol_table.get_address(symbol);

                                        }

                                        // left most bit of A_COMMAND
                                        out_file << 0;

                                        // 15 bit val
                                        for (int i = 14; i >= 0; --i) {
                                                bool bit = (1 << i) & addr;
                                                out_file << bit;
                                        }

                                        out_file << std::endl;
                                        break;
                                }
                                case C_COMMAND: {
                                        uint8_t dest_code = code.dest(parser.dest(curr_line));
                                        uint8_t comp_code = code.comp(parser.comp(curr_line));
                                        uint8_t jump_code = code.jump(parser.jump(curr_line));

                                        // left most bit of C_COMMAND
                                        out_file << 1;

                                        // two unused bits
                                        out_file << 1;
                                        out_file << 1;

                                        // 7 bit comp => a c6 c5 c4 c3 c2 c1
                                        for (int i = 6; i >= 0; --i) {
                                                bool bit = (1 << i) & comp_code;
                                                out_file << bit;
                                        }

                                        // 3 bit dest => d1 d2 d3
                                        for (int i = 2; i >= 0; --i) {
                                                bool bit = (1 << i) & dest_code;
                                                out_file << bit;
                                        }

                                        // 3 bit jump => j1 j2 j3
                                        for (int i = 2; i >= 0; --i) {
                                                bool bit = (1 << i) & jump_code;
                                                out_file << bit;
                                        }

                                        out_file << std::endl;
                                        break;
                                }
                                default:
                                        break;
			}
		}

        out_file.close();
	}

	return 0;
}

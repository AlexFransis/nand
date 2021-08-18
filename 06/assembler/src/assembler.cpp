#include "assembler.h"


Assembler::Assembler(const std::string &file_name, std::ifstream &fstream):
        m_file_name(file_name),
        m_fstream(fstream)
{
        std::string out_ext = ".hack";
        m_ostream.open(m_file_name + out_ext);
}

Assembler::~Assembler()
{
        m_fstream.close();
        m_ostream.close();
}

void Assembler::translate()
{
	if (m_fstream.is_open()) {
		Parser parser (m_fstream);
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
                m_fstream.clear();
                m_fstream.seekg(0);

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
                                        std::string instr = assemble_ainstr(addr);
                                        m_ostream << instr << std::endl;
                                        break;
                                }
                                case C_COMMAND: {
                                        uint8_t dest_code = code.dest(parser.dest(curr_line));
                                        uint8_t comp_code = code.comp(parser.comp(curr_line));
                                        uint8_t jump_code = code.jump(parser.jump(curr_line));
                                        std::string instr = assemble_cinstr(dest_code, comp_code, jump_code);
                                        m_ostream << instr << std::endl;
                                        break;
                                }
                                default:
                                        break;
			}
		}
	}
}


std::string Assembler::assemble_ainstr(uint16_t code)
{
        std::string instr;
        // left most bit of A_COMMAND
        instr.append("0");

        // 15 bit val
        for (int i = 14; i >= 0; --i) {
                bool bit = (1 << i) & code;
                instr.append(std::to_string(bit));
        }

        return instr;
}


std::string Assembler::assemble_cinstr(uint8_t dest_code,
                                       uint8_t comp_code,
                                       uint8_t jump_code)
{
        std::string instr;

        // left most bit of C_COMMAND
        instr.append("1");

        // two unused bits
        instr.append("1");
        instr.append("1");

        // 7 bit comp => a c6 c5 c4 c3 c2 c1
        for (int i = 6; i >= 0; --i) {
                bool bit = (1 << i) & comp_code;
                instr.append(std::to_string(bit));
        }

        // 3 bit dest => d1 d2 d3
        for (int i = 2; i >= 0; --i) {
                bool bit = (1 << i) & dest_code;
                instr.append(std::to_string(bit));
        }

        // 3 bit jump => j1 j2 j3
        for (int i = 2; i >= 0; --i) {
                bool bit = (1 << i) & jump_code;
                instr.append(std::to_string(bit));
        }

        return instr;
}

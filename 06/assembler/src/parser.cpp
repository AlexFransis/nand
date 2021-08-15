#include <fstream>
#include <iostream>
#include <bitset>
#include "parser.h"
#include "code.h"


Parser::Parser(std::ifstream &fstream)
	:m_fstream(fstream)
{
}

std::string ltrim(const std::string &s) {
	const std::string ws = " \n\r\t\f\v";
	size_t start = s.find_first_not_of(ws);
	return (start == std::string::npos) ? std::string() : s.substr(start);
}

std::string rtrim(const std::string &s) {
	const std::string ws = " \n\r\t\f\v";
	size_t end = s.find_last_not_of(ws);
	return (end == std::string::npos) ? std::string() : s.substr(0, end + 1);
}


void Parser::parse() {
	while (has_more_commands()) {
		advance();
		const std::string no_comments = trim_comments(m_line);
		const std::string no_ws = trim_ws(no_comments);
		const std::string curr_line = no_ws;

		if (curr_line.empty()) {
			continue;
		}

		Command c_type = command_type(curr_line);
		Code code;

		std::string curr_sym = symbol(curr_line);
		std::string curr_dest = dest(curr_line);
		std::string curr_comp = comp(curr_line);
		std::string curr_jump = jump(curr_line);

		switch (c_type) {
			case A_COMMAND:
				{
					uint16_t immediate = code.immediate(curr_sym);
					std::cout << "symbol: " << curr_sym << std::endl;
					std::cout << "symbol code: " << std::bitset<15>(immediate) << std::endl;
					break;
				}
			case L_COMMAND:
				{
					std::cout << "symbol: " << curr_sym << std::endl;
					break;
				}
			case C_COMMAND : 
				{
					uint8_t dest_code = code.dest(curr_dest);
					uint8_t dest_comp = code.comp(curr_comp);
					uint8_t dest_jump = code.jump(curr_jump);
					std::cout << "dest: " << curr_dest << std::endl;
					std::cout << "dest code: " << std::bitset<3>(dest_code) << std::endl;
					std::cout << "comp: " << curr_comp << std::endl;
					std::cout << "comp code: " << std::bitset<7>(dest_comp) << std::endl;
					std::cout << "jump: " << curr_jump << std::endl;
					std::cout << "jump code: " << std::bitset<3>(dest_jump) << std::endl;
					break;
				}
		}
	}
}

std::string Parser::trim_ws(const std::string &s) {
	return rtrim(ltrim(s));
}

std::string Parser::trim_comments(const std::string &s) {
	std::size_t pos = s.find("//");
	return (pos == std::string::npos) ? s : s.substr(0, pos);
}

bool Parser::has_more_commands() const {
	return !m_fstream.eof();
}

void Parser::advance() {
	std::getline(m_fstream, m_line);
}

Command Parser::command_type(const std::string &curr_line){
	const char& first_char = curr_line.front();

	if (first_char == '@') {
		return A_COMMAND;
	}

	if (first_char == '(') {
		return L_COMMAND;
	}

	return C_COMMAND;
}

std::string Parser::symbol(const std::string &curr_line) {
	const std::string symbols = "@(";
	size_t pos = curr_line.find_first_of(symbols);
	return (pos == std::string::npos) ? std::string() : curr_line.substr(pos + 1);
}

std::string Parser::dest(const std::string &curr_line) {
	const std::string dest = "=";
	size_t pos = curr_line.find_first_of(dest);
	return (pos == std::string::npos) ? std::string() : curr_line.substr(0, pos);
}

std::string Parser::comp(const std::string &curr_line) {
	const std::string comp1 = "=";
	const std::string comp2 = ";";
	size_t pos1 = curr_line.find_first_of(comp1);
	size_t pos2 = curr_line.find_first_of(comp2);

	// D=A+1
	if (pos1 != std::string::npos && pos2 == std::string::npos) {
		return curr_line.substr(pos1 + 1);
	}

	// A+1;JMP
	if (pos1 == std::string::npos && pos2 != std::string::npos) {
		return curr_line.substr(0, pos2);
	}

	// A+1
	if (pos1 == std::string::npos && pos2 == std::string::npos) {
		return curr_line;
	}

	return std::string();
}

std::string Parser::jump(const std::string &curr_line) {
	const std::string jump = ";";
	size_t pos = curr_line.find_first_of(jump);
	return pos == std::string::npos ? std::string() : curr_line.substr(pos);
}

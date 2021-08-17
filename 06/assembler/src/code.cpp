#include "code.h"

Code::Code()
{
	dest_table = {
		{"", 		0b000},
		{"M", 		0b001},
		{"D", 		0b010},
		{"MD", 		0b011},
		{"A", 		0b100},
		{"AM", 		0b101},
		{"AD", 		0b110},
		{"AMD", 	0b111},
	};

	jump_table = {
		{"", 		0b000},
		{"JGT", 	0b001},
		{"JEQ", 	0b010},
		{"JGE", 	0b011},
		{"JLT", 	0b100},
		{"JNE", 	0b101},
		{"JLE", 	0b110},
		{"JMP", 	0b111},
	};

	comp_table = {
		// when a=0
		{"0", 		0b0101010},
		{"1", 		0b0111111},
		{"-1", 		0b0111010},
		{"D", 		0b0001100},
		{"A", 		0b0110000},
		{"!D", 		0b0001101},
		{"!A", 		0b0110001},
		{"-D", 		0b0001111},
		{"-A", 		0b0110011},
		{"D+1", 	0b0011111},
		{"A+1", 	0b0110111},
		{"D-1", 	0b0001110},
		{"A-1", 	0b0110010},
		{"D+A", 	0b0000010},
		{"D-A", 	0b0010011},
		{"A-D", 	0b0000111},
		{"D&A", 	0b0000000},
		{"D|A", 	0b0010101},

		// when a=1
		{"M", 		0b1110000},
		{"!M", 		0b1110001},
		{"-M", 		0b1110011},
		{"M+1", 	0b1110111},
		{"M-1", 	0b1110010},
		{"D+M", 	0b1000010},
		{"D-M", 	0b1010011},
		{"M-D", 	0b1000111},
		{"D&M", 	0b1000000},
		{"D|M", 	0b1010101},
	};
}

uint8_t Code::dest(const std::string &s)
{
	return dest_table[s];
}

uint8_t Code::jump(const std::string &s)
{
	return jump_table[s];
}

uint8_t Code::comp(const std::string &s)
{
	return comp_table[s];
}

uint16_t Code::immediate(const std::string &s)
{
	int val;
	try {
		val = std::stoi(s);
	}
	catch (...) {
		throw std::runtime_error("invalid @ value");
	}

	// 2 ^ 15 - 1 = 32767 or 0x7fff
	if (val < 0 || val > 0x7FFF) {
		throw std::runtime_error("@ value out of range");
	}

	return (uint16_t)val;
}

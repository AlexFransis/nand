#include "symbol_table.h"


SymbolTable::SymbolTable():
        m_stack_pointer(0x0010),
        m_symbol_table({
                {"SP", 		0x0000},
                {"LCL", 	0x0001},
                {"ARG", 	0x0002},
                {"THIS", 	0x0003},
                {"THAT", 	0x0004},
                {"R0",	 	0x0000},
                {"R1",	 	0x0001},
                {"R2",	 	0x0002},
                {"R3",	 	0x0003},
                {"R4",	 	0x0004},
                {"R5",	 	0x0005},
                {"R6",	 	0x0006},
                {"R7",	 	0x0007},
                {"R8",	 	0x0008},
                {"R9",	 	0x0009},
                {"R10",	 	0x000A},
                {"R11",	 	0x000B},
                {"R12",	 	0x000C},
                {"R13",	 	0x000D},
                {"R14",	 	0x000E},
                {"R15",	 	0x000F},
                {"SCREEN",	0x4000}, // 16384
                {"KBD",		0x6000}, // 24576
                })
{
}

bool SymbolTable::contains(const std::string &symbol)
{
        std::unordered_map<std::string, uint16_t>::const_iterator got = m_symbol_table.find(symbol);
        return !(got == m_symbol_table.end());
}

uint16_t SymbolTable::get_address(const std::string &symbol)
{
        std::unordered_map<std::string, uint16_t>::const_iterator got = m_symbol_table.find(symbol);
        return got->second;
}

void SymbolTable::add(const std::string &symbol, uint16_t address)
{
        std::pair<std::string, uint16_t> pair (symbol, address);
        m_symbol_table.insert(pair);
}

void SymbolTable::add(const std::string &symbol)
{
        std::pair<std::string, uint16_t> pair (symbol, m_stack_pointer);
        m_symbol_table.insert(pair);
        m_stack_pointer++;
}

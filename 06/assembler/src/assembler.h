#include <fstream>
#include <iostream>
#include "parser.h"
#include "code.h"
#include "symbol_table.h"


class Assembler {
private:
        std::string				m_file_name;
        std::ifstream				&m_fstream;
        std::ofstream				m_ostream;

public:
        Assembler(const std::string&, std::ifstream&);
        ~Assembler();

        void					translate();
        std::string				assemble_ainstr(uint16_t);
        std::string				assemble_cinstr(uint8_t, uint8_t, uint8_t);
};

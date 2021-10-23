#include <iostream>
#include <list>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <cstdio>
#include "instr_mapper.h"
#include "parser.h"
#include "translator.h"

namespace fs = std::filesystem;

Translator::Translator(const std::vector<fs::path> &inputs, const fs::path &output)
        :m_inputs(inputs),
         m_output(output)
{
        m_ifstream = std::ifstream();
        m_ofstream = std::ofstream();
}

Translator::~Translator()
{
        m_ifstream.close();
        m_ofstream.close();
}

void Translator::begin()
{
        m_ofstream.open(m_output);
        if (!m_ofstream.good()) {
                std::string err = "[ERR] Could not open file: " + std::string(m_output);
                throw std::domain_error(err);
        }

        std::vector<fs::path>::const_iterator it = m_inputs.begin();
        while (it != m_inputs.end()) {
                m_ifstream.clear();
                m_ifstream.close();
                m_ifstream.open(*it);

                if (!m_ifstream.good()) {
                        std::string err = "[ERR] Could not open file: " + std::string(*it);
                        throw std::domain_error(err);
                }

                std::cout << "[INFO] Begin translation: " << std::string(*it) << std::endl;
                int line_number = 0;
                Parser p;
                InstructionMapper mapper (it->stem());
                while (!m_ifstream.eof()) {
                        line_number++;
                        Command vmc;
                        std::string current_line;
                        std::getline(m_ifstream, current_line);

                        if (!p.try_parse(current_line, vmc))
                        {
                                std::string err = "[ERR] Invalid command on line " + std::to_string(line_number) +
                                        " in file: " + std::string(it->stem());
                                throw std::domain_error(err);
                        }

                        std::list<std::string> asm_instrs = mapper.map_command(vmc);
                        for (const std::string &instr : asm_instrs) {
                                m_ofstream << instr << std::endl;
                        }
                }

                ++it;
        }
}

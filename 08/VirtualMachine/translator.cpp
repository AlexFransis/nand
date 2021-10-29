#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "instr_mapper.h"
#include "parser.h"
#include "translator.h"

namespace fs = std::filesystem;

std::string Translator::generate_uuid() const
{
        std::string alphanumeric = "0123456789abcdefghijklmnopqrstuvwxyz";
        char uuid[17];
        for (int i = 0; i < 16; i++) {
                int rand = std::rand() % alphanumeric.length();
                uuid[i] = alphanumeric[rand];
        }
        uuid[16] = '\0';

        return std::string(uuid);
}

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

        std::cout << "[INFO] Opening output file: " << std::string(m_output) << std::endl;
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
                InstructionMapper mapper;
                std::unordered_map<std::string, std::string> state;
                state["filename"] = it->stem();
                state["function_scope"] = "NULL";
                while (!m_ifstream.eof()) {
                        line_number++;
                        Command vmc;
                        std::string current_line;
                        std::getline(m_ifstream, current_line);

                        if (p.is_ws_or_comment(current_line)) {
                                continue;
                        }

                        if (!p.try_parse(current_line, vmc)) {
                                std::string err = "[ERR] Invalid command on line " + std::to_string(line_number) +
                                        " in file: " + std::string(it->stem());
                                throw std::domain_error(err);
                        }

                        if (vmc.name == "return") {
                                state["function_scope"] = "NULL";
                        }

                        if (vmc.name == "function") {
                                state["function_scope"] = vmc.arg1(); // update state with current function scope
                        }

                        state["uuid"] = generate_uuid(); // update state with uuid for labels
                        std::list<std::string> asm_instrs = mapper.map_command(vmc, state);
                        for (const std::string &instr : asm_instrs) {
                                m_ofstream << instr << std::endl;
                        }
                }

                ++it;
        }
}

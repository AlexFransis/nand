#ifndef GUARD_instr_mapper_h
#define GUARD_instr_mapper_h

#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include "parser.h"

typedef std::vector<std::string> instrs;
typedef std::unordered_map<std::string, instrs> instr_table;

class InstructionMapper {
private:
        static const instr_table		m_instr_table;
        std::string				m_uuid;
        std::string				m_filename;
        std::string				m_curr_func;

        bool					is_bracketed(const std::string &s);
        bool					is_placeholder(const std::string &s);
        bool					is_number(const std::string &s);
        bool					is_valid_index(const std::string &segment, const std::string &index);
        std::string 				generate_uuid() const;
        void					replace(size_t delim_start, size_t delim_end,
                                                        std::string &placeholder, const std::string &arg);
        std::vector<std::string>		resolve_placeholder(const std::string &placeholder, const Command &vm);
        void					map_command_aux(const std::string &command,
                                                                const Command &vm,
                                                                std::list<std::string> &asm_instrs);
public:
        InstructionMapper(const std::string &filename);

        std::list<std::string>			map_command(const Command &command);
};

#endif

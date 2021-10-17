#ifndef GUARD_command_translator_h
#define GUARD_command_translator_h

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <iterator>
#include "command.h"

typedef std::vector<std::string> instrs;
typedef std::unordered_map<std::string, instrs> instr_table;

class CommandTranslator {
private:
        instr_table				m_instr_table;
        std::string				m_uuid;
        std::string				m_filename;
        std::string				m_curr_func;

        bool					is_bracketed(const std::string &s);
        bool					is_placeholder(const std::string &s);
        bool					is_valid_index(const std::string &segment,
                                                               const std::string &index);

        std::string 				generate_uuid() const;
        instr_table 				init_instr_table() const;

        void					replace(size_t delim_start,
                                                        size_t delim_end,
                                                        std::string &placeholder,
                                                        const std::string &arg);
        std::vector<std::string>		resolve_placeholder(const std::string &placeholder, const Command &vm);
        void					translate_command_aux(const std::string &command,
                                                                      const Command &vm,
                                                                      std::list<std::string> &asm_instrs);
public:
        CommandTranslator(const std::string &filename);

        std::list<std::string>			translate_command(const Command &command);
};

#endif

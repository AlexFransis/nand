#ifndef GUARD_asm_commands_h
#define GUARD_asm_commands_h

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <iterator>
#include "vm_command.h"

typedef std::vector<std::string> commands;
typedef std::unordered_map<std::string, commands> command_table;

class ASMCommands {
private:
        command_table				m_asm_rules;
        bool					is_bracketed(const std::string &s);
        bool					is_valid_index(const std::string &segment,
                                                               const std::string &str_index);
        command_table 				init_rules();
        std::string 				get_uuid();
        void					get_asm_commands_aux(const command_table &c_table,
                                                                     const std::string &command,
                                                                     std::list<std::string> &asm_commands);
        void					resolve_placeholders(const command_table &c_table,
                                                                     const VMCommand &command,
                                                                     std::list<std::string> &asm_commands);

public:
        ASMCommands();

        std::list<std::string>		get_asm_commands(const VMCommand &command);
};

#endif

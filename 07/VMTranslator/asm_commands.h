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
        std::string				m_uuid;
        std::string				m_filename;
        bool					is_bracketed(const std::string &s);
        bool					is_placeholder(const std::string &s);
        bool					is_valid_index(const std::string &segment,
                                                               const std::string &str_index);
        command_table 				init_rules() const;
        std::string 				get_uuid() const;
        commands				resolve_placeholder(const command_table &c_table,
                                                                    const VMCommand &vm,
                                                                    const std::string &s);
        void					get_asm_commands_aux(const command_table &c_table,
                                                                     const std::string &command,
                                                                     const VMCommand &vm,
                                                                     std::list<std::string> &asm_commands);
public:
        ASMCommands(const std::string &filename);

        std::list<std::string>			get_asm_commands(const VMCommand &command);
};

#endif

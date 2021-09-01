#ifndef GUARD_asm_commands_h
#define GUARD_asm_commands_h

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
        bool					bracketed(const std::string &s);
        command_table				m_asm_rules;
        command_table 				init_rules();

        template<class insert_iterator>
        void					get_asm_commands_aux(const command_table &c_table,
                                                                     const std::string &command,
                                                                     insert_iterator insert);

public:
        ASMCommands();

        std::list<std::string>		get_asm_commands(const VMCommand &command);
};

#endif

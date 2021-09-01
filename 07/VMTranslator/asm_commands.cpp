#include "asm_commands.h"
#include <stdexcept>
#include <string>
#include <unordered_map>

typedef std::vector<std::string> commands;
typedef std::unordered_map<std::string, commands> command_table;

ASMCommands::ASMCommands()
        : m_asm_rules(init_rules())
{
}


bool ASMCommands::bracketed(const std::string &s)
{
        return s.size() > 1 && *(s.begin()) == '<' && *--(s.end()) == '>';
}

std::string to_brackets(const std::string &s)
{
        return "<" + s + ">";
}

template<class insert_iterator>
void ASMCommands::get_asm_commands_aux(const command_table &c_table, const std::string &command, insert_iterator insert)
{
        if (!bracketed(command)) {
                *insert++ = command;
                return;
        }

        command_table::const_iterator found = c_table.find(command);
        if (found == c_table.end()) {
                std::domain_error("Invalid command: " + command);
        }

        commands commands = found->second;
        commands::const_iterator it = commands.begin();
        while (it != commands.end()) {
                get_asm_commands_aux(c_table, *it, insert);
                ++it;
        }
}


std::list<std::string> ASMCommands::get_asm_commands(const VMCommand &command)
{
        // get all possible asm translations for a given vm command

        std::string c_type = command.command();
        std::list<std::string> asm_commands;

        // pass a back inserter to add commands
        get_asm_commands_aux(m_asm_rules, to_brackets(c_type), std::back_inserter(asm_commands));

        if (c_type == "push" || c_type == "pop") {
                // insert < store > command at front
                get_asm_commands_aux(m_asm_rules, to_brackets("store"), std::front_inserter(asm_commands));
                // insert < segment index > to front of commands
                std::string arg2 = command.arg2();
                asm_commands.push_front("\n");
                asm_commands.push_front(arg2);
                std::string arg1 = command.arg1();
                get_asm_commands_aux(m_asm_rules, to_brackets(arg1), std::front_inserter(asm_commands));
        }

        return asm_commands;
}

command_table ASMCommands::init_rules()
{
        return {
                {"<argument>", 		{"@ARG\n"}},
                {"<local>", 		{"@LCL\n"}},
                {"<static>", 		{"@R"}},
                {"<constant>", 		{"@"}},
                {"<this>", 		{"@THIS\n"}},
                {"<that>", 		{"@THAT\n"}},
                {"<pointer>", 		{"<this>", "<that>"}},
                {"<temp>", 		{"@R"}},
                {"<move>", 		{"@SP\n", "A=M\n", "M=D\n"}},
                {"<increment>", 	{"@SP\n", "AM=M+1\n"}},
                {"<decrement>", 	{"@SP\n", "AM=M-1\n"}},
                {"<compute-add>", 	{"M=M+D\n"}},
                {"<compute-sub>", 	{"M=M-D\n"}},
                {"<compute-neg>", 	{"M=M+D\n"}},
                {"<compute-eq>", 	{"M=M+D\n"}},
                {"<compute-gt>", 	{"M=M+D\n"}},
                {"<compute-lt>", 	{"M=M+D\n"}},
                {"<compute-and>", 	{"M=M+D\n"}},
                {"<compute-or>", 	{"M=M+D\n"}},
                {"<compute-not>", 	{"M=M+D\n"}},
                {"<store>", 		{"D=A\n"}},
                {"<push>", 		{"<move>", "<increment>"}},
                {"<add>", 		{"<decrement>", "<decrement>", "<compute-add>"}},
                {"<sub>", 		{"<decrement>", "<decrement>", "<compute-sub>"}},
                {"<neg>", 		{"<decrement>", "<decrement>", "<compute-neg>"}},
                {"<eq>", 		{"<decrement>", "<decrement>", "<compute-eq>"}},
                {"<gt>", 		{"<decrement>", "<decrement>", "<compute-gt>"}},
                {"<lt>", 		{"<decrement>", "<decrement>", "<compute-lt>"}},
                {"<or>", 		{"<decrement>", "<decrement>", "<compute-or>"}},
                {"<not>", 		{"<decrement>", "<decrement>", "<compute-not>"}},
                {"<and>", 		{"<decrement>", "<decrement>", "<compute-and>"}},
        };
}

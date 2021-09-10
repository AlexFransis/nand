#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cassert>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include "asm_commands.h"

bool ASMCommands::is_bracketed(const std::string &s)
{
        return s.size() > 1 && *(s.begin()) == '<' && *--(s.end()) == '>';
}

bool ASMCommands::is_placeholder(const std::string &s)
{
        size_t delim_start = s.find("{{");
        size_t delim_end = s.find("}}");
        return delim_start != std::string::npos && delim_end != std::string::npos;
}

std::string to_brackets(const std::string &s)
{
        return "<" + s + ">";
}

bool ASMCommands::is_valid_index(const std::string &segment, const std::string &str_index)
{
        std::string::const_iterator it = str_index.begin();
        while (it != str_index.end()) {
                if (!std::isdigit(*it)) return false;
                ++it;
        }

        int index = std::stoi(str_index);
        if (index < 0) return false;
        if (segment == "constant") return index <= 32767;
        if (segment == "pointer") return index <= 1;
        if (segment == "temp") return index <= 7;

        return true;
}

std::string ASMCommands::get_uuid()
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

ASMCommands::ASMCommands()
        : m_asm_rules(init_rules()),
          m_uuid(get_uuid())
{
}

std::list<std::string> ASMCommands::get_asm_commands(const VMCommand &command)
{
        std::list<std::string> asm_commands;
        get_asm_commands_aux(m_asm_rules, to_brackets(command.command()), command, asm_commands);

        return asm_commands;
}

void ASMCommands::get_asm_commands_aux(const command_table &c_table, const std::string &command,
                                       const VMCommand &vm, std::list<std::string> &asm_commands)
{
        if (!is_bracketed(command) && !is_placeholder(command)) {
                asm_commands.push_back(command);
                return;
        }

        if (is_placeholder(command)) {
                commands resolved = resolve_placeholder(c_table, vm, command);
                for (const std::string &c : resolved) {
                        get_asm_commands_aux(c_table, c, vm, asm_commands);
                }
                return;
        }

        command_table::const_iterator found = c_table.find(command);
        if (found == c_table.end()) {
                std::domain_error("Invalid command: " + command);
        }

        commands commands = found->second;
        commands::iterator it = commands.begin();

        while (it != commands.end()) {
                get_asm_commands_aux(c_table, *it, vm, asm_commands);
                ++it;
        }
}

commands ASMCommands::resolve_placeholder(const command_table &c_table, const VMCommand &vm, const std::string &s)
{
        assert(is_placeholder(s));
        size_t delim_start = s.find("{{");
        size_t delim_end = s.find("}}");
        std::string placeholder = s.substr(delim_start + 2, delim_end - (delim_start + 2));

        if (placeholder == "segment") {
                command_table::const_iterator segment = m_asm_rules.find(to_brackets(vm.arg1()));
                if (segment == m_asm_rules.end()) {
                        std::domain_error("could not resolve arg: " + vm.arg1());
                }

                return segment->second;
        }

        if (placeholder == "index") {
                std::string seg = vm.arg1();
                std::string i = vm.arg2();
                if (!is_valid_index(seg, i)) {
                        std::domain_error("invalid index: " + i);
                }

                std::string index = s;
                index.insert(delim_start, i);
                delim_start = delim_start + i.length();
                delim_end = delim_end + i.length();
                index.erase(delim_start, (delim_end + 2) - delim_start);

                return { index };
        }

        if (placeholder == "uuid") {
                std::string uuid = s;
                uuid.insert(delim_start, m_uuid);
                delim_start = delim_start + m_uuid.length();
                delim_end = delim_end + m_uuid.length();
                uuid.erase(delim_start, (delim_end + 2) - delim_start);

                return { uuid };
        }

        return std::vector<std::string>();
}

command_table ASMCommands::init_rules()
{
        return {
                // MEMORY ACCESS
                {"<argument>", 		{"<constant>", "@ARG", "D=M+D", "@R13", "AM=D", "D=M"}},
                {"<local>", 		{"<constant>", "@LCL", "D=M+D", "@R13", "AM=D", "D=M"}},
                {"<static>", 		{"@R{{index}}"}},
                {"<constant>", 		{"@{{index}}", "D=A"}},
                {"<this>", 		{"<constant>", "@THIS", "D=M+D", "@R13", "AM=D", "D=M"}},
                {"<that>", 		{"<constant>", "@THAT", "D=M+D", "@R13", "AM=D", "D=M"}},
                {"<pointer>", 		{"<constant>", "@R3", "D=A+D", "@R13", "AM=D", "D=M"}},
                {"<temp>", 		{"<constant>", "@R5", "D=A+D", "@R13", "AM=D", "D=M"}},
                {"<move>", 		{"{{segment}}"}},
                {"<push-stack>", 	{"@SP", "A=M", "M=D"}},
                {"<pop-stack>", 	{"@SP", "A=M", "D=M", "@R13", "A=M", "M=D"}},
                {"<increment>", 	{"@SP", "AM=M+1"}},
                {"<decrement>", 	{"@SP", "AM=M-1"}},
                {"<double-dec>", 	{"@SP", "M=M-1", "AM=M-1"}},

                // ===== C_ARITHMETIC =====
                {"<compute-eq>", 	{"<compute-sub>",
                                         "@eqtrue__{{uuid}}",
                                         "D;JEQ",
                                         "@SP",
                                         "A=M",
                                         "M=0",
                                         "@eqend__{{uuid}}",
                                         "0;JMP",
                                         "(eqtrue__{{uuid}})",
                                         "@SP",
                                         "A=M",
                                         "M=-1",
                                         "(eqend__{{uuid}})"}},

                {"<compute-gt>", 	{"<compute-sub>",
                                         "@gttrue__{{uuid}}",
                                         "D;JGT",
                                         "@SP",
                                         "A=M",
                                         "M=0",
                                         "@gtend__{{uuid}}",
                                         "0;JMP",
                                         "(gttrue__{{uuid}})",
                                         "@SP",
                                         "A=M",
                                         "M=-1",
                                         "(gtend__{{uuid}})"}},

                {"<compute-lt>", 	{"<compute-sub>",
                                         "@lttrue__{{uuid}}",
                                         "D;JLT",
                                         "@SP",
                                         "A=M",
                                         "M=0",
                                         "@ltend__{{uuid}}",
                                         "0;JMP",
                                         "(lttrue__{{uuid}})",
                                         "@SP",
                                         "A=M",
                                         "M=-1",
                                         "(ltend__{{uuid}})"}},
                {"<compute-and>", 	{"MD=M&D"}},
                {"<compute-or>", 	{"MD=M|D"}},
                {"<compute-not>", 	{"MD=!D"}},
                {"<compute-add>", 	{"MD=M+D"}},
                {"<compute-sub>", 	{"MD=M-D"}},
                {"<compute-neg>", 	{"MD=-D"}},

                // ===== VM COMMANDS =====
                {"<push>", 		{"<move>", "<push-stack>", "<increment>"}},
                {"<pop>", 		{"<decrement>", "<move>", "<pop-stack>"}},

                {"<add>", 		{"<double-dec>", "<compute-add>", "<increment>"}},
                {"<sub>", 		{"<double-dec>", "<compute-sub>", "<increment>"}},
                {"<eq>", 		{"<double-dec>", "<compute-eq>", "<increment>"}},
                {"<gt>", 		{"<double-dec>", "<compute-gt>", "<increment>"}},
                {"<lt>", 		{"<double-dec>", "<compute-lt>", "<increment>"}},
                {"<or>", 		{"<double-dec>", "<compute-or>", "<increment>"}},
                {"<and>", 		{"<double-dec>", "<compute-and>", "<increment>"}},
                {"<neg>", 		{"<decrement>", "<compute-neg>", "<increment>"}},
                {"<not>", 		{"<decrement>", "<compute-not>", "<increment>"}},
        };
}

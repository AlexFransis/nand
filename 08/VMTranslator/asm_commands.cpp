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
        return s.size() > 1 && *(s.begin()) == '<' && *(--(s.end())) == '>';
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

std::string ASMCommands::create_uuid() const
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

ASMCommands::ASMCommands(const std::string &filename)
        : m_asm_rules(init_rules()),
          m_uuid(create_uuid()),
          m_filename(filename),
          m_curr_func("NULL")
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
                commands::const_iterator it = resolved.begin();
                while (it != resolved.end()) {
                        get_asm_commands_aux(c_table, *it, vm, asm_commands);
                        ++it;
                }
                return;
        }

        command_table::const_iterator found = c_table.find(command);
        if (found == c_table.end()) {
                std::domain_error("[ERR] Invalid command: " + command);
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

        auto replace = [&delim_start, &delim_end] (const std::string &placeholder, const std::string &arg) -> std::string {
                std::string result = placeholder;
                result.insert(delim_start, arg);
                delim_start = delim_start + arg.length();
                delim_end = delim_end + arg.length();
                result.erase(delim_start, (delim_end + 2) - delim_start);

                return result;
        };

        if (placeholder == "segment") {
                command_table::const_iterator segment = m_asm_rules.find(to_brackets(vm.arg1()));
                if (segment == m_asm_rules.end()) {
                        std::domain_error("[ERR] Could not resolve arg: " + vm.arg1());
                }

                return segment->second;
        }

        if (placeholder == "index") {
                if (!is_valid_index(vm.arg1(), vm.arg2())) {
                        std::domain_error("[ERR] Invalid index: " + vm.arg2());
                }

                std::string index = replace(s, vm.arg2());

                return { index };
        }

        if (placeholder == "uuid") {
                std::string uuid = replace(s, m_uuid);

                return { uuid };
        }

        if (placeholder == "filename") {
                std::string filename = replace(s, m_filename);

                return { filename };
        }

        if (placeholder == "function") {
                std::string function = replace(s, m_curr_func);

                return { function };
        }

        if (placeholder == "label") {
                std::string label = replace(s, vm.arg1());

                return { label };
        }

        return std::vector<std::string>();
}

command_table ASMCommands::init_rules() const
{
        return {
                // ===== MEMORY ACCESS =================
                {"<push>", 		{"<move>", "<push-stack>", "<increment>"}},
                {"<pop>", 		{"<decrement>", "<move>", "<pop-stack>"}},

                // ===== C_ARITHMETIC ==================
                {"<add>", 		{"<double-dec>", "<compute-add>", "<increment>"}},
                {"<sub>", 		{"<double-dec>", "<compute-sub>", "<increment>"}},
                {"<eq>", 		{"<double-dec>", "<compute-eq>", "<increment>"}},
                {"<gt>", 		{"<double-dec>", "<compute-gt>", "<increment>"}},
                {"<lt>", 		{"<double-dec>", "<compute-lt>", "<increment>"}},
                {"<or>", 		{"<double-dec>", "<compute-or>", "<increment>"}},
                {"<and>", 		{"<double-dec>", "<compute-and>", "<increment>"}},
                {"<neg>", 		{"<decrement>", "<compute-neg>", "<increment>"}},
                {"<not>", 		{"<decrement>", "<compute-not>", "<increment>"}},

                // ===== PROGRAM FLOW ==================
                {"<label>", 		{"({{function}}${{label}})"}},
                {"<goto>", 		{"@{{function}}${{label}}", "0;JMP"}},
                {"<if-goto>", 		{"<decrement>", "D=M", "@{{function}}${{label}}", "D;JNE"}},

                // ===== FUNCTIONS =====================
                {"<function>", 		{}},
                {"<call>", 		{}},
                {"<return>", 		{}},

                // STACK OPERATIONS
                {"<move>", 		{"{{segment}}"}},
                {"<push-stack>", 	{"@SP", "A=M", "M=D"}},
                {"<pop-stack>", 	{"@SP", "A=M", "D=M", "@R13", "A=M", "M=D"}},
                {"<increment>", 	{"@SP", "AM=M+1"}},
                {"<decrement>", 	{"@SP", "AM=M-1"}},
                {"<double-dec>", 	{"@SP", "M=M-1", "AM=M-1"}},

                // SEGMENTS
                {"<static>", 		{"@{{filename}}.{{index}}", "D=A", "@R13", "AM=D", "D=M"}},
                {"<constant>", 		{"@{{index}}", "D=A"}},
                {"<argument>", 		{"<constant>", "@ARG", "D=M+D", "@R13", "AM=D", "D=M"}},
                {"<local>", 		{"<constant>", "@LCL", "D=M+D", "@R13", "AM=D", "D=M"}},
                {"<this>", 		{"<constant>", "@THIS", "D=M+D", "@R13", "AM=D", "D=M"}},
                {"<that>", 		{"<constant>", "@THAT", "D=M+D", "@R13", "AM=D", "D=M"}},
                {"<pointer>", 		{"<constant>", "@R3", "D=A+D", "@R13", "AM=D", "D=M"}},
                {"<temp>", 		{"<constant>", "@R5", "D=A+D", "@R13", "AM=D", "D=M"}},

                // COMPUTATIONS
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
                {"<compute-neg>", 	{"MD=-D"}}
        };
}

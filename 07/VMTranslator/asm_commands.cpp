#include <algorithm>
#include <cctype>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include "asm_commands.h"
#include "vm_command.h"

typedef std::vector<std::string> commands;
typedef std::unordered_map<std::string, commands> command_table;

bool ASMCommands::is_bracketed(const std::string &s)
{
        return s.size() > 1 && *(s.begin()) == '<' && *--(s.end()) == '>';
}

std::string to_brackets(const std::string &s)
{
        return "<" + s + ">";
}

ASMCommands::ASMCommands()
        : m_asm_rules(init_rules())
{
}

std::list<std::string> ASMCommands::get_asm_commands(const VMCommand &command)
{
        std::list<std::string> asm_commands;

        get_asm_commands_aux(m_asm_rules, to_brackets(command.command()), asm_commands);
        resolve_placeholders(m_asm_rules, command, asm_commands);

        return asm_commands;
}

void ASMCommands::get_asm_commands_aux(const command_table &c_table, const std::string &command,
                                       std::list<std::string> &asm_commands)
{
        if (!is_bracketed(command)) {
                asm_commands.push_back(command);
                return;
        }

        command_table::const_iterator found = c_table.find(command);
        if (found == c_table.end()) {
                std::domain_error("Invalid command: " + command);
        }

        commands commands = found->second;
        commands::const_iterator it = commands.begin();

        while (it != commands.end()) {
                get_asm_commands_aux(c_table, *it, asm_commands);
                ++it;
        }
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
        char uuid[16];
        for (int i = 0; i < 16; i++) {
                uuid[i] = alphanumeric[std::rand() % (alphanumeric.length() - 1)];
        }

        return std::string(uuid);
}

void ASMCommands::resolve_placeholders(const command_table &c_table, const VMCommand &command,
                                       std::list<std::string> &asm_commands)
{
        std::string uuid = get_uuid();
        std::list<std::string>::iterator it = asm_commands.begin();
        while (it != asm_commands.end()) {
                size_t delim_start = it->find("{{");
                if (delim_start != std::string::npos) {
                        size_t delim_end = it->find("}}");
                        if (delim_end == std::string::npos) {
                                std::domain_error("could not find placeholder delimiters");
                        }

                        std::string placeholder = it->substr(delim_start + 2, delim_end - (delim_start + 2));
                        if (placeholder == "segment") {
                                command_table::const_iterator segment = m_asm_rules.find(to_brackets(command.arg1()));
                                if (segment == m_asm_rules.end()) {
                                        std::domain_error("could not resolve arg: " + command.arg1());
                                }

                                it = asm_commands.erase(it++);
                                asm_commands.insert(it, segment->second.begin(), segment->second.end());

                                // check if index needs to be resolved from any of the inserted commands
                                std::vector<std::string>::size_type i = 0;
                                for (; i < segment->second.size(); ++i) {
                                        --it;
                                }

                                continue;
                        }

                        if (placeholder == "index") {
                                std::string index = command.arg2();
                                if (!is_valid_index(command.arg1(), index)) {
                                        std::domain_error("invalid index: " + index);
                                }

                                it->insert(delim_start, index);
                                delim_start = delim_start + index.length();
                                delim_end = delim_end + index.length();
                                it->erase(delim_start, (delim_end + 2) - delim_start);
                        }

                        if (placeholder == "uuid") {
                                it->insert(delim_start, uuid);
                                delim_start = delim_start + uuid.length();
                                delim_end = delim_end + uuid.length();
                                it->erase(delim_start, (delim_end + 2) - delim_start);
                        }
                }

                ++it;
        }
}

command_table ASMCommands::init_rules()
{
        return {
                // MEMORY ACCESS
                {"<argument>", 		{"@ARG"}},
                {"<local>", 		{"@LCL"}},
                {"<static>", 		{"@R{{index}}"}},
                {"<constant>", 		{"@{{index}}", "D=A"}},
                {"<this>", 		{"@THIS"}},
                {"<that>", 		{"@THAT"}},
                {"<pointer>", 		{"<this>", "<that>"}},
                {"<temp>", 		{"@R{{index}}"}},
                {"<move>", 		{"@SP", "A=M", "M=D"}},
                {"<store>", 		{"{{segment}}"}},
                {"<load>", 		{"M=D"}},
                {"<increment>", 	{"@SP", "AM=M+1"}},
                {"<decrement>", 	{"@SP", "AM=M-1"}},
                {"<double-dec>", 	{"@SP", "M=M-1", "AM=M-1"}},

                // ===== C_ARITHMETIC =====
                {"<compute-eq>", 	{"<compute-sub>",
                                         "@eqtrue__{{uuid}}",
                                         "D;JEQ",
                                         "MD=0",
                                         "@eqend__{{uuid}}",
                                         "0;JMP",
                                         "(eqtrue__{{uuid}})",
                                         "@SP",
                                         "A=M",
                                         "MD=-1",
                                         "(eqend__{{uuid}})"}},

                {"<compute-gt>", 	{"<compute-sub>",
                                         "@gttrue__{{uuid}}",
                                         "D;JLT",
                                         "MD=0",
                                         "@gtend__{{uuid}}",
                                         "0;JMP",
                                         "(gttrue__{{uuid}})",
                                         "@SP",
                                         "A=M",
                                         "MD=-1",
                                         "(gtend__{{uuid}})"}},

                {"<compute-lt>", 	{"<compute-sub>",
                                         "@lttrue__{{uuid}}",
                                         "D;JGT",
                                         "MD=0",
                                         "@ltend__{{uuid}}",
                                         "0;JMP",
                                         "(lttrue__{{uuid}})",
                                         "@SP",
                                         "A=M",
                                         "MD=-1",
                                         "(ltend__{{uuid}})"}},
                {"<compute-and>", 	{"MD=M&D"}},
                {"<compute-or>", 	{"MD=M|D"}},
                {"<compute-not>", 	{"MD=!D"}},
                {"<compute-add>", 	{"MD=M+D"}},
                {"<compute-sub>", 	{"MD=M-D"}},
                {"<compute-neg>", 	{"MD=-D"}},

                // ===== VM COMMANDS =====
                {"<push>", 		{"<store>", "<move>", "<increment>"}},
                {"<pop>", 		{"<decrement>", "<move>"}},

                {"<add>", 		{"<double-dec>", "<compute-add>", "<increment>"}},
                {"<sub>", 		{"<decrement>", "<decrement>", "<compute-sub>", "<increment>"}},
                {"<eq>", 		{"<decrement>", "<decrement>", "<compute-eq>", "<increment>"}},
                {"<gt>", 		{"<decrement>", "<decrement>", "<compute-gt>", "<increment>"}},
                {"<lt>", 		{"<decrement>", "<decrement>", "<compute-lt>", "<increment>"}},
                {"<or>", 		{"<decrement>", "<decrement>", "<compute-or>", "<increment>"}},
                {"<and>", 		{"<decrement>", "<decrement>", "<compute-and>", "<increment>"}},
                {"<neg>", 		{"<decrement>", "<compute-neg>", "<increment>"}},
                {"<not>", 		{"<decrement>", "<compute-not>", "<increment>"}},
        };
}

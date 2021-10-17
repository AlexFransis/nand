#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cassert>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include "command_translator.h"

bool CommandTranslator::is_bracketed(const std::string &s)
{
        return s.size() > 1 && *(s.begin()) == '<' && *(--(s.end())) == '>';
}

bool CommandTranslator::is_placeholder(const std::string &s)
{
        size_t delim_start = s.find("{{");
        size_t delim_end = s.find("}}");
        return delim_start != std::string::npos && delim_end != std::string::npos;
}

bool CommandTranslator::is_valid_index(const std::string &segment, const std::string &index)
{
        std::string::const_iterator it = index.begin();
        while (it != index.end()) {
                if (!std::isdigit(*it)) return false;
                ++it;
        }

        int idx = std::stoi(index);
        if (idx < 0) return false;
        if (segment == "constant") return idx <= 32767;
        if (segment == "pointer") return idx <= 1;
        if (segment == "temp") return idx <= 7;

        return true;
}

std::string to_brackets(const std::string &s)
{
        return "<" + s + ">";
}

std::string CommandTranslator::generate_uuid() const
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

CommandTranslator::CommandTranslator(const std::string &filename)
        : m_instr_table(init_instr_table()),
          m_uuid(std::string()),
          m_filename(filename),
          m_curr_func("NULL")
{
}

std::list<std::string> CommandTranslator::translate_command(const Command &command)
{
        std::list<std::string> asm_instrs;
        m_uuid = generate_uuid();
        translate_command_aux(to_brackets(command.name()), command, asm_instrs);

        return asm_instrs;
}

// TODO: Make this function TCO
void CommandTranslator::translate_command_aux(const std::string &command, const Command &vm, std::list<std::string> &asm_instrs)
{
        if (!is_bracketed(command) && !is_placeholder(command)) {
                asm_instrs.push_back(command);
                return;
        }

        if (is_placeholder(command)) {
                instrs resolved = resolve_placeholder(command, vm);
                instrs::const_iterator it = resolved.begin();
                while (it != resolved.end()) {
                        translate_command_aux(*it, vm, asm_instrs);
                        ++it;
                }
                return;
        }

        instr_table::const_iterator found = m_instr_table.find(command);
        if (found == m_instr_table.end()) {
                std::domain_error("[ERR] Invalid command: " + command);
        }

        instrs commands = found->second;
        instrs::iterator it = commands.begin();
        while (it != commands.end()) {
                translate_command_aux(*it, vm, asm_instrs);
                ++it;
        }
}

void CommandTranslator::replace(size_t delim_start, size_t delim_end, std::string &placeholder, const std::string &arg)
{
        placeholder.insert(delim_start, arg);
        delim_start = delim_start + arg.length();
        delim_end = delim_end + arg.length();
        placeholder.erase(delim_start, (delim_end + 2) - delim_start);
}

std::vector<std::string> CommandTranslator::resolve_placeholder(const std::string &s, const Command &vm)
{
        assert(is_placeholder(s));
        std::string resolved = s;
        size_t delim_start = resolved.find("{{");
        while (delim_start != std::string::npos) {
                size_t delim_end = resolved.find("}}");
                if (delim_end == std::string::npos) {
                        std::domain_error("[ERR] Invalid placeholder: " + s);
                }

                std::string placeholder = resolved.substr(delim_start + 2, delim_end - (delim_start + 2));

                if (placeholder == "segment") {
                        instr_table::const_iterator segment = m_instr_table.find(to_brackets(vm.arg1()));
                        if (segment == m_instr_table.end()) {
                                std::domain_error("[ERR] Could not resolve arg: " + vm.arg1());
                        }

                        return segment->second;
                }

                if (placeholder == "index") {
                        if (!is_valid_index(vm.arg1(), vm.arg2())) {
                                std::domain_error("[ERR] Invalid index: " + vm.arg2());
                        }
                        replace(delim_start, delim_end, resolved, vm.arg2());
                }

                if (placeholder == "uuid") {
                        replace(delim_start, delim_end, resolved, m_uuid);
                }

                if (placeholder == "filename") {
                        replace(delim_start, delim_end, resolved, m_filename);
                }

                if (placeholder == "function") {
                        replace(delim_start, delim_end, resolved, m_curr_func);
                }

                if (placeholder == "label") {
                        replace(delim_start, delim_end, resolved, vm.arg1());
                }

                delim_start = resolved.find("{{");
        }

        return { resolved };
}

instr_table CommandTranslator::init_instr_table() const
{
        return {
                // ===== MEMORY ACCESS =================
                {"<push>", 		{"{{segment}}", "<push-stack>", "<increment>"}},
                {"<pop>", 		{"<decrement>", "{{segment}}", "<pop-stack>"}},

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

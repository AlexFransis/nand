#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "instr_mapper.h"

bool InstructionMapper::is_bracketed(const std::string &s)
{
        return s.size() > 1 && *(s.begin()) == '<' && *(--(s.end())) == '>';
}

bool InstructionMapper::is_placeholder(const std::string &s)
{
        size_t delim_start = s.find("{{");
        size_t delim_end = s.find("}}");
        return delim_start != std::string::npos && delim_end != std::string::npos;
}

bool InstructionMapper::is_number(const std::string &s)
{
        std::string::const_iterator it = s.begin();
        while (it != s.end()) {
                if (!std::isdigit(*it)) return false;
                ++it;
        }

        return true;
}

bool InstructionMapper::is_valid_index(const std::string &segment, const std::string &index)
{
        if (!is_number(index)) return false;
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

void InstructionMapper::replace(size_t delim_start, size_t delim_end, std::string &placeholder, const std::string &arg)
{
        placeholder.insert(delim_start, arg);
        delim_start = delim_start + arg.length();
        delim_end = delim_end + arg.length();
        placeholder.erase(delim_start, (delim_end + 2) - delim_start);
}

std::vector<std::string> InstructionMapper::get_bootstrap_instrs() const
{
        instr_table::const_iterator found = m_instr_table.find("<bootstrap>");
        assert(found != m_instr_table.end());
        return found->second;
}

std::list<std::string> InstructionMapper::map_command(const Command &command, std::unordered_map<std::string, std::string> &state)
{
        std::list<std::string> asm_instrs;
        map_command_aux(to_brackets(command.name), command, state, asm_instrs);

        return asm_instrs;
}

void InstructionMapper::map_command_aux(const std::string &command,
                                        const Command &vm,
                                        std::unordered_map<std::string, std::string> &state,
                                        std::list<std::string> &asm_instrs)
{
        // TODO: Make this function TCO
        if (!is_bracketed(command) && !is_placeholder(command)) {
                asm_instrs.push_back(command);
                return;
        }

        if (is_placeholder(command)) {
                std::vector<std::string> resolved = resolve_placeholder(command, vm, state);
                std::vector<std::string>::const_iterator it = resolved.begin();
                while (it != resolved.end()) {
                        map_command_aux(*it, vm, state, asm_instrs);
                        ++it;
                }
                return;
        }

        instr_table::const_iterator found = m_instr_table.find(command);
        if (found == m_instr_table.end()) {
                std::domain_error("[ERR] Invalid command: " + command);
        }

        std::vector<std::string> commands = found->second;
        std::vector<std::string>::iterator it = commands.begin();
        while (it != commands.end()) {
                map_command_aux(*it, vm, state, asm_instrs);
                ++it;
        }
}

std::vector<std::string> InstructionMapper::resolve_placeholder(const std::string &s,
                                                                const Command &vm,
                                                                std::unordered_map<std::string, std::string> &state)
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

                if (placeholder == "func_args") {
                        if (!is_number(vm.arg2())) {
                                std::domain_error("[ERR] Invalid number of arguments: " + vm.arg2());
                        }
                        replace(delim_start, delim_end, resolved, vm.arg2());
                }

                if (placeholder == "func_locals") {
                        if (!is_number(vm.arg2())) {
                                std::domain_error("[ERR] Invalid number of locals: " + vm.arg2());
                        }
                        replace(delim_start, delim_end, resolved, vm.arg2());
                }

                if (placeholder == "uuid") {
                        replace(delim_start, delim_end, resolved, state["uuid"]);
                }

                if (placeholder == "filename") {
                        replace(delim_start, delim_end, resolved, state["filename"]);
                }

                if (placeholder == "function_scope") {
                        replace(delim_start, delim_end, resolved, state["function_scope"]);
                }

                if (placeholder == "func_name") {
                        replace(delim_start, delim_end, resolved, vm.arg1());
                }

                if (placeholder == "label") {
                        replace(delim_start, delim_end, resolved, vm.arg1());
                }

                delim_start = resolved.find("{{");
        }

        return { resolved };
}

/*
  TODO: optimizations:
  - remove unnecessary SP manipulation in case of a push instr followed by a pop instr
 */
const instr_table InstructionMapper::m_instr_table =
{
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
        {"<label>", 		{"({{function_scope}}${{label}})"}},
        {"<goto>", 		{"@{{function_scope}}${{label}}", "0;JMP"}},
        {"<if-goto>", 		{"<decrement>", "D=M", "@{{function_scope}}${{label}}", "D;JNE"}},

        // ===== FUNCTIONS =====================
        {"<function>", 		{"({{func_name}})", 		// declare function label for entry
                                 "@i", 				// init loop variable
                                 "M=0",
                                 "(LOOP__{{uuid}})",
                                 "<func-locals>",		// repeat n times (n = nb of local variables)
                                 "@i",
                                 "D=D-M",
                                 "@END__{{uuid}}",
                                 "D;JEQ",
                                 "D=0",				// init local vars to 0 and push
                                 "<push-stack>",
                                 "<increment>",
                                 "@i",
                                 "M=M+1",
                                 "@LOOP__{{uuid}}",
                                 "0;JMP",
                                 "(END__{{uuid}})"}},

        {"<call>", 		{"@RETURN__{{uuid}}", 		// push return address
                                 "D=A",
                                 "<push-stack>",
                                 "<increment>",
                                 "@LCL", 			// push LCL address of calling function
                                 "D=M",
                                 "<push-stack>",
                                 "<increment>",
                                 "@ARG", 			// push ARG address of calling function
                                 "D=M",
                                 "<push-stack>",
                                 "<increment>",
                                 "@THIS", 			// push THIS address of calling function
                                 "D=M",
                                 "<push-stack>",
                                 "<increment>",
                                 "@THAT", 			// push THAT address of calling function
                                 "D=M",
                                 "<push-stack>",
                                 "<increment>",
                                 "@SP", 			// reposition ARG = SP - n - 5 (n = func args)
                                 "D=M",
                                 "@5",
                                 "D=D-A",
                                 "@{{func_args}}",
                                 "D=D-A",
                                 "@ARG",
                                 "M=D",
                                 "@SP", 			// reposition LCL = SP
                                 "D=M",
                                 "@LCL",
                                 "M=D",
                                 "@{{func_name}}", 		// transfer control to function
                                 "0;JMP",
                                 "(RETURN__{{uuid}})"}},	// label for return address declared

        {"<return>", 		{"@LCL",
                                 "D=M",
                                 "@FRAME",			// FRAME = LCL
                                 "M=D",
                                 "@5",
                                 "D=A",
                                 "@FRAME",
                                 "AM=M-D",
                                 "D=M",
                                 "@RET",			// RET = *(FRAME - 5)
                                 "M=D",
                                 "<decrement>",
                                 "D=M",				// get return value
                                 "@ARG",
                                 "A=M",
                                 "M=D",				// reposition return value for caller *ARG = pop()
                                 "@ARG",
                                 "D=M",
                                 "@SP",				// restore SP to the caller SP = ARG+1
                                 "M=D+1",
                                 "@FRAME",
                                 "AM=M+1",
                                 "D=M",
                                 "@LCL",			// LCL = *(FRAME - 4)
                                 "M=D",
                                 "@FRAME",
                                 "AM=M+1",
                                 "D=M",
                                 "@ARG",			// ARG = *(FRAME - 3)
                                 "M=D",
                                 "@FRAME",
                                 "AM=M+1",
                                 "D=M",
                                 "@THIS",			// THIS = *(FRAME - 2)
                                 "M=D",
                                 "@FRAME",
                                 "AM=M+1",
                                 "D=M",
                                 "@THAT",			// THAT = *(FRAME - 1)
                                 "M=D",
                                 "@RET",
                                 "A=M",
                                 "0;JMP"}},			// goto RET

        // BOOTSTRAP CODE
        {"<bootstrap>", 	{"@256", "D=A", "@SP", "M=D", "@Sys.init", "0;JMP"}},

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


        // CONSTANTS
        {"<func-locals>", 	{"@{{func_locals}}", "D=A"}},

        // COMPUTATIONS
        {"<compute-eq>", 	{"<compute-sub>",
                                 "@EQTRUE__{{uuid}}",
                                 "D;JEQ",
                                 "@SP",
                                 "A=M",
                                 "M=0",
                                 "@EQEND__{{uuid}}",
                                 "0;JMP",
                                 "(EQTRUE__{{uuid}})",
                                 "@SP",
                                 "A=M",
                                 "M=-1",
                                 "(EQEND__{{uuid}})"}},

        {"<compute-gt>", 	{"<compute-sub>",
                                 "@GTTRUE__{{uuid}}",
                                 "D;JGT",
                                 "@SP",
                                 "A=M",
                                 "M=0",
                                 "@GTEND__{{uuid}}",
                                 "0;JMP",
                                 "(GTTRUE__{{uuid}})",
                                 "@SP",
                                 "A=M",
                                 "M=-1",
                                 "(GTEND__{{uuid}})"}},

        {"<compute-lt>", 	{"<compute-sub>",
                                 "@LTTRUE__{{uuid}}",
                                 "D;JLT",
                                 "@SP",
                                 "A=M",
                                 "M=0",
                                 "@LTEND__{{uuid}}",
                                 "0;JMP",
                                 "(LTTRUE__{{uuid}})",
                                 "@SP",
                                 "A=M",
                                 "M=-1",
                                 "(LTEND__{{uuid}})"}},

        {"<compute-and>", 	{"MD=M&D"}},
        {"<compute-or>", 	{"MD=M|D"}},
        {"<compute-not>", 	{"MD=!D"}},
        {"<compute-add>", 	{"MD=M+D"}},
        {"<compute-sub>", 	{"MD=M-D"}},
        {"<compute-neg>", 	{"MD=-D"}}
};

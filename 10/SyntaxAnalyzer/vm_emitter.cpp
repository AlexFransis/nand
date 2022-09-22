#include "vm_emitter.h"
#include <cassert>
#include <string>


std::string format_push_pop(const PUSH_POP &push_pop, const SEGMENT &segment, int i)
{
        std::string result;

        switch (push_pop) {
        case PUSH_POP::PUSH :
                result += "push ";
                break;
        case PUSH_POP::POP :
                result += "pop ";
                break;
        default:
                break;
        }

        switch (segment) {
        case SEGMENT::ARGUMENT:
                result += "argument ";
                break;
        case SEGMENT::LOCAL:
                result += "local ";
                break;
        case SEGMENT::STATIC:
                result += "static ";
                break;
        case SEGMENT::CONSTANT:
                result += "constant ";
                break;
        case SEGMENT::THIS:
                result += "this ";
                break;
        case SEGMENT::THAT:
                result += "that ";
                break;
        case SEGMENT::POINTER:
                result += "pointer ";
                break;
        case SEGMENT::TEMP:
                result += "temp ";
                break;
        default:
                break;
        }

        result += std::to_string(i);

        return result;
}

std::string format_function(const std::string &name, int n_locals)
{
        return "function " + name + " " + std::to_string(n_locals);
}

void VMEmitter::emit_push(const SEGMENT &segment, int i, std::vector<std::string> &out_vm_commands)
{
        out_vm_commands.push_back(format_push_pop(PUSH_POP::PUSH, segment, i));
}

void VMEmitter::emit_pop(const SEGMENT &segment, int i, std::vector<std::string> &out_vm_commands)
{
        out_vm_commands.push_back(format_push_pop(PUSH_POP::POP, segment, i));
}

void VMEmitter::emit_label(const std::string &label, std::vector<std::string> &out_vm_commands)
{
        out_vm_commands.push_back("@" + label);
}

void VMEmitter::emit_function(const std::string &name, int n_locals, std::vector<std::string> &out_vm_commands)
{
        out_vm_commands.push_back(format_function(name, n_locals));
}

void VMEmitter::emit_arithmetic(const COMMAND &command, std::vector<std::string> &out_vm_commands)
{
        switch (command) {
        case COMMAND::ADD :
                out_vm_commands.push_back("add");
        case COMMAND::SUB :
                out_vm_commands.push_back("sub");
        case COMMAND::NEG :
                out_vm_commands.push_back("neg");
        case COMMAND::EQ :
                out_vm_commands.push_back("eq");
        case COMMAND::LT :
                out_vm_commands.push_back("lt");
        case COMMAND::GT :
                out_vm_commands.push_back("gt");
        case COMMAND::OR :
                out_vm_commands.push_back("or");
        case COMMAND::NOT :
                out_vm_commands.push_back("not");
                break;
        default:
                break;
        }
}

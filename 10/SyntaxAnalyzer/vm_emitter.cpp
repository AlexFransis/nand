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

std::string format_call(const std::string &name, int n_args)
{
        return "call " + name + " " + std::to_string(n_args);
}

void VMEmitter::emit_push(const SEGMENT &segment, int i, std::vector<std::string> &out_vm_commands)
{
        out_vm_commands.push_back(format_push_pop(PUSH_POP::PUSH, segment, i));
}

void VMEmitter::emit_push(const SCOPE &scope, int i, std::vector<std::string> &out_vm_commands)
{
        if (scope == SCOPE::VAR || scope == SCOPE::FIELD) {
                emit_push(SEGMENT::LOCAL, i, out_vm_commands);

        }

        if (scope == SCOPE::STATIC) {
                emit_push(SEGMENT::STATIC, i, out_vm_commands);
        }
}


void VMEmitter::emit_pop(const SEGMENT &segment, int i, std::vector<std::string> &out_vm_commands)
{
        out_vm_commands.push_back(format_push_pop(PUSH_POP::POP, segment, i));
}

void VMEmitter::emit_pop(const SCOPE &scope, int i, std::vector<std::string> &out_vm_commands)
{
        if (scope == SCOPE::VAR || scope == SCOPE::FIELD) {
                emit_pop(SEGMENT::LOCAL, i, out_vm_commands);

        }

        if (scope == SCOPE::STATIC) {
                emit_pop(SEGMENT::STATIC, i, out_vm_commands);
        }
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
                break;
        case COMMAND::SUB :
                out_vm_commands.push_back("sub");
                break;
        case COMMAND::NEG :
                out_vm_commands.push_back("neg");
                break;
        case COMMAND::EQ :
                out_vm_commands.push_back("eq");
                break;
        case COMMAND::LT :
                out_vm_commands.push_back("lt");
                break;
        case COMMAND::GT :
                out_vm_commands.push_back("gt");
                break;
        case COMMAND::OR :
                out_vm_commands.push_back("or");
                break;
        case COMMAND::NOT :
                out_vm_commands.push_back("not");
                break;
        default:
                break;
        }
}

void VMEmitter::emit_call(const std::string &name, int n_args, std::vector<std::string> &out_vm_commands)
{
        out_vm_commands.push_back(format_call(name, n_args));
}

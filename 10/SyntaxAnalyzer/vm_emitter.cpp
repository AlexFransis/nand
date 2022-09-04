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

void VMEmitter::emit_push(const SEGMENT &segment, int i, std::vector<std::string> &out_vm_commands)
{
        out_vm_commands.push_back(format_push_pop(PUSH_POP::PUSH, segment, i));
}

void VMEmitter::emit_pop(const SEGMENT &segment, int i, std::vector<std::string> &out_vm_commands)
{
        out_vm_commands.push_back(format_push_pop(PUSH_POP::POP, segment, i));
}

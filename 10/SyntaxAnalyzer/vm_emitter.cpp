#include "vm_emitter.h"
#include <cassert>
#include <string>

const std::string PUSH = "push";
const std::string POP = "pop";

const std::string ARGUMENT = "argument";
const std::string LOCAL = "local";
const std::string STATIC = "static";
const std::string CONSTANT = "constant";
const std::string THIS = "this";
const std::string THAT = "that";
const std::string POINTER = "pointer";
const std::string TEMP = "temp";

const std::string ADD = "add";
const std::string SUB = "sub";
const std::string NEG = "neg";
const std::string EQ = "eq";
const std::string GT = "gt";
const std::string LT = "lt";
const std::string AND = "and";
const std::string OR = "or";
const std::string NOT = "not";


std::string format_push_pop(const PUSH_POP &push_pop, const SEGMENT &segment, int i)
{
        std::string result;

        if (push_pop == PUSH_POP::PUSH) result += "push ";
        if (push_pop == PUSH_POP::POP) result += "pop ";

        if (segment == SEGMENT::ARGUMENT) result += "argument ";
        if (segment == SEGMENT::LOCAL) result += "local ";
        if (segment == SEGMENT::STATIC) result += "static ";
        if (segment == SEGMENT::CONSTANT) result += "constant ";
        if (segment == SEGMENT::THIS) result += "this ";
        if (segment == SEGMENT::THAT) result += "that ";
        if (segment == SEGMENT::POINTER) result += "pointer ";
        if (segment == SEGMENT::TEMP) result += "temp ";

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

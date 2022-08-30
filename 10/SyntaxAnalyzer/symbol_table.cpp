#include "symbol_table.h"
#include <utility>

SymbolTable::SymbolTable()
{
        class_scope = std::unordered_map<std::string, const Symbol>();
        subroutine_scope = std::unordered_map<std::string, const Symbol>();
        class_name = std::string();
        subroutine_name = std::string();
}

void SymbolTable::record_symbol(Symbol &s)
{
        if (s.kind == "static") {
                s.index = static_count;
                ++static_count;
                class_scope.insert(std::make_pair(s.name, s));
        }
        if (s.kind == "field") {
                s.index = field_count;
                ++field_count;
                class_scope.insert(std::make_pair(s.name, s));
        }
        if (s.kind == "var") {
                s.index = var_count;
                ++var_count;
                subroutine_scope.insert(std::make_pair(s.name, s));
        }
        if (s.kind == "arg") {
                s.index = arg_count;
                ++arg_count;
                subroutine_scope.insert(std::make_pair(s.name, s));
        }

}

void SymbolTable::define_symbol(const std::string &name, const std::string &type, const std::string &kind)
{
        Symbol s { name, type, kind };
        record_symbol(s);
}

void SymbolTable::define_symbol(Symbol &s)
{
        record_symbol(s);
}

void SymbolTable::begin_subroutine(const std::string &sname)
{
        subroutine_name = sname;
        subroutine_scope.clear();
        arg_count = 0;
        var_count = 0;
}

void SymbolTable::begin_class(const std::string &cname)
{
        class_name = cname;
        class_scope.clear();
        field_count = 0;
        static_count = 0;
}

int SymbolTable::count_kind(const std::string &kind)
{
        if (kind == "static") return static_count;
        if (kind == "field") return field_count;
        if (kind == "var") return var_count;
        if (kind == "arg") return arg_count;

        return -1;
}

std::string SymbolTable::kind_of(const std::string &name)
{
        std::unordered_map<std::string, const Symbol>::const_iterator found = subroutine_scope.find(name);
        if (found != subroutine_scope.end()) {
                return found->second.kind;
        }

        found = class_scope.find(name);
        if (found != class_scope.end()) {
                return found->second.kind;
        }

        return "none";
}

std::string SymbolTable::type_of(const std::string &name)
{
        std::unordered_map<std::string, const Symbol>::const_iterator found = subroutine_scope.find(name);
        if (found != subroutine_scope.end()) {
                return found->second.type;
        }

        found = class_scope.find(name);
        if (found != class_scope.end()) {
                return found->second.type;
        }

        return "none";
}

int SymbolTable::index_of(const std::string &name)
{
        std::unordered_map<std::string, const Symbol>::const_iterator found = subroutine_scope.find(name);
        if (found != subroutine_scope.end()) {
                return found->second.index;
        }

        found = class_scope.find(name);
        if (found != class_scope.end()) {
                return found->second.index;
        }

        return -1;
}

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
        if (s.scope == SCOPE::STATIC) {
                s.index = static_count;
                ++static_count;
                class_scope.insert(std::make_pair(s.name, s));
        }

        if (s.scope == SCOPE::FIELD) {
                s.index = field_count;
                ++field_count;
                class_scope.insert(std::make_pair(s.name, s));
        }

        if (s.scope == SCOPE::VAR) {
                s.index = var_count;
                ++var_count;
                subroutine_scope.insert(std::make_pair(s.name, s));
        }

        if (s.scope == SCOPE::ARG) {
                s.index = arg_count;
                ++arg_count;
                subroutine_scope.insert(std::make_pair(s.name, s));
        }
}

void SymbolTable::define_symbol(const std::string &name, const std::string &type, SCOPE kind)
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

int SymbolTable::count_kind(SCOPE kind)
{
        if (kind == SCOPE::STATIC) return static_count;
        if (kind == SCOPE::FIELD) return field_count;
        if (kind == SCOPE::VAR) return var_count;
        if (kind == SCOPE::ARG) return arg_count;

        return -1;
}

SCOPE SymbolTable::kind_of(const std::string &name)
{
        std::unordered_map<std::string, const Symbol>::const_iterator found = subroutine_scope.find(name);
        if (found != subroutine_scope.end()) {
                return found->second.scope;
        }

        found = class_scope.find(name);
        if (found != class_scope.end()) {
                return found->second.scope;
        }

        return SCOPE::UNKNOWN;
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

bool SymbolTable::try_get(const std::string &name, Symbol *symbol)
{
        std::unordered_map<std::string, const Symbol>::const_iterator found = subroutine_scope.find(name);
        if (found != subroutine_scope.end()) {
                *symbol = found->second;
                return true;
        }

        found = class_scope.find(name);
        if (found != class_scope.end()) {
                *symbol = found->second;
                return true;
        }

        return false;
}

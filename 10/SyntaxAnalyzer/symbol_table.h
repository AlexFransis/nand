#ifndef GUARD_symbol_table_h
#define GUARD_symbol_table_h

#include <string>
#include <unordered_map>

struct Symbol {
        std::string name;
        std::string type;
        std::string kind;
        int index;
};

class SymbolTable {
private:
        std::unordered_map<std::string, const Symbol> class_scope;
        std::unordered_map<std::string, const Symbol> subroutine_scope;
        int static_count;
        int field_count;
        int arg_count;
        int var_count;

public:
        SymbolTable();

        void begin_subroutine();
        Symbol define_symbol(const std::string &name, const std::string &type, const std::string &kind);
        int count_kind(const std::string &kind);
        std::string kind_of(const std::string &name);
        std::string type_of(const std::string &name);
        int index_of(const std::string &name);

};

#endif

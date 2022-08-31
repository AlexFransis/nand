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
        int static_count;
        int field_count;
        int arg_count;
        int var_count;

        void record_symbol(Symbol &s);
        std::unordered_map<std::string, const Symbol> class_scope;
        std::unordered_map<std::string, const Symbol> subroutine_scope;

public:
        std::string class_name;
        std::string subroutine_name;

        SymbolTable();

        void begin_subroutine(const std::string &subroutine_name);
        void begin_class(const std::string &class_name);
        void define_symbol(const std::string &name, const std::string &type, const std::string &kind);
        void define_symbol(Symbol &s);
        int count_kind(const std::string &kind);
        std::string kind_of(const std::string &name);
        std::string type_of(const std::string &name);
        int index_of(const std::string &name);
        inline std::unordered_map<std::string, const Symbol> view_class_scope() const { return class_scope; }
        inline std::unordered_map<std::string, const Symbol> view_subroutine_scope() const { return subroutine_scope; }

};

#endif

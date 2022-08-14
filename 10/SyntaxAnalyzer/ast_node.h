#ifndef GUARD_ast_node_h
#define GUARD_ast_node_h

#include <string>
#include <vector>
#include <memory>

struct AstNode {
        std::string type;
        std::string terminal_value;
        std::vector<std::unique_ptr<AstNode>> children;
};

#endif

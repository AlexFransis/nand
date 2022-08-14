#ifndef GUARD_file_writer_h
#define GUARD_file_writer_h

#include <memory>
#include <fstream>
#include "ast_node.h"


class FileWriter {

public:
        FileWriter();
        void 					write_xml(const std::unique_ptr<AstNode> &ast, std::ofstream &ofstream);
};

#endif

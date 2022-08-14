#include <iostream>
#include "analyzer.h"
#include "ast_node.h"
#include "file_reader.h"
#include "file_writer.h"

namespace fs = std::filesystem;

int main(int argc, char** argv)
{
        if (argc != 3) {
                std::cerr << "Usage:\n";
                std::cerr << "\t" << *argv << " -f <filename>\n";
                std::cerr << "\t" << *argv << " -d <dir>\n";
                return 1;
        }

        std::string input_type = *(argv + 1);
        std::string input_path = *(argv + 2);

        if (input_type != "-f" && input_type != "-d") {
                std::cerr << "Usage:\n";
                std::cerr << "\t" << *argv << " -f <filename>\n";
                std::cerr << "\t" << *argv << " -d <dir>\n";
                return 1;
        }

        try {
                FileReader reader;
                std::vector<io_paths> paths = reader.get_io_paths(input_path, input_type);
                std::vector<io_paths>::const_iterator it = paths.begin();

                while (it != paths.end()) {
                        fs::path jack_file = it->first;
                        fs::path xml_file = it->second;
                        std::ifstream ifstream;
                        std::ofstream ofstream;

                        std::cout << "[INFO] Compiling file: " << std::string(jack_file) << std::endl;

                        ifstream.open(jack_file);
                        if (!ifstream.good()) {
                                std::string err = "[ERR] Could not open file: " + std::string(jack_file);
                                throw std::domain_error(err);
                        }

                        Analyzer a;
                        std::unique_ptr<AstNode> ast = a.analyze(ifstream);

                        std::cout << "[INFO] Opening output file: " << std::string(xml_file) << std::endl;
                        ofstream.open(xml_file);
                        if (!ofstream.good()) {
                                std::string err = "[ERR] Could not open file: " + std::string(xml_file);
                                throw std::domain_error(err);
                        }

                        FileWriter writer;
                        writer.write_xml(ast, ofstream);

                        ++it;
                }

        } catch (const std::domain_error &err) {
                std::cerr << err.what() << std::endl;
                return 1;
        }

        std::cout << "[INFO] Compilation completed" << std::endl;

        return 0;
}

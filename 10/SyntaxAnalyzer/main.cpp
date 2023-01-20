#include <filesystem>
#include <iostream>
#include <string>
#include "analyzer.h"
#include "ast_node.h"
#include "file_reader.h"
#include "file_writer.h"

namespace fs = std::filesystem;

int main(int argc, char** argv)
{
        if (argc < 2) {
                std::cerr << "Usage:\n";
                std::cerr << "\t" << *argv << " $filename | $dir\n";
                return 1;
        }

        std::string input_path = *(argv + 1);
        std::string xml_out = std::string();
        if (argc == 3) {
                xml_out = *(argv + 2);
        }

        try {
                FileReader reader (input_path);
                std::vector<fs::path> paths = reader.get_jack_files();
                std::vector<fs::path>::const_iterator it = paths.cbegin();

                while (it != paths.end()) {
                        fs::path jack_file = *it;
                        std::ifstream ifstream;
                        std::ofstream ofstream;

                        std::cout << "[INFO] Compiling file: " << std::string(jack_file) << std::endl;
                        ifstream.open(jack_file);
                        if (!ifstream.good()) {
                                std::string err = "[ERR] Could not open file: " + std::string(jack_file);
                                throw std::domain_error(err);
                        }

                        Analyzer a;
                        FileWriter writer(jack_file);
                        if (!xml_out.empty() && xml_out == "--xml-out") {
                                std::unique_ptr<AstNode> ast = a.compile(ifstream);
                                writer.write_xml(ast);

                        } else {
                                std::vector<std::string> vm_code = a.compile_vm_code(ifstream);
                                writer.write_vm_commands(vm_code);
                        }

                        ++it;
                }

        } catch (const std::domain_error &err) {
                std::cerr << err.what() << std::endl;
                return 1;
        }

        std::cout << "[INFO] Compilation completed" << std::endl;

        return 0;
}

#include <iostream>
#include <list>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <cstdio>
#include "file_handler.h"

namespace fs = std::filesystem;

FileHandler::FileHandler(const std::string &path, INPUT_TYPE input)
        :m_input_path(fs::path(path)),
         m_input_type(input)
{
}

std::pair<std::vector<fs::path>, fs::path> FileHandler::get_io_paths()
{
        std::string out_ext = ".asm";
        std::string in_ext = ".vm";

        std::vector<fs::path> input_files = scan_files(m_input_type, in_ext);
        fs::path output_file = construct_output_file(m_input_type, out_ext);

        return std::make_pair(input_files, output_file);
}

std::vector<fs::path> FileHandler::scan_files(INPUT_TYPE input, const std::string &ext)
{
        std::vector<fs::path> result;
        if (m_input_type == INPUT_TYPE::DIR) {
                if (!fs::exists(m_input_path) || !fs::is_directory(m_input_path)) {
                        throw std::domain_error("[ERR] Invalid directory path");
                }

                std::vector<fs::path> files = traverse_dir(m_input_path, ext);
                if (files.size() == 0) {
                        throw std::domain_error("[ERR] No valid files in directory");
                }

                std::copy(files.begin(), files.end(), std::back_inserter(result));
        }

        if (m_input_type == INPUT_TYPE::FILE) {
                if (!fs::exists(m_input_path) || m_input_path.extension() != ext) {
                        throw std::domain_error("[ERR] Invalid file path");
                }

                result.push_back(m_input_path);
        }

        return result;
}

fs::path FileHandler::construct_output_file(INPUT_TYPE input, const std::string &ext)
{
        fs::path out;
        if (m_input_type == INPUT_TYPE::FILE) {
                out = m_input_path.replace_extension(ext);
        }

        if (m_input_type == INPUT_TYPE::DIR) {
                // create a file in the directory passed
                out = m_input_path / m_input_path.stem().replace_extension(ext);
        }

        return out;
}

std::vector<fs::path> FileHandler::traverse_dir(const fs::path &dir, const std::string &ext)
{
        std::vector<fs::path> result;
        for (const fs::directory_entry &e : fs::recursive_directory_iterator(dir)) {
                fs::path file_p = e.path();
                if (file_p.extension() == ext) {
                        result.push_back(file_p);
                }
        }

        return result;
}

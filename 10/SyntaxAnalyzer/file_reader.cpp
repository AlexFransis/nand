#include "file_reader.h"

namespace fs = std::filesystem;


FileReader::FileReader()
{
        m_ifstream = std::ifstream();
        m_ofstream = std::ofstream();
}

std::vector<io_paths> FileReader::get_io_paths(const std::filesystem::path &input_path, const std::string &type)
{
        std::string out_ext = ".xml";
        std::string in_ext = ".jack";

        std::vector<fs::path> input_files = scan_files(type, input_path, in_ext);

        return construct_output_files(input_files, out_ext);
}

std::vector<fs::path> FileReader::scan_files(const std::string &type, const fs::path &input_path, const std::string &ext)
{
        std::vector<fs::path> result;
        if (type == "-d") {
                if (!fs::exists(input_path) || !fs::is_directory(input_path)) {
                        throw std::domain_error("[ERR] Invalid directory path");
                }

                std::vector<fs::path> files = traverse_dir(input_path, ext);
                if (files.size() == 0) {
                        throw std::domain_error("[ERR] No valid files in directory");
                }

                std::copy(files.begin(), files.end(), std::back_inserter(result));
        }

        if (type == "-f") {
                if (!fs::exists(input_path) || input_path.extension() != ext) {
                        throw std::domain_error("[ERR] Invalid file path");
                }

                result.push_back(input_path);
        }

        return result;
}

std::vector<io_paths> FileReader::construct_output_files(std::vector<fs::path> &inputs, const std::string &ext)
{
        std::vector<io_paths> result;
        std::vector<fs::path>::const_iterator it = inputs.cbegin();
        while (it != inputs.end()) {
                fs::path out = *it;
                out.replace_extension(ext);
                result.push_back(std::make_pair(*it, out)); // input output
                it++;
        }

        return result;
}

std::vector<fs::path> FileReader::traverse_dir(const fs::path &dir, const std::string &ext)
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

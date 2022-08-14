#ifndef GUARD_file_reader_h
#define GUARD_file_reader_h

#include <filesystem>
#include <string>
#include <vector>
#include <fstream>

typedef std::pair<std::filesystem::path, std::filesystem::path> io_paths;

class FileReader {
private:
        std::vector<std::filesystem::path>	traverse_dir(const std::filesystem::path &dir, const std::string &ext);
        std::vector<std::filesystem::path>	scan_files(const std::filesystem::path &input_path, const std::string &ext);
        std::vector<io_paths>			construct_output_files(std::vector<std::filesystem::path> &inputs, const std::string &ext);

public:
        FileReader();
        std::vector<io_paths> 			get_io_paths(const std::filesystem::path &input_path);
};

#endif

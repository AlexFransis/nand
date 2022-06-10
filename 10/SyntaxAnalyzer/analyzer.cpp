#include <iostream>
#include <string>
#include <unordered_map>
#include "file_handler.h"
#include "analyzer.h"
#include "tokenizer.h"
#include "parser.h"


namespace fs = std::filesystem;

Analyzer::Analyzer(const std::string &path, const std::string &input)
        :m_input_path(fs::path(path))
{
        m_ifstream = std::ifstream();
        m_ofstream = std::ofstream();
        m_input_type = input == "-d" ? INPUT_TYPE::DIR : INPUT_TYPE::FILE;
}

Analyzer::~Analyzer()
{
        m_ifstream.close();
        m_ofstream.close();
}

std::string ltrim(const std::string &s)
{
	const std::string ws = " \n\r\t\f\v";
	size_t start = s.find_first_not_of(ws);
	return (start == std::string::npos) ? std::string() : s.substr(start);
}

std::string rtrim(const std::string &s)
{
	const std::string ws = " \n\r\t\f\v";
	size_t end = s.find_last_not_of(ws);
	return (end == std::string::npos) ? std::string() : s.substr(0, end + 1);
}

std::string Analyzer::trim_comments(const std::string &line)
{
	std::size_t pos_begin = line.find("//");
        if (pos_begin != std::string::npos) {
                return line.substr(0, pos_begin);
        }

        pos_begin = line.find("/*");
        if (pos_begin != std::string::npos) {
                std::size_t pos_end = line.find("*/");
                if (pos_end == std::string::npos) {
                }

        }

	return (pos_begin == std::string::npos) ? line : line.substr(0, pos_begin);
}

std::string Analyzer::trim_ws(const std::string &line)
{
	return rtrim(ltrim(line));
}

bool Analyzer::try_write_xml(const std::vector<std::pair<std::string, std::string>> &tokens, std::ofstream &ofstream)
{
        if (!ofstream.good()) {
                return false;
        }

        std::unordered_map<std::string, std::string> token_map {
                {"SYMBOL" , "symbol"},
                {"IDENTIFIER" , "identifier"},
                {"KEYWORD" , "keyword"},
                {"STRING_CONST" , "stringConstant"},
                {"INTEGER_CONST" , "integerConstant"},
        };

        std::unordered_map<std::string, std::string> xml_special_chars {
                {"<", "&lt;"},
                {">", "&gt;"},
                {"&", "&amp;"},
                {"\"", "&quot;"},
                {"\'", "&apos;"},
        };

        ofstream << "<tokens>\n";
        for (const std::pair<std::string, std::string> &token : tokens) {
                std::string element = token_map[token.second];
                auto search = xml_special_chars.find(token.first);
                ofstream << "<" << element << "> ";
                ofstream << (search != xml_special_chars.end() ? search->second : token.first);
                ofstream << " </" << element << ">\n";
        }
        ofstream << "</tokens>\n";

        return true;
}

void Analyzer::begin()
{
        FileHandler fh;
        m_io_paths = fh.get_io_paths(m_input_path, m_input_type);
        std::vector<io_paths>::const_iterator it = m_io_paths.begin();
        while (it != m_io_paths.end()) {
                fs::path jack_file = it->first;
                fs::path xml_file = it->second;

                m_ifstream.open(jack_file);
                m_ofstream.open(xml_file);

                if (!m_ifstream.good()) {
                        std::string err = "[ERR] Could not open file: " + std::string(jack_file);
                        throw std::domain_error(err);
                }
                std::cout << "[INFO] Opening input file: " << std::string(jack_file) << std::endl;

                if (!m_ofstream.good()) {
                        std::string err = "[ERR] Could not open file: " + std::string(xml_file);
                        throw std::domain_error(err);
                }
                std::cout << "[INFO] Opening output file: " << std::string(xml_file) << std::endl;

                Tokenizer t;
                int line_number = 0;
                tokens tokens;
                while (!m_ifstream.eof()) {
                        line_number++;
                        std::string current_line;
                        std::getline(m_ifstream, current_line);
                        std::string trimmed_comments = trim_comments(current_line);
                        std::string trimmed_ws = trim_ws(trimmed_comments);

                        if (!t.try_tokenize(trimmed_ws, tokens)) {
                                std::string err = "[ERR] Invalid syntax on line " + std::to_string(line_number) +
                                        " in file: " + std::string(jack_file);
                                throw std::domain_error(err);
                        }
                }

                if (!try_write_xml(tokens, m_ofstream)) {
                        std::string err = "Could not write XML file";
                        throw std::domain_error(err);
                }

                m_ifstream.close();
                m_ofstream.close();

                it++;
        }
}

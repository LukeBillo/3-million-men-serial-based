#pragma once
#include <string>
#include <list>
#include <fstream>
#include <map>

class three_million_men_reader
{
public:
	three_million_men_reader(const std::string& file_name);
	~three_million_men_reader() = default;
	
	void read_all_lines();
	std::shared_ptr<std::map<std::string, std::string>> get_lines();
private:
	std::shared_ptr<std::map<std::string, std::string>> _name_pair_map;
	std::ifstream input_file;
};


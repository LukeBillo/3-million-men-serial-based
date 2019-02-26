#pragma once
#include <string>
#include <list>
#include <fstream>
#include <map>
#include "ThreeMillionMenSerial.h"

class three_million_men_reader
{
public:
	using name_map = three_million_men_serial::name_map;

	three_million_men_reader(const std::string& file_name);
	~three_million_men_reader() = default;

	void read_all_lines();
	std::unique_ptr<name_map> get_lines();
private:
	std::unique_ptr<name_map> _name_pair_map;
	std::ifstream input_file;
};


#include <sstream>
#include "ThreeMillionMenReader.h"

three_million_men_reader::three_million_men_reader(const std::string& file_name)
{
	input_file = std::ifstream(file_name, std::ios::in);

	if (!input_file.good())
	{
		throw "There was an error with the file provided.";
	}
}

void three_million_men_reader::read_all_lines()
{
	_name_pair_map = std::make_unique<name_map>();
	std::string line;

	while(std::getline(input_file, line))
	{
		std::stringstream stream(line);
		std::string current_name;
		std::string neighbour_name;

		std::getline(stream, current_name, ',');
		std::getline(stream, neighbour_name, ',');

		_name_pair_map->insert({ current_name, neighbour_name });
	}
}

std::unique_ptr<three_million_men_reader::name_map> three_million_men_reader::get_lines()
{
	return std::move(_name_pair_map);
}
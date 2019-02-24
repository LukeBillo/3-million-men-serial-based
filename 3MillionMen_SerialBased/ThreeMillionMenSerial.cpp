#include "ThreeMillionMenSerial.h"

three_million_men_serial::three_million_men_serial(std::shared_ptr<name_map>& names)
{
	_names_by_key = names;
	_results = std::make_unique<name_list>();
}

void three_million_men_serial::run()
{
	auto names_by_value = std::make_unique<name_map>();
	auto matched_pairs = std::make_unique<name_uo_map>();

	for (auto i = _names_by_key->begin(); i != _names_by_key->end(); ++i)
	{
		names_by_value->operator[](i->second) = i->first;
	}

	for (auto i = _names_by_key->begin(); i != _names_by_key->end(); ++i)
	{
		for (auto j = names_by_value->begin(); j != names_by_value->end(); ++j)
		{
			if (i->second == j->first)
			{
				matched_pairs->insert({i->first, j->second});
				_names_by_key->erase(i);
			}	
		}
	}
}

std::list<std::string>* three_million_men_serial::get_results()
{
}

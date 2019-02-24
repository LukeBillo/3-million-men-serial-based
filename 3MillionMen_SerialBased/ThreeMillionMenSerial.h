#pragma once
#include <memory>
#include <map>
#include <list>
#include <unordered_map>

class three_million_men_serial
{
public:
	using name_list = std::list<std::string>;
	using name_map = std::map<std::string, std::string>;
	using name_uo_map = std::unordered_map<std::string, std::string>;

	three_million_men_serial(std::shared_ptr<name_map>&);
	~three_million_men_serial() = default;

	void run();
	name_list* get_results();

private:
	std::shared_ptr<name_map> _names_by_key;
	std::unique_ptr<name_list> _results;
};


#pragma once
#include <memory>
#include <map>
#include <list>
#include <unordered_map>

class three_million_men_serial
{
public:
	using name_list = std::list<std::string>;
	using result_list = std::list<std::pair<int, std::string>>;
	using name_map = std::map<std::string, std::string>;
	using name_vector = std::vector<std::pair<std::string, std::string>>;

	three_million_men_serial(std::shared_ptr<name_map>&);
	~three_million_men_serial() = default;

	void run();
	name_list* get_results();

private:
	std::shared_ptr<name_map> _names_by_key;
	std::unique_ptr<result_list> _results;

	struct pass_result
	{
		std::shared_ptr<name_vector> matches;
		std::shared_ptr<name_vector> non_matches;

		pass_result(std::shared_ptr<name_vector>& matches, std::shared_ptr<name_vector>& non_matches) : matches(matches), non_matches(non_matches) {}
	};

	pass_result& match_pairs_recursive(name_vector::iterator name_by_key_it,
		name_vector::iterator name_by_value_it,
		std::shared_ptr<name_vector>& names_by_key,
		std::shared_ptr<name_vector>& names_by_value,
		std::shared_ptr<name_vector>& matches,
		std::shared_ptr<name_vector>& non_matches);

	bool is_last(std::pair<std::string, std::string> unmatched_pair) const;
};


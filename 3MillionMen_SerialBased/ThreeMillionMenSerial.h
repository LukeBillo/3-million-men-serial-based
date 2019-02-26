#pragma once
#include <memory>
#include <map>
#include <list>
#include <unordered_map>

class three_million_men_serial
{
public:
	using name_pair = std::pair<std::string, std::string>;
	using ordered_name = std::pair<int, std::string>;
	using name_list = std::list<std::string>;
	using output_list = std::list<ordered_name>;
	using name_map = std::map<std::string, std::string>;
	using name_vector = std::vector<name_pair>;

	three_million_men_serial(std::unique_ptr<name_map>&);
	~three_million_men_serial() = default;

	void run();
	name_list* get_results() const;

private:
	std::unique_ptr<name_map> _names_by_key;
	std::unique_ptr<output_list> _results;

	struct initial_result
	{
		std::unique_ptr<name_vector> matches;
		std::unique_ptr<name_vector> non_matches;

		initial_result(std::unique_ptr<name_vector>& matches, std::unique_ptr<name_vector>& non_matches)
		{
			this->matches = std::move(matches);
			this->non_matches = std::move(non_matches);
		}
	};

	std::unique_ptr<initial_result> match_pairs(
		name_vector::iterator name_by_key_it,
		name_vector::iterator name_by_value_it,
		std::unique_ptr<name_vector> names_by_key,
		std::unique_ptr<name_vector> names_by_value,
		std::unique_ptr<name_vector> matches,
		std::unique_ptr<name_vector> non_matches) const;

	void run_hardy_algorithm_inductive(
		std::unique_ptr<name_vector> matches,
		std::unique_ptr<output_list> results);

	std::unique_ptr<output_list> run_hardy_algorithm_inductive_recursive(
		std::unique_ptr<name_vector> matches_by_value,
		std::unique_ptr<output_list> outputs,
		std::unique_ptr<name_vector> matches_by_key,
		std::unique_ptr<name_vector> new_matches,
		std::unique_ptr<output_list> new_outputs,
		size_t offset) const;

	bool is_last(std::pair<std::string, std::string> unmatched_pair) const;
};


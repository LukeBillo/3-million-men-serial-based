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
	name_map* _names_by_key;
	output_list* _outputs;

	struct initial_pass_output
	{
		name_vector matches;
		name_vector non_matches;

		initial_pass_output(name_vector& matches, name_vector& non_matches)
		{
			this->matches = matches;
			this->non_matches = non_matches;
		}
	};

	static initial_pass_output* initial_pass_recursive(
		name_vector::iterator name_by_key_it,
		name_vector::iterator name_by_value_it,
		name_vector* names_by_key,
		name_vector* names_by_value,
		name_vector* matches,
		name_vector* non_matches);

	void run_hardy_algorithm_inductive(
		name_vector matches,
		output_list* outputs);

	output_list* run_hardy_algorithm_inductive_recursive(
		name_vector* matches_by_value,
		output_list* outputs,
		name_vector* matches_by_key,
		name_vector* new_matches,
		output_list* new_outputs,
		size_t offset) const;

	bool is_last(std::pair<std::string, std::string> unmatched_pair) const;
};


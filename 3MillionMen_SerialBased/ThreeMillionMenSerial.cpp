#include "ThreeMillionMenSerial.h"
#include <algorithm>

three_million_men_serial::three_million_men_serial(std::unique_ptr<name_map>& names)
{
	_names_by_key = std::move(names);
	_results = std::make_unique<output_list>();
}

void three_million_men_serial::run()
{
	auto names_by_key_vector = std::make_unique<name_vector>();

	for (auto& i : *_names_by_key)
	{
		names_by_key_vector->push_back({i.first, i.second });
	}

	auto names_by_value = std::make_unique<name_vector>(*names_by_key_vector);
	std::sort(names_by_value->begin(), names_by_value->end(), [](const name_pair& pair1, const name_pair& pair2)
	{
		return pair1.second < pair2.second;
	});

	auto matched_pairs = std::make_unique<name_vector>();
	auto unmatched_pairs = std::make_unique<name_vector>();

	const auto name_by_key_it = names_by_key_vector->begin();
	const auto name_by_value_it = names_by_value->begin();

	auto result = match_pairs(
		name_by_key_it, 
		name_by_value_it,
		std::move(names_by_key_vector),
		std::move(names_by_value),
		std::move(matched_pairs),
		std::move(unmatched_pairs));

	auto last_name = is_last(result->non_matches->front()) ? result->non_matches->front() : result->non_matches->back();
	auto size = _names_by_key->size();

	auto results = std::make_unique<output_list>();
	results->push_back({ size, last_name.second });
	results->push_back({ size - 1, last_name.first });

	run_hardy_algorithm_inductive(std::move(result->matches), std::move(results));
}

bool three_million_men_serial::is_last(const std::pair<std::string, std::string> unmatched_pair) const
{
	for (auto& i : *_names_by_key)
	{
		if (unmatched_pair.second == i.first)
		{
			return false;
		}
	}

	return true;
}

std::unique_ptr<three_million_men_serial::initial_result> three_million_men_serial::match_pairs(
	name_vector::iterator name_by_key_it,
	name_vector::iterator name_by_value_it,
	std::unique_ptr<name_vector> names_by_key,
	std::unique_ptr<name_vector> names_by_value,
	std::unique_ptr<name_vector> matches,
	std::unique_ptr<name_vector> non_matches) const
{
	if (name_by_key_it == names_by_key->end())
	{
		const auto last_unmatched_name_it = names_by_value->begin();
		non_matches->push_back({ last_unmatched_name_it->first, last_unmatched_name_it->second });

		return std::make_unique<initial_result>(matches, non_matches);
	}

	if (name_by_value_it == names_by_value->end())
	{
		non_matches->push_back({ name_by_key_it->first, name_by_key_it->second });
		const auto next_name_by_key = names_by_key->erase(name_by_key_it);
		const auto reset_name_by_value_it = names_by_value->begin();

		return match_pairs(
			next_name_by_key,
			reset_name_by_value_it, 
			std::move(names_by_key), 
			std::move(names_by_value),
			std::move(matches),
			std::move(non_matches));
	}

	if (name_by_key_it->second == name_by_value_it->first)
	{
		matches->push_back({ name_by_key_it->first, name_by_value_it->second });

		const auto next_name_by_key = names_by_key->erase(name_by_key_it);

		names_by_value->erase(name_by_value_it);
		const auto reset_name_by_value_it = names_by_value->begin();

		return match_pairs(
			next_name_by_key,
			reset_name_by_value_it,
			std::move(names_by_key),
			std::move(names_by_value),
			std::move(matches),
			std::move(non_matches));
	}

	const auto next_name_by_value = ++name_by_value_it;

	return match_pairs(
		name_by_key_it,
		next_name_by_value,
		std::move(names_by_key),
		std::move(names_by_value),
		std::move(matches),
		std::move(non_matches));
}

void three_million_men_serial::run_hardy_algorithm_inductive(
	std::unique_ptr<name_vector> matches,
	std::unique_ptr<output_list> results)
{
	results->sort([](ordered_name name1, ordered_name name2)
	{
		return name1.second < name2.second;
	});

	auto matches_by_value = std::make_unique<name_vector>(*matches);
	std::sort(matches_by_value->begin(), matches_by_value->end(), [](const name_pair& pair1, const name_pair& pair2)
	{
		return pair1.second < pair2.second;
	});

	auto new_matches = std::make_unique<name_vector>();
	auto new_outputs = std::make_unique<output_list>();

	auto outputs = run_hardy_algorithm_inductive_recursive(
		std::move(matches_by_value),
		std::move(results),
		std::move(matches),
		std::move(new_matches),
		std::move(new_outputs),
		2);

	outputs->sort([](const ordered_name& name1, const ordered_name& name2)
	{
		return name1.first < name2.first;
	});

	_results = std::move(outputs);
}

auto three_million_men_serial::run_hardy_algorithm_inductive_recursive(
	std::unique_ptr<name_vector> matches_by_value,
	std::unique_ptr<output_list> outputs,
	std::unique_ptr<name_vector> matches_by_key,
	std::unique_ptr<name_vector> new_matches,
	std::unique_ptr<output_list> new_outputs,
	const size_t offset) const -> std::unique_ptr<output_list>
{
	if (offset > _names_by_key->size() + 1)
	{
		return outputs;
	}

	// F
	auto matches_by_value_it = matches_by_value->begin();

	// G
	auto outputs_it = outputs->begin();

	// H
	auto matches_by_key_it = matches_by_key->begin();

	while (matches_by_value_it != matches_by_value->end())
	{
		if (matches_by_value_it->second == matches_by_key_it->first)
		{
			new_matches->push_back({ matches_by_value_it->first, matches_by_key_it->second });
			++matches_by_value_it;
			++matches_by_key_it;

			continue;
		}

		if (outputs_it != outputs->end() && matches_by_value_it->second == outputs_it->second)
		{
			new_outputs->push_back({ outputs_it->first - offset, matches_by_value_it->first });
			++matches_by_value_it;
			++outputs_it;

			continue;
		}

		if (outputs_it != outputs->end() && matches_by_value_it->second > outputs_it->second)
		{
			++outputs_it;
			continue;
		}

		if (matches_by_value_it->second > matches_by_key_it->first)
		{
			++matches_by_key_it;
		}
	}

	new_outputs->sort([](const ordered_name& name1, const ordered_name& name2)
	{
		return name1.second < name2.second;
	});

	outputs->merge(*new_outputs, [](const ordered_name& name1, const ordered_name& name2)
	{
		return name1.second < name2.second;

	});

	auto new_matches_by_key = std::make_unique<name_vector>(*new_matches);
	std::sort(new_matches_by_key->begin(), new_matches_by_key->end(), [](name_pair& pair1, name_pair& pair2)
	{
		return pair1.first < pair2.first;
	});

	std::sort(new_matches->begin(), new_matches->end(), [](name_pair& pair1, name_pair& pair2)
	{
		return pair1.second < pair2.second;
	});

	auto matches_for_next_pass = std::make_unique<name_vector>();
	auto outputs_for_next_pass = std::make_unique<output_list>();

	return run_hardy_algorithm_inductive_recursive(
		std::move(new_matches),
		std::move(outputs),
		std::move(new_matches_by_key),
		std::move(matches_for_next_pass),
		std::move(outputs_for_next_pass),
		offset * 2);
}

three_million_men_serial::name_list* three_million_men_serial::get_results() const
{
	auto results = new name_list();

	for (auto& i : *_results)
	{
		results->push_back(i.second);
	}

	return results;
}

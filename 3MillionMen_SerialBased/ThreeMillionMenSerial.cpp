#include "ThreeMillionMenSerial.h"
#include <algorithm>

three_million_men_serial::three_million_men_serial(std::unique_ptr<name_map>& names)
{
	_names_by_key = names.release();
	_outputs = new output_list();
}

void three_million_men_serial::run()
{
	auto* names_by_key_vector = new name_vector();

	for (auto& i : *_names_by_key)
	{
		names_by_key_vector->push_back({i.first, i.second });
	}

	auto* names_by_value = new name_vector(*names_by_key_vector);
	std::sort(names_by_value->begin(), names_by_value->end(), [](const name_pair& pair1, const name_pair& pair2)
	{
		return pair1.second < pair2.second;
	});

	const auto matched_pairs = new name_vector();
	const auto unmatched_pairs = new name_vector();

	const auto name_by_key_it = names_by_key_vector->begin();
	const auto name_by_value_it = names_by_value->begin();

	auto initial_pass_output = initial_pass_recursive(
		name_by_key_it, 
		name_by_value_it,
		names_by_key_vector,
		names_by_value,
		matched_pairs,
		unmatched_pairs);

	auto last_name = is_last(initial_pass_output->non_matches.front()) ? initial_pass_output->non_matches.front() : initial_pass_output->non_matches.back();
	auto size = _names_by_key->size();

	auto initial_outputs = new output_list();
	initial_outputs->push_back({ size, last_name.second });
	initial_outputs->push_back({ size - 1, last_name.first });

	run_hardy_algorithm_inductive(initial_pass_output->matches, initial_outputs);
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

three_million_men_serial::initial_pass_output* three_million_men_serial::initial_pass_recursive(
	name_vector::iterator name_by_key_it,
	name_vector::iterator name_by_value_it,
	name_vector* names_by_key,
	name_vector* names_by_value,
	name_vector* matches,
	name_vector* non_matches)
{
	if (name_by_key_it == names_by_key->end())
	{
		const auto last_unmatched_name_it = names_by_value->begin();

		non_matches->push_back({ last_unmatched_name_it->first, last_unmatched_name_it->second });

		delete names_by_key;
		delete names_by_value;

		return new initial_pass_output(*matches, *non_matches);
	}

	if (name_by_key_it->first == name_by_value_it->second)
	{
		matches->push_back({ name_by_value_it->first, name_by_key_it->second });

		auto next_name_by_value_it = name_by_value_it + 1;
		names_by_value->erase(name_by_value_it);

		++name_by_key_it;

		return initial_pass_recursive(
			name_by_key_it,
			name_by_value_it,
			names_by_key,
			names_by_value,
			matches,
			non_matches);
	}

	if (name_by_key_it->first < name_by_value_it->second)
	{
		non_matches->push_back({ name_by_key_it->first, name_by_key_it->second });
		const auto next_name_by_key = ++name_by_key_it;

		return initial_pass_recursive(
			next_name_by_key,
			name_by_value_it,
			names_by_key,
			names_by_value,
			matches,
			non_matches);
	}

	const auto next_name_by_value = ++name_by_value_it;

	return initial_pass_recursive(
		name_by_key_it,
		next_name_by_value,
		names_by_key,
		names_by_value,
		matches,
		non_matches);
}

void three_million_men_serial::run_hardy_algorithm_inductive(
	name_vector matches,
	output_list* outputs)
{
	outputs->sort([](const ordered_name name1, const ordered_name name2)
	{
		return name1.second < name2.second;
	});

	std::sort(matches.begin(), matches.end(), [](const name_pair& pair1, const name_pair& pair2)
	{
		return pair1.first < pair2.first;
	});

	auto matches_by_value = new name_vector(matches);
	std::sort(matches_by_value->begin(), matches_by_value->end(), [](const name_pair& pair1, const name_pair& pair2)
	{
		return pair1.second < pair2.second;
	});

	const auto new_matches = new name_vector();
	const auto new_outputs = new output_list();

	auto final_outputs = run_hardy_algorithm_inductive_recursive(
		matches_by_value,
		outputs,
		&matches,
		new_matches,
		new_outputs,
		2);

	final_outputs->sort([](const ordered_name& name1, const ordered_name& name2)
	{
		return name1.first < name2.first;
	});

	_outputs = final_outputs;
}

three_million_men_serial::output_list* three_million_men_serial::run_hardy_algorithm_inductive_recursive(
	name_vector* matches_by_value,
	output_list* outputs,
	name_vector* matches_by_key,
	name_vector* new_matches,
	output_list* new_outputs,
	const size_t offset) const
{
	if (offset > _names_by_key->size() + 1)
	{
		delete matches_by_value;
		delete matches_by_key;
		delete new_matches;
		delete new_outputs;

		matches_by_value = nullptr;
		matches_by_key = nullptr;
		new_matches = nullptr;
		new_outputs = nullptr;

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

	auto new_matches_by_key = new name_vector(*new_matches);
	std::sort(new_matches_by_key->begin(), new_matches_by_key->end(), [](name_pair& pair1, name_pair& pair2)
	{
		return pair1.first < pair2.first;
	});

	std::sort(new_matches->begin(), new_matches->end(), [](name_pair& pair1, name_pair& pair2)
	{
		return pair1.second < pair2.second;
	});

	const auto matches_for_next_pass = new name_vector();
	const auto outputs_for_next_pass = new output_list();

	return run_hardy_algorithm_inductive_recursive(
		new_matches,
		outputs,
		new_matches_by_key,
		matches_for_next_pass,
		outputs_for_next_pass,
		offset * 2);
}

three_million_men_serial::name_list* three_million_men_serial::get_results() const
{
	auto results = new name_list();

	for (auto& i : *_outputs)
	{
		results->push_back(i.second);
	}

	return results;
}

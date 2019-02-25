#include "ThreeMillionMenSerial.h"
#include <algorithm>

three_million_men_serial::three_million_men_serial(std::shared_ptr<name_map>& names)
{
	_names_by_key = names;
	_results = std::make_shared<result_list>();
}

void three_million_men_serial::run()
{
	auto names_by_key_copy = std::make_shared<name_vector>();

	for (auto i = _names_by_key->begin(); i != _names_by_key->end(); ++i)
	{
		names_by_key_copy->push_back({ i->first, i->second });
	}

	auto names_by_value = std::make_shared<name_vector>(*names_by_key_copy.get());
	std::sort(names_by_value->begin(), names_by_value->end(), [](const std::pair<std::string, std::string> &x, const std::pair<std::string, std::string> &y)
	{
		return x.second < y.second;
	});

	auto matched_pairs = std::make_shared<name_vector>();
	auto unmatched_pairs = std::make_shared<name_vector>();

	const auto name_by_key_it = names_by_key_copy->begin();
	const auto name_by_value_it = names_by_value->begin();

	auto result = match_pairs_recursive(name_by_key_it, name_by_value_it, names_by_key_copy, names_by_value, matched_pairs, unmatched_pairs);

	auto last_name = is_last(result.non_matches->front()) ? result.non_matches->front() : result.non_matches->back();
	int size = _names_by_key->size();

	_results->push_back({ size, last_name.second });
	_results->push_back({ size - 1, last_name.first });

	run_hardy_algorithm_inductive(result.matches, _results);
}

three_million_men_serial::pass_result three_million_men_serial::match_pairs_recursive(
	name_vector::iterator name_by_key_it,
	name_vector::iterator name_by_value_it,
	std::shared_ptr<name_vector>& names_by_key,
	std::shared_ptr<name_vector>& names_by_value,
	std::shared_ptr<name_vector>& matches,
	std::shared_ptr<name_vector>& non_matches)
{
	if (name_by_key_it == names_by_key->end())
	{
		const auto last_unmatched_name_it = names_by_value->begin();
		non_matches->push_back({ last_unmatched_name_it->first, last_unmatched_name_it->second });

		const auto result = pass_result(matches, non_matches);
		return result;
	}

	if (name_by_value_it == names_by_value->end())
	{
		non_matches->push_back({ name_by_key_it->first, name_by_key_it->second });
		const auto next_name_by_key = names_by_key->erase(name_by_key_it);
		const auto reset_name_by_value_it = names_by_value->begin();

		return match_pairs_recursive(next_name_by_key, reset_name_by_value_it, names_by_key, names_by_value, matches, non_matches);
;	}

	if (name_by_key_it->second == name_by_value_it->first)
	{
		matches->push_back({ name_by_key_it->first, name_by_value_it->second });

		const auto next_name_by_key = names_by_key->erase(name_by_key_it);

		names_by_value->erase(name_by_value_it);
		const auto reset_name_by_value_it = names_by_value->begin();

		return match_pairs_recursive(next_name_by_key, reset_name_by_value_it, names_by_key, names_by_value, matches, non_matches);
;	}

	const auto next_name_by_value = ++name_by_value_it;
	return match_pairs_recursive(name_by_key_it, next_name_by_value, names_by_key, names_by_value, matches, non_matches);
}

bool three_million_men_serial::is_last(const std::pair<std::string, std::string> unmatched_pair) const
{
	for (auto i = _names_by_key->begin(); i != _names_by_key->end(); ++i)
	{
		if (unmatched_pair.second == i->first)
		{
			return false;
		}
	}

	return true;
}

void three_million_men_serial::run_hardy_algorithm_inductive(
	std::shared_ptr<name_vector> matches,
	std::shared_ptr<result_list> results)
{
	const auto matches_copy = std::make_shared<name_vector>(*matches.get());
	run_hardy_algorithm_inductive_recursive(matches, results, matches_copy);
}

void three_million_men_serial::run_hardy_algorithm_inductive_recursive(
	std::shared_ptr<name_vector> matches,
	std::shared_ptr<result_list> results,
	std::shared_ptr<name_vector> matches_copy,
	)
{
	// matches
	auto matches_it = matches->begin();

	// non_matches
	auto non_matches_it = non_matches->begin();

	// H
	auto matches_copy_it = matches_copy->begin();

	while (matches_it != matches->end())
	{
		if (matches_it->second == matches_copy_it->first)
		{
			results->matches->push_back({ matches_it->first, matches_copy_it->second });
			++matches_it;
			++matches_copy_it;
		}

		if (matches_it->second == non_matches_it->second)
		{
			_results
			// matches++, non_matches++
			// (y - t, x)
		}

		if (matches_it->second > non_matches_it->second)
		{
			// non_matches++
		}

		if (matches_it->second > matches_copy_it->first)
		{
			// H++
		}
	}
}

std::list<std::string>* three_million_men_serial::get_results()
{
	return new std::list<std::string>();
}

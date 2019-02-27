#include <iostream>
#include "ThreeMillionMenReader.h"
#include "ThreeMillionMenSerial.h"
#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;

int main(int argc, const char* argv[])
{
	const high_resolution_clock::time_point algorithm_start = high_resolution_clock::now();

    if (argc <= 1)
    {
		throw "Usage: 3MillionMen_SerialBased [input_file] [OPTIONAL: output_file]";
    }

	
	const std::string input_file = argv[1];

	auto* reader = new three_million_men_reader(input_file);
	reader->read_all_lines();
	auto names = reader->get_lines();
	delete reader;

	const auto algorithm = new three_million_men_serial(names);
	algorithm->run();
	const auto results = algorithm->get_results();

	const high_resolution_clock::time_point algorithm_end = high_resolution_clock::now();

	const std::string output_file = argc > 2 ? argv[2] : "results.txt";
	std::ofstream output_file_stream(output_file);

	for (const auto& result : *results)
	{
		output_file_stream << result << std::endl;
	}

	delete results;
	delete algorithm;

	const auto algorithm_duration = duration_cast<microseconds>(algorithm_end - algorithm_start).count();

	std::cout << "Duration of search-based algorithm: " << algorithm_duration << std::endl;

	std::cout << "Press enter to exit." << std::endl;
	std::cin.ignore();

	return 0;
}

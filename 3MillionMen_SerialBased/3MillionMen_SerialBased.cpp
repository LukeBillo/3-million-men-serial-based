#include <iostream>
#include "ThreeMillionMenReader.h"
#include "ThreeMillionMenSerial.h"

int main(int argc, const char* argv[])
{
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

	const std::string output_file = argc > 2 ? argv[2] : "results.txt";
	std::ofstream output_file_stream(output_file);

	for (const auto& result : *results)
	{
		output_file_stream << result << std::endl;
	}

	delete results;
	delete algorithm;

	return 0;
}

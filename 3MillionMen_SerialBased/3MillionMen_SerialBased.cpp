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

	return 0;
}

// CSVBenchmark.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include "include/csv.hpp"
#include "minicsv.h"
#include "csv_stream.h"

using namespace csv;

class timer
{
public:
	timer() = default;
	void start(const std::string& text_)
	{
		text = text_;
		begin = std::chrono::high_resolution_clock::now();
	}
	void stop()
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto dur = end - begin;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		printf("%s timing:%dms\n", text.c_str(), ms);
	}

private:
	std::string text;
	std::chrono::high_resolution_clock::time_point begin;
};

int main()
{
	const int MAX_LOOP = 10;
	timer stopwatch;
	std::string csv_file = "d:\\example.csv";
	string_view sv;
	stopwatch.start("csv_parser");
	for (int a = 0; a < MAX_LOOP; ++a)
	{
		CSVReader reader(csv_file.c_str());
		for (CSVRow& row : reader) { // Input iterator
			for (CSVField& field : row) {
				// For efficiency, get<>() produces a string_view
				sv = field.get<>();
			}
		}
		// printing sv resulting in crash!
		//std::cout << sv << std::endl;
	}
	stopwatch.stop();

	std::string str;
	stopwatch.start("MiniCSV");
	for (int a = 0; a < MAX_LOOP; ++a)
	{
		mini::csv::ifstream is(csv_file.c_str());
		is.enable_trim_quote_on_str(false, '\"');
		is.set_delimiter(',', "$$");
		if (is.is_open())
		{
			while (is.read_line())
			{
				size_t total = is.num_of_delimiter() + 1;
				for (size_t i = 0; i < total; ++i)
					is >> str;
			}
			//std::cout << str << std::endl;
		}
	}
	stopwatch.stop();

	str = "";
	stopwatch.start("CSV Stream");
	for (int a = 0; a < MAX_LOOP; ++a)
	{
		capi::csv::ifstream is(csv_file.c_str());
		is.enable_trim_quote_on_str(false, '\"');
		is.set_delimiter(',', "$$");
		if (is.is_open())
		{
			while (is.read_line())
			{
				size_t total = is.num_of_delimiter() + 1;
				for (size_t i = 0; i < total; ++i)
					is >> str;
			}
			//std::cout << str << std::endl;
		}
	}
	stopwatch.stop();

	return 0;
}


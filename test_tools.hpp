#pragma once

#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <algorithm>

static const unsigned repetitions = 1000;
static const unsigned cycles = 100;


template<typename Fun>
void call_test(Fun& f)
{
	std::vector<unsigned> measurements;
	measurements.reserve(cycles);

	for(unsigned c = 0; c < cycles; c++)
	{
		auto start_time = std::chrono::steady_clock::now();
		for(unsigned r = 0; r < repetitions; r++)
		{
			f();
		}
		auto end_time = std::chrono::steady_clock::now();

		measurements.push_back((end_time - start_time)/std::chrono::nanoseconds(1));
	}

	std::sort(measurements.begin(), measurements.end());

	std::cout << measurements[0] << "\t" << measurements[cycles/2] << "\t" << measurements[cycles-1] << std::endl;
}

template<typename Fun>
void run_benchmark()
{
	Fun::test();

	Fun f;
	std::cout << std::setw(30) << typeid(Fun).name() << " : \t";
	call_test(f);
}

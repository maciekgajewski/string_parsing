#pragma once

#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <algorithm>

static const unsigned repetitions = 1000;
static const unsigned cycles = 100;
static int baseline = 0;


struct f_null
{
	static void test()
	{
	}

	bool operator()() const
	{
		return true;
	}
};

template<typename Fun>
void do_call_test(Fun& f, std::vector<int>& measurements)
{
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
}

static void calibrate()
{
	std::vector<int> measurements;
	measurements.reserve(cycles);

	f_null f;
	do_call_test(f, measurements);

	baseline = measurements[0];
}


template<typename Fun>
void call_test(Fun& f)
{
	std::vector<int> measurements;
	measurements.reserve(cycles);

	do_call_test(f, measurements);

	std::cout << measurements[0] - baseline << "\t" << measurements[cycles/2] - baseline  << "\t" << measurements[cycles-1] - baseline  << std::endl;
}

template<typename Fun>
void run_benchmark()
{
	Fun::test();

	Fun f;
	std::cout << std::setw(50) << typeid(Fun).name() << " : \t";
	call_test(f);
}

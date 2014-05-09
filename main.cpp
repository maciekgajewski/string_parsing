#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <cstdio>
#include <cstring>

#include <boost/regex.hpp>

static const unsigned repetitions = 1000;
static const unsigned cycles = 100;

const char* input = "ABC:Defghij";
char out1[4] = "";
char out2[16] = "";

struct f_null
{
	bool operator()() const
	{
		std::strcpy(out1, "ABC");
		std::strcpy(out2, "Defghij");
		return true;
	}
};

struct f_scanf
{
	bool operator()() const
	{
		return std::sscanf(input, "%3s:%16s", out1, out2) == 2;
	}
};

struct f_regexp_with_compile
{
	bool operator()()
	{
		boost::regex e("([[:alpha:]]{3}):([[:alpha:]]{1,16})");
		boost::match_results<const char*> mr;

		bool result = boost::regex_search(input, mr, e);
		if (result)
		{
			std::strcpy(out1, mr[1].str().c_str());
			std::strcpy(out2, mr[2].str().c_str());
		}
		return result;

	}
};

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
void test()
{
	Fun f;
	bool r = f();
	bool pass = (r == true && std::string(out1) == "ABC" && std::string(out2) == "Defghij");
	if (!pass)
	{
		std::cerr << typeid(Fun).name() << " fails. result=" << std::boolalpha << r << ", out1=" << out1 << ", out2= " << out2 << std::endl;
		throw std::logic_error("test failed");
	}

}

template<typename Fun>
void run()
{
	Fun f;
	std::cout << std::setw(30) << typeid(Fun).name() << " : \t";
	call_test(f);
}


int main(int, char**)
{
	test<f_null>();
	test<f_scanf>();
	test<f_regexp_with_compile>();

	run<f_null>();
	run<f_scanf>();
	run<f_regexp_with_compile>();
}

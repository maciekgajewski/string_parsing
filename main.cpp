#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <cstdio>
#include <cstring>

#include <boost/regex.hpp>
#include <boost/format.hpp>

static const unsigned repetitions = 1000;
static const unsigned cycles = 100;

template<typename Fun>
struct simple_cstr
{
	const char* m_input = "ABC:Defghij";
	char m_out1[4];
	char m_out2[16];

	simple_cstr() { m_out1[0] = 0; m_out2[0] = 0; }

	static void test()
	{
		Fun f;
		bool r = f();
		bool pass = (r == true && std::string(f.m_out1) == "ABC" && std::string(f.m_out2) == "Defghij");
		if (!pass)
		{
			throw std::logic_error(boost::str(boost::format("%1% test failed: result=%2%, out1=%3%, out2=%4%") % typeid(Fun).name() % r % f.m_out1 % f.m_out2));
		}
	}

};

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

struct f_scanf_cstr : public simple_cstr<f_scanf_cstr>
{
	bool operator()()
	{
		return std::sscanf(m_input, "%3s:%16s", m_out1, m_out2) == 2;
	}
};

struct f_regexp_with_compile_cstr : public simple_cstr<f_regexp_with_compile_cstr>
{
	bool operator()()
	{
		boost::regex e("([[:alpha:]]{3}):([[:alpha:]]{1,16})");
		boost::match_results<const char*> mr;

		bool result = boost::regex_search(m_input, mr, e);
		if (result)
		{
			std::strcpy(m_out1, mr[1].str().c_str());
			std::strcpy(m_out2, mr[2].str().c_str());
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
void run()
{
	Fun::test();

	Fun f;
	std::cout << std::setw(30) << typeid(Fun).name() << " : \t";
	call_test(f);
}


int main(int, char**)
{
	try
	{
		run<f_null>();
		run<f_scanf_cstr>();
		run<f_regexp_with_compile_cstr>();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

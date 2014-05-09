#pragma once

#include <stdexcept>

#include <cstdio>
#include <cstring>

#include <boost/regex.hpp>
#include <boost/format.hpp>

#include "test_tools.hpp"

// ==============================================
// simple string, output to char buffers

namespace simple_id_cstr
{

// base class and test
template<typename Fun>
struct base
{
	const char* m_input = "ABC:Defghij";
	char m_out1[4];
	char m_out2[16];

	base() { m_out1[0] = 0; m_out2[0] = 0; }

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

struct f_scanf : public base<f_scanf>
{
	bool operator()()
	{
		return std::sscanf(m_input, "%3s:%16s", m_out1, m_out2) == 2;
	}
};

struct f_regexp_with_compile : public base<f_regexp_with_compile>
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

struct f_regexp_no_compile : public base<f_regexp_with_compile>
{
	boost::regex m_e = boost::regex("([[:alpha:]]{3}):([[:alpha:]]{1,16})");

	bool operator()()
	{
		boost::match_results<const char*> mr;

		bool result = boost::regex_search(m_input, mr, m_e);
		if (result)
		{
			std::strcpy(m_out1, mr[1].str().c_str());
			std::strcpy(m_out2, mr[2].str().c_str());
		}
		return result;

	}
};

static void run()
{
	run_benchmark<f_scanf>();
	run_benchmark<f_regexp_with_compile>();
	run_benchmark<f_regexp_no_compile>();
}

}


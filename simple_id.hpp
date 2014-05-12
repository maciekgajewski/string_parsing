#pragma once

#include <stdexcept>

#include <cstdio>
#include <cstring>

#include <boost/regex.hpp>
#include <boost/format.hpp>

#include <boost/spirit/include/qi.hpp>

#include "test_tools.hpp"

// ==============================================
// simple string, output to char buffers

namespace simple_id
{

// base class and test
template<typename Fun>
struct base
{
	const std::string m_input = "ABC:Defghij";
	std::string m_out1;
	std::string m_out2;

	base()
	{
		m_out1.reserve(3);
		m_out2.reserve(16);
	}

	static void test()
	{
		Fun f;
		for(int i = 0; i < 10; i++)
		{
			bool r = f();
			bool pass = (r == true && f.m_out1 == "ABC" && f.m_out2 == "Defghij");
			if (!pass)
			{
				throw std::logic_error(boost::str(boost::format("%1% test failed: result=%2%, out1=%3%, out2=%4%") % typeid(Fun).name() % r % f.m_out1 % f.m_out2));
			}
		}
	}

};

struct f_manual : public base<f_manual>
{
	bool operator()()
	{
		if (m_input.length() > 4 && m_input[3] == ':')
		{
			m_out1.clear();
			m_out2.clear();

			std::copy(m_input.begin(), m_input.begin()+3, std::back_inserter(m_out1));
			std::copy(m_input.begin()+4, m_input.end(), std::back_inserter(m_out2));

			return true;
		}
		else
		{
			return false;
		}
	}
};

struct f_manual_substr : public base<f_manual>
{
	bool operator()()
	{
		if (m_input.length() > 4 && m_input[3] == ':')
		{
			m_out1 = m_input.substr(0, 3);
			m_out2 = m_input.substr(4);
			return true;
		}
		else
		{
			return false;
		}
	}
};

struct f_scanf : public base<f_scanf>
{
	bool operator()()
	{
		char out1[4] = "";
		char out2[16] = "";

		int res = std::sscanf(m_input.c_str(), "%3s:%16s", out1, out2);
		if (res == 2)
		{
			m_out1 = out1;
			m_out2 = out2;
			return true;
		}
		else
		{
			return false;
		}

	}
};

struct f_regexp_with_compile : public base<f_regexp_with_compile>
{
	bool operator()()
	{
		boost::regex e("([[:alpha:]]{3}):([[:alpha:]]{1,16})");
		boost::smatch mr;

		bool result = boost::regex_search(m_input, mr, e);
		if (result)
		{
			m_out1 =  mr[1].str();
			m_out2 =  mr[2].str();
		}
		return result;

	}
};

struct f_regexp_no_compile : public base<f_regexp_with_compile>
{
	boost::regex m_e = boost::regex("([[:alpha:]]{3}):([[:alpha:]]{1,16})");

	bool operator()()
	{
		boost::smatch mr;

		bool result = boost::regex_search(m_input, mr, m_e);
		if (result)
		{
			m_out1 =  mr[1].str();
			m_out2 =  mr[2].str();
		}
		return result;

	}
};

struct f_qi : public base<f_qi>
{
	std::vector<char> m_p1;
	std::vector<char> m_p2;

	f_qi()
	{
		m_p1.reserve(3);
		m_p1.reserve(16);
	}

	bool operator()()
	{
		using namespace boost::spirit;

		m_p1.clear();
		m_p2.clear();

		auto b = m_input.begin();
		auto e = m_input.end();

		qi::parse(
			b, e,
			//grammar
			qi::repeat(3)[ascii::alpha] >> ':' >> qi::repeat(1,16)[ascii::alnum],
			m_p1, m_p2);

		if (b == e)
		{
			m_out1.clear();
			m_out2.clear();

			std::copy(m_p1.begin(), m_p1.end(), std::back_inserter(m_out1));
			std::copy(m_p2.begin(), m_p2.end(), std::back_inserter(m_out2));

			return true;
		}
		else
		{
			return false;
		}
	}
};


static void run()
{
	run_benchmark<f_manual>();
	run_benchmark<f_manual_substr>();
	run_benchmark<f_scanf>();
	run_benchmark<f_regexp_with_compile>();
	run_benchmark<f_regexp_no_compile>();
	run_benchmark<f_qi>();
	std::cout << std::endl;
}

}


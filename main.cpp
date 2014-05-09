

#include "simple_id.hpp"

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



int main(int, char**)
{
	try
	{
		run_benchmark<f_null>();

		simple_id_cstr::run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

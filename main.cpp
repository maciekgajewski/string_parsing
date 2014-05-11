#include "simple_id.hpp"
#include "simple_id_cstr.hpp"




int main(int, char**)
{
	try
	{
		calibrate();

		simple_id_cstr::run();
		simple_id::run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}

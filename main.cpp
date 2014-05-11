

#include "simple_id.hpp"




int main(int, char**)
{
	try
	{
		calibrate();

		simple_id_cstr::run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	// qi test
//	{
//		std::cout << " qi test: " << std::endl;
//		using namespace boost::spirit;

//		std::vector<char> a, b;

//		std::string text = "abc:defg";

//		auto it = text.begin();

//		qi::parse(
//			it, text.end(),
//			//grammar
//			qi::repeat(3)[ascii::alpha] >> ':' >> qi::repeat(1,16)[ascii::alnum],
//			a, b);

//		bool success = it == text.end();

//		std::string as, bs;
//		std::copy(a.begin(), a.end(), std::back_inserter(as));
//		std::copy(b.begin(), b.end(), std::back_inserter(bs));

//		std::cout << " parsing success: " << std::boolalpha << success << std::endl;
//		if (success)
//		{
//			std::cout << "result: " << as << ", " << bs << std::endl;
//		}

//	}
}

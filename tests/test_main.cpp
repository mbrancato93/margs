#include "margs.hpp"
// #include <gtest/gtest.h>

// TEST( Main, BasicAssertions )
// {
// 	char *test[2] = { "test", "here" };
// 	margs::margs( 2, test );	
// }

int main( int argc, char *argv[] )
{
	// char *test[3] = { "test", "--here", "1"	};
	margs::margs t;

	t.add_optional( 
			"here", 
			{ "--here", "-h" }, 
			margs::entry( {
				.nargs="1"	
			} )
		   );
	t.add_required( "val" );

	t.add_optional( 
			"debug",
			{ "--debug", "-d" },
			margs::entry( { .nargs="1" } )
		      );

	if (!t.parse( argc, argv )) return EXIT_FAILURE;



	std::unique_ptr<int> here = std::make_unique<int>(-1);
	std::unique_ptr<int> debug = std::make_unique<int>(-1);
	std::unique_ptr<std::string> val = std::make_unique<std::string>("");


	t.get( "here", here );
	t.get( "debug", debug );

	if ( !t.get( "val", val ) ) return EXIT_FAILURE;

	std::cout << "here: " << *here << std::endl;
	std::cout << "debug: " << *debug << std::endl;
	std::cout << "val: " << *val << std::endl;
}

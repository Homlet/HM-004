#include "MacroTime.h"
#include "MacroWindow.h"

#include "Base.h"
#include "Main.h"


/*!
 * Starting point for the process. Calls setup and main loop.
 *
 * @return Program exit state.
 */
int main( void )
{
	try
	{
		Core::run();

	} catch( std::exception& e )
	{
		std::cerr << e.what();
		char c;
		std::cin >> c;

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

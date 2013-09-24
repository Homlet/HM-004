#include "MacroTime.h"
#include "MacroWindow.h"

#include "Base.h"
#include "Main.h"

#include "Renderer.h"


/*!
 * Starting point for the process. Calls setup and main loop.
 *
 * @return Program exit state.
 */
int main( void )
{
	try
	{
		Core* core = new Core();
		core->run();

	} catch( std::exception& e )
	{
		std::cout << e.what();

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


/*!
 * Sets up the window and the render context.
 */
Core::Core( void )
{
	if ( !glfwInit() )
		throw std::exception( "GLFW failed to initialise." );
	else
		std::cout << "GLFW successfully initialised." << std::endl;

	if ( !glewInit() )
		throw std::exception( "GLEW failed to initalise." );
	else
		std::cout << "GLEW successfully initialised." << std::endl;

	glfwWindowHint( GLFW_RED_BITS,   WIN_RED_BITS   );
	glfwWindowHint( GLFW_GREEN_BITS, WIN_GREEN_BITS );
	glfwWindowHint( GLFW_BLUE_BITS,  WIN_BLUE_BITS  );
	glfwWindowHint( GLFW_ALPHA_BITS, WIN_ALPHA_BITS );
	glfwWindowHint( GLFW_SAMPLES, WIN_FSAA );

	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	window = glfwCreateWindow( WIN_W, WIN_H, WIN_TITLE, NULL, NULL );

	glfwDefaultWindowHints();

	if ( !window )
	{
		glfwTerminate();

		throw std::exception( "Window failed to open." );
	} else
		std::cout << "Window opened successfully." << std::endl;

	glfwMakeContextCurrent( window );

	renderer = new Renderer( window );
}


/*!
 * Houses the main game logic and render loops.
 */
void Core::run( void )
{
	      double  t = 0.0;
	const double dt = TME_TICK;

	double current_time     = 0.0;
	double last_time        = 0.0;
	double accumulated_time = 0.0;

	while( !glfwWindowShouldClose( window ) )
	{
		glfwPollEvents();

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		current_time = glfwGetTime();
		accumulated_time += current_time - last_time;
		last_time = current_time;

		while ( accumulated_time >= dt )
		{
			accumulated_time -= TME_TICK;
			t += dt;
		}

		double alpha = accumulated_time / dt;
		
		renderer->render( alpha );

		glfwSwapBuffers( window );
	}

	glfwDestroyWindow( window );
	glfwTerminate();
}


/*!
 * Returns a pointer to the renderer object.
 */
Renderer* Core::getRenderer( void )
{
	return renderer;
}


/*!
 * Outputs the content of a text file into the referenced string.
 *
 * @param uri Path to the file.
 * @param output Reference for the output string.
 *
 * @return Returns if the file was read successfully.
 */
bool readTextFile( std::string url, std::string& output )
{
	std::ifstream file_reader( url );
	
	if ( file_reader.is_open() )
	{
		std::string line;
		while ( file_reader.good() )
		{
			std::getline( file_reader, line );
			output.append( line + '\n' );
		}

		file_reader.close();
		
		return true;
	} else
	{
		std::cout << "Unable to open file: " + url << std::endl;

		return false;
	}
}

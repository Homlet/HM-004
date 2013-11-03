#include "MacroTime.h"
#include "MacroWindow.h"

#include "Base.h"
#include "Main.h"

#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"

#include "Terrain.h"


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


/*!
 * Sets up the window and the render context.
 */
Core::Core( void )
{
	// Initialise GLFW.
	if ( !glfwInit() )
		throw std::exception( "GLFW failed to initialise." );
	else
		std::cout << "GLFW successfully initialised." << std::endl;

	// Open window.
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

	// Initialise GLEW.
	glewExperimental = GL_TRUE;

	if ( glewInit() != GLEW_OK )
	{
		glfwTerminate();
		throw std::exception( "GLEW failed to initalise." );
	} else
		std::cout << "GLEW successfully initialised.\n" << std::endl;

	// Initialise renderer.
	renderer = new Renderer( window );

	// Dummy terrain.
	Terrain* terrain = new Terrain();
	renderer->addTerrain( (*terrain->chunks)[glm::ivec3( -32, -32, -32 )] );
}


bool Core::exists = false;
Core* Core::instance;


/*!
 * Returns a pointer to the core instance. Creates one if it does not exist.
 */
Core* Core::getInstance( void )
{
	if ( !exists )
	{
		instance = new Core();
		exists = true;
	}

	return instance;
}


/*!
 * Houses the main game logic and render loops.
 */
void Core::run( void )
{
	Core* core = getInstance();

	      double  t = 0.0;
	const double dt = TME_TICK;

	double current_time     = 0.0;
	double last_time        = 0.0;
	double accumulated_time = 0.0;

	while( !glfwWindowShouldClose( core->window ) )
	{
		glfwPollEvents();

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		current_time = glfwGetTime();
		accumulated_time += current_time - last_time;
		last_time = current_time;

		while ( accumulated_time >= dt )
		{
			accumulated_time -= dt;
			t += dt;
		}

		double alpha = accumulated_time / dt;
		
		core->renderer->render( alpha );

		glfwSwapBuffers( core->window );
	}

	glfwDestroyWindow( core->window );
	glfwTerminate();
}


/*!
 * Returns a pointer to the renderer object.
 */
Renderer* Core::getRenderer( void )
{
	Core* core = getInstance();

	return core->renderer;
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


/*!
 * Outputs the filename part of a given url.
 *
 * @return Returns true if a filename could be found in the string.
 */
bool getFilename( std::string url, std::string& output )
{
	output = url;

	const size_t last_slash_index = output.find_last_of( "\\/" );
	if ( last_slash_index != std::string::npos )
		output.erase( 0, last_slash_index + 1 );

	const size_t extension_index = output.rfind( '.' );
	if ( extension_index != std::string::npos )
		output.erase( extension_index );

	return true;
}

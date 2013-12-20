#include "MacroTime.h"
#include "MacroWindow.h"

#include "Base.h"

#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"

#include "Terrain.h"


/*!
* Sets up the window and the render context.
*/
Core::Core( void )
{
	// Initialise GLFW.
	if ( !glfwInit() )
		throw std::exception( "GLFW failed to initialise." );
	else
		std::cout << "GLFW successfully initialised.\n";

	// Open window.
	glfwWindowHint( GLFW_RED_BITS,   WIN_RED_BITS );
	glfwWindowHint( GLFW_GREEN_BITS, WIN_GREEN_BITS );
	glfwWindowHint( GLFW_BLUE_BITS,  WIN_BLUE_BITS );
	glfwWindowHint( GLFW_ALPHA_BITS, WIN_ALPHA_BITS );
	glfwWindowHint( GLFW_SAMPLES,    WIN_FSAA );

	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE );

	GLFWwindow* window = glfwCreateWindow( WIN_W, WIN_H, WIN_TITLE, NULL, NULL );

	glfwDefaultWindowHints();

	if ( !window )
	{
		glfwTerminate();

		throw std::exception( "Window failed to open." );
	}
	else
		std::cout << "Window opened successfully.\n";

	glfwMakeContextCurrent( window );

	// Initialise GLEW.
	glewExperimental = GL_TRUE;

	if ( glewInit() != GLEW_OK )
	{
		glfwTerminate();
		throw std::exception( "GLEW failed to initalise." );
	}
	else
		std::cout << "GLEW successfully initialised.\n\n";

	// Initialise renderer.
	renderer = new Renderer( window );
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

	// Dummy camera.
	Camera camera( true );
	camera.setProjection(
		80,
		(float) WIN_W / WIN_H,
		0.1,
		256
	);
	camera.setDirection( glm::vec3( 0.0, 0.0, 1.0 ) );
	camera.moveTo( glm::vec3( 96.0, 96.0, 0.0 ) );

	// Dummy terrain.
	Terrain terrain;
	terrain.addToRenderer( Core::getRenderer() );

	double  t = 0.0;
	const double dt = TME_TICK;

	double current_time     = 0.0;
	double last_time        = 0.0;
	double accumulated_time = 0.0;

	while ( !glfwWindowShouldClose( core->renderer->window ) )
	{
		glfwPollEvents();

		current_time = glfwGetTime();
		accumulated_time += current_time - last_time;
		last_time = current_time;

		while ( accumulated_time >= dt )
		{
			accumulated_time -= dt;
			t += dt;
		}

		double alpha = accumulated_time / dt;

		core->renderer->render( alpha, &camera );

		glfwSwapBuffers( core->renderer->window );
	}

	glfwDestroyWindow( core->renderer->window );
	glfwTerminate();
}


/*!
* Allows intensive operations to prevent the window becoming unresponsive.
*/
void Core::cheapUpdate( void )
{
	glfwPollEvents();

	Core* core = getInstance();

	if ( glfwWindowShouldClose( core->renderer->window ) )
	{
		glfwDestroyWindow( core->renderer->window );
		glfwTerminate();

		exit( EXIT_SUCCESS );
	}
}


/*!
* Allows intensive operations to prevent the window becoming unresponsive.
* Draws a progress bar using OpenGL direct mode.
*/
void Core::cheapProgress( std::string name, float progress )
{
	glfwPollEvents();

	Core* core = getInstance();

	if ( glfwWindowShouldClose( core->renderer->window ) )
	{
		glfwDestroyWindow( core->renderer->window );
		glfwTerminate();

		exit( EXIT_SUCCESS );
	}

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	Core::getRenderer()->renderProgress( name, progress );

	glfwSwapBuffers( core->renderer->window );
}


/*!
* Returns a pointer to the renderer object.
*/
Renderer* Core::getRenderer( void )
{
	return getInstance()->renderer;
}

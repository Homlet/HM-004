#include "MacroTime.h"
#include "MacroWindow.h"
#include "MacroInput.h"

#include "Base.h"

#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"

#include "State.h"
#include "Player.h"
#include "Input.h"
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

	GLFWwindow* window = glfwCreateWindow(
		WIN_W, WIN_H,
		WIN_TITLE,
		( WIN_FULL ) ? glfwGetPrimaryMonitor() : 0,
		0
	);

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

	// Initialise input.
	input = new Input();

	input->add( IN_FORWARD,  { GLFW_KEY_W, GLFW_KEY_UP } );
	input->add( IN_BACKWARD, { GLFW_KEY_S, GLFW_KEY_DOWN } );
	input->add( IN_LEFT,     { GLFW_KEY_A, GLFW_KEY_LEFT } );
	input->add( IN_RIGHT,    { GLFW_KEY_D, GLFW_KEY_RIGHT } );
	input->add( IN_UP,       { GLFW_KEY_SPACE } );
	input->add( IN_DOWN,     { GLFW_KEY_LEFT_CONTROL } );
	input->add( "pause", { GLFW_KEY_ESCAPE, GLFW_KEY_P } );
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
		exists = true;
		instance = new Core();
	}

	return instance;
}


/*!
* Houses the main game logic and render loops.
*/
void Core::run( void )
{
	Core* core = getInstance();
	getRenderer()->setup();

	// Dummy terrain.
	Terrain terrain;
	terrain.addToRenderer( getRenderer() );

	// Dummy state.
	setState( new State() );

	// Disable mouse cursor.
	glfwSetInputMode( getRenderer()->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
	bool paused = false;

	      double  t = 0.0;
	const double dt = TME_TICK;

	double current_time     = 0.0;
	double last_time        = 0.0;
	double accumulated_time = 0.0;

	while ( !glfwWindowShouldClose( core->renderer->window ) )
	{
		current_time = glfwGetTime();
		accumulated_time += current_time - last_time;
		last_time = current_time;

		while ( accumulated_time >= dt )
		{
			glfwPollEvents();
			getInput()->poll();

			if ( getInput()->pressed( "pause" ) )
			{
				paused = !paused;

				glfwSetInputMode(
					getRenderer()->window,
					GLFW_CURSOR,
					( paused ) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED
				);
			}

			getState()->update( dt, t );
			accumulated_time -= dt;
			t += dt;
		}

		double alpha = accumulated_time / dt;

		core->renderer->render( alpha, getState()->getPlayer()->getCamera() );

		glfwSwapBuffers( core->renderer->window );
	}

	delete core->state;

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

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
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


/*!
 * Returns a pointer to the current state object.
 */
State* Core::getState( void )
{
	Core* core = getInstance();

	if ( !core->state )
	{
		core->state = new State();
		core->state->create();
	}

	return core->state;
}


/*!
 * Changes the current state object. This will delete the old object.
 */
void Core::setState( State* next )
{
	Core* core = getInstance();

	if ( core->state )
		delete core->state;

	core->state = next;
}


/*!
 * Returns a pointer to the input object.
 */
Input* Core::getInput( void )
{
	return getInstance()->input;
}

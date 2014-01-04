#include "MacroWindow.h"

#include "Base.h"
#include "Input.h"

#include "Renderer.h"


Input::Input( void )
{
}


/*!
 * Registers a new input for polling.
 */
void Input::add( std::string name, std::vector<int> keys )
{
	this->keys[name] = keys;
}
 

/*!
 * Returns if an input is currently active.
 */
bool Input::get( std::string name )
{
	return cache[name];
}


/*!
 * Returns if an input was pressed in the last poll.
 */
bool Input::pressed( std::string name )
{
	return cache[name] && !cacheOld[name];
}


/*!
 * Returns if an input was released in the last poll.
 */
bool Input::released( std::string name )
{
	return !cache[name] && cacheOld[name];
}


/*!
 * Polls for changes in inputs.
 */
void Input::poll( void )
{
	GLFWwindow* window = Core::getRenderer()->window;

	ox = x;
	oy = y;
	glfwGetCursorPos( window, &x, &y );
	dx = x - ox;
	dy = y - oy;

	for ( auto itr = keys.begin(); itr != keys.end(); itr++ )
	{
		bool down = false;

		for ( int i = 0; i < itr->second.size(); i++ )
		{
			if ( glfwGetKey( window, itr->second[i] ) )
			{
				down = true;
				break;
			}
		}

		cacheOld[itr->first] = this->cache[itr->first];
		this->cache[itr->first] = down;
	}
}


/*!
 * Return the mouse delta from the center for this frame as a vector.
 */
glm::vec2 Input::mouseDelta( void )
{
	return glm::vec2( dx, dy );
}


/*!
 * Return the mouse absolute position as a vector.
 */
glm::vec2 Input::mouseAbsolute( void )
{
	return glm::vec2( x, y );
}

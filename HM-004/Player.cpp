#include "MacroWindow.h"
#include "MacroInput.h"

#include "Base.h"
#include "Player.h"

#include "Renderer.h"
#include "Camera.h"
#include "Input.h"


Player::Player( bool directional ) :
	camera( new Camera( directional ) )
{
}


Player::~Player( void )
{
	delete camera;
}


/*!
 * Returns a reference to the player's camera.
 */
Camera* Player::getCamera( void ) const
{
	return camera;
}


DebugPlayer::DebugPlayer( void ) :
	Player()
{
    camera->setProjection(
        80,
        (float) WIN_W / WIN_H,
        0.1,
        128
    );
    camera->setDirection( glm::vec3( 0.0, 0.0, 1.0 ) );
    camera->moveTo( glm::vec3( 96.0, 96.0, 0.0 ) );

	Core::getInput()->add( "sprint", { GLFW_KEY_LEFT_SHIFT } );
}


/*!
 * Interpolates the camera between the last two updates.
 */
void DebugPlayer::interpolate( void )
{
	// TODO.
}


/*!
 * Move the camera with keyboard, and look toward the mouse.
 */
void DebugPlayer::update( double delta, double elapsed )
{
	Input* input = Core::getInput();
	
	// Point camera towards mouse.
	glm::vec2 mouse = input->mouseDelta();

	mx += mouse.x;
	my += mouse.y;
	my = glm::clamp( my, 1.0, WIN_H - 1.0 );
	double sy = ( WIN_H - my ) * M_PI / WIN_H;

	camera->setDirection( glm::vec3(
		glm::cos( mx / 200 ) * glm::sin( sy ),
		glm::cos( sy + M_PI ),
		glm::sin( mx / 200 ) * glm::sin( sy )
	) );

	// Move with the keyboard.
	double s = delta * speed;
	if ( input->get( "sprint" ) ) s *= 15;
	if ( input->get( IN_FORWARD  ) ) camera->moveRelative( glm::vec3( 0.0, 0.0, -s ) );
	if ( input->get( IN_BACKWARD ) ) camera->moveRelative( glm::vec3( 0.0, 0.0,  s ) );
	if ( input->get( IN_LEFT     ) ) camera->moveRelative( glm::vec3( -s, 0.0, 0.0 ) );
	if ( input->get( IN_RIGHT    ) ) camera->moveRelative( glm::vec3(  s, 0.0, 0.0 ) );
	if ( input->get( IN_UP       ) ) camera->moveBy( glm::vec3( 0.0,  s, 0.0 ) );
	if ( input->get( IN_DOWN     ) ) camera->moveBy( glm::vec3( 0.0, -s, 0.0 ) );
}

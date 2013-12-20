#include "Base.h"
#include "Camera.h"

#include "Matrices.h"


Camera::Camera( bool directional ) :
	   matrices( new Matrices() ),
	directional( directional ),
	   position( 0.0 ),
	     target( 0.0, 0.0, 1.0 ),
	  direction( 0.0, 0.0, 1.0 ),
	         up( 0.0, 1.0, 0.0 )
{
}


/*!
 * Returns pointer to the linked matrices.
 */
Matrices* Camera::getMatrices( void ) const
{
	return matrices;
}


/*!
* Attaches a projection matrix to the camera.
*/
void Camera::setProjection( double fov, double ratio, double near, double far )
{
	this->fov   = fov;
	this->ratio = ratio;
	this->near  = near;
	this->far   = far;

	matrices->setProjection( fov, ratio, near, far );
}


/*!
 * Sets whether the camera should point at a location or in a specific direction.
 */
void Camera::setDirectional( bool directional )
{
	this->directional = directional;

	if ( directional )
	{
		target = position + direction;
		matrices->lookAt( position, target, up );
	}
}


/*!
 * Sets the direction of the camera. The vector will be normalised before storing.
 */
void Camera::setDirection( glm::vec3 direction )
{
	this->direction = direction = glm::normalize( direction );

	if ( directional )
	{
		target = position + direction;
		matrices->lookAt( position, target, up );
	}
}


/*!
 * Sets the camera's target, if it is not directional.
 */
void Camera::setTarget( glm::vec3 target )
{
	if ( !directional )
	{
		this->target = target;
		matrices->lookAt( position, target, up );
	}
}


/*!
 * Moves the camera to an exact position. If the camera is directional,
 * the target is updated.
 */
void Camera::moveTo( glm::vec3 position )
{
	this->position = position;
	
	if ( directional )
		target = position + direction;

	matrices->lookAt( position, target, up );
}


/*!
 * Moves the camera by a specific offset. If the camera is directional,
 * the target is updated.
 */
void Camera::moveBy( glm::vec3 offset )
{
	position += offset;

	if ( directional )
		target = position + direction;

	matrices->lookAt( position, target, up );
}


/*!
 * Moves the camera by a specific offset relative to the camera's view plane.
 * Best used with directional cameras.
 */
void Camera::moveRelative( glm::vec3 offset )
{
	glm::mat4 view = matrices->getView();
	glm::vec4 abs_offset = glm::vec4( offset, 0.0 ) * view;

	position += glm::vec3( abs_offset );

	if ( directional )
		target = position + direction;
	
	matrices->lookAt( position, target, up );
}

#include "Base.h"
#include "Matrices.h"


Matrices::Matrices() :
	     model( 1.0 ),
	      view( 1.0 ),
	projection( 1.0 )
{
}


void Matrices::loadIdentity( void )
{
	model = glm::mat4( 1.0 );
}


void Matrices::translate( glm::vec3 translation )
{
	model *= glm::translate( translation );
}


void Matrices::scale( glm::vec3 factor )
{
	model *= glm::scale( factor );
}


void Matrices::rotate( glm::vec3 axis, float angle )
{
	model *= glm::rotate( glm::radians( angle ), axis );
}


void Matrices::lookAt( glm::vec3 position, glm::vec3 target, glm::vec3 up )
{
	view = glm::lookAt( position, target, up );
}


void Matrices::setProjection( double fov, double ratio, double near, double far )
{
	projection = glm::perspective( glm::radians( fov ), ratio, near, far );
}


void Matrices::setOrtho( double xNear, double xFar, double yNear, double yFar, double zNear, double zFar )
{
	projection = glm::ortho( xNear, xFar, yNear, yFar, zNear, zFar );
}


glm::mat4 Matrices::getModel( void ) const
{
	return model;
}


glm::mat4 Matrices::getView( void ) const
{
	return view;
}


glm::mat4 Matrices::getModelView( void ) const
{
	return view * model;
}


glm::mat4 Matrices::getProjection( void ) const
{
	return projection;
}


glm::mat3 Matrices::getNormal( void ) const
{
	return glm::transpose( glm::inverse( glm::mat3( view * model ) ) );
}

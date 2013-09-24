#include "Base.h"
#include "VAO.h"


/*!
 * Generate a vertex array object.
 */
VAO::VAO( void )
{
	glGenVertexArrays( 1, &ID );
}


/*!
 * Bind the vertex array object asscociated with this object.
 */
void VAO::bind( void )
{
	glBindVertexArray( ID );
}


/*!
 * Unbind the current vertex array object.
 */
void VAO::unbind( void )
{
	glBindVertexArray( 0 );
}

#include "Base.h"
#include "Mesh.h"

#include "VAO.h"


/*!
 * Creates a vbo and an ibo and buffers given data to them.
 */
Mesh::Mesh( std::vector<vertex> vertices, std::vector<GLushort> indices, GLenum poly_mode ) :
	poly_mode( poly_mode ),
	count( (int) indices.size() ),
	vao( new VAO() )
{
	glGenBuffers( 1, &vertexID );
	glGenBuffers( 1, &indexID );
	
	vao->bind();
	{
		bind();

		if ( count > 0 )
		{
			glBufferData(
				GL_ARRAY_BUFFER,
				sizeof ( vertex ) * vertices.size(),
				&vertices[0],
				GL_STATIC_DRAW
			);
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				sizeof ( indices ) * indices.size(),
				&indices[0],
				GL_STATIC_DRAW
			);
		}
		
		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );

		GLsizei stride = sizeof ( GLfloat ) * 9;
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			stride,
			(GLvoid*) 0
		);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_TRUE,
			stride,
			(GLvoid*) (int) ( sizeof ( GLfloat ) * 3 )
		);
		glVertexAttribPointer(
			2,
			3,
			GL_FLOAT,
			GL_TRUE,
			stride,
			(GLvoid*) (int) ( sizeof ( GLfloat ) * 6 )
		);
	}
	vao->unbind();
}


/*!
 * Binds the data and index buffers associated with this object.
 */
void Mesh::bind( void )
{
	glBindBuffer( GL_ARRAY_BUFFER,        vertexID );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexID );
}


/*!
 * Unbinds data and index buffers.
 */
void Mesh::unbind( void )
{
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}


/*!
 * Renders the mesh to the back buffer.
 */
void Mesh::draw( void )
{
	vao->bind();

	glDrawElements( poly_mode, count, GL_UNSIGNED_SHORT, (GLvoid*) 0 );

	vao->unbind();
}


/*!
 * Sets the current position.
 */
void Mesh::setPosition( glm::vec3 position )
{
	this->position = position;
}


/*!
 * Sets the current scale factor.
 */
void Mesh::setScale( glm::vec3 scale )
{
	this->scale = scale;
}


/*!
 * Set the current orientation as an axis-angle.
 */
void Mesh::setOrientation( glm::vec4 orientation )
{
	this->orientation = orientation;
}


/*!
 * Returns the current position in 3d cartesian space.
 */
glm::vec3 Mesh::getPosition( void )
{
	return position;
}


/*!
 * Return the current 3d scale factor.
 */
glm::vec3 Mesh::getScale( void )
{
	return scale;
}


/*!
 * Return the current axis-angle orientation.
 */
glm::vec4 Mesh::getOrientation( void )
{
	return orientation;
}


/*!
 * Add the offset to the current position.
 */
void Mesh::translate( glm::vec3 offset )
{
	position += offset;
}


/*!
 * Add the given factor to the scale value.
 */
void Mesh::addScale( glm::vec3 factor )
{
	scale += factor;
}


/*!
 * Add the given angle to the rotation about the pre-defined axis.
 */
void Mesh::rotate( float amount )
{
	orientation.w += amount;
}


/*!
 * Instantiate a mesh from the given arguments.
 */
LerpMesh::LerpMesh( std::vector<vertex> vertices, std::vector<GLushort> indices, GLenum poly_mode ) :
	Mesh( vertices, indices, poly_mode )
{
}


/*!
 * Set the current position, storing the old position.
 */
void LerpMesh::setPosition( glm::vec3 position )
{
	positionOld = this->position;
	this->position = position;
}


/*!
 * Sets the current scale factor, storing the old factor.
 */
void LerpMesh::setScale( glm::vec3 scale )
{
	scaleOld = this->scale;
	this->scale = scale;
}


/*!
 * Sets the current orientation, storing the old orientation.
 */
void LerpMesh::setOrientation( glm::vec4 orientation )
{
	orientationOld = this->orientation;
	this->orientation = orientation;
}


/*!
 * Returns the position of the object during the previous update frame,
 * for interpolation purposes.
 */
glm::vec3 LerpMesh::getOldPosition( void )
{
	return positionOld;
}


/*!
 * Returns the scale factor of the object during the previous update
 * frame, for interpolation purposes.
 */
glm::vec3 LerpMesh::getOldScale( void )
{
	return scaleOld;
}


/*!
 * Returns the orientation of the object during the previous update
 * frame, for interpolation purposes.
 */
glm::vec4 LerpMesh::getOldOrientation( void )
{
	return orientationOld;
}


/*!
 * Adds the given offset to the position of the object, storing the
 * old position.
 */
void LerpMesh::translate( glm::vec3 offset )
{
	positionOld = position;
	position += offset;
}


/*!
 * Adds the given factor to the scale value, storing the old value.
 */
void LerpMesh::addScale( glm::vec3 factor )
{
	scaleOld = scale;
	scale += factor;
}


/*!
 * Rotates the object around the previously defined axis, storing
 * the old orientation.
 */
void LerpMesh::rotate( float amount )
{
	orientationOld = orientation;
	orientation.w += amount;
}


/*!
 * Returns a pointer to a cube with size and position specified.
 */
Mesh* Mesh::createCube( glm::vec3 position, glm::vec3 size )
{
	vertex v_a[8] = {
		{ -0.5, -0.5, -0.5 },
		{  0.5, -0.5, -0.5 },
		{  0.5,  0.5, -0.5 },
		{ -0.5,  0.5, -0.5 },
		{ -0.5, -0.5,  0.5 },
		{  0.5, -0.5,  0.5 },
		{  0.5,  0.5,  0.5 },
		{ -0.5,  0.5,  0.5 }
	};
	std::vector<vertex> v;
	v.assign( v_a, v_a + 8 );

	GLushort i_a[17] = {
		0,1,2,3,7,4,5,1, 0xffff,
		6,5,4,7,3,2,1,5
	};
	std::vector<GLushort> i;
	i.assign( i_a, i_a + 17 );

	Mesh* m = new Mesh( v, i, GL_TRIANGLE_FAN );

	m->setPosition( position );
	m->setScale( size );

	return m;
}

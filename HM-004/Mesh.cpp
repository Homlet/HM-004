#include "Base.h"
#include "Mesh.h"

#include "VAO.h"


/*!
 * Creates a vbo and an ibo and buffers given data to them.
 */
Mesh::Mesh( std::vector<vertex> vertices, std::vector<GLuint> indices, GLenum poly_mode ) :
	poly_mode( poly_mode ),
	count( (int) indices.size() ),
	vao( new VAO() ),
	scale( 1.0, 1.0, 1.0 )
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
				sizeof ( GLuint ) * indices.size(),
				&indices[0],
				GL_STATIC_DRAW
			);

			empty = false;
		} else
			empty = true;
		
		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );

		GLsizei stride = sizeof ( vertex );
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
	unbind();
}


/*!
 * Rebuffers data without creating a new VBO.
 */
void Mesh::rebuffer( std::vector<vertex> vertices, std::vector<GLuint> indices, GLenum poly_mode )
{
	this->poly_mode = poly_mode;
	count = (int) indices.size();

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
				sizeof ( GLuint ) * indices.size(),
				&indices[0],
				GL_STATIC_DRAW
			);
		}
	}
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
	if ( !empty )
	{
		vao->bind();

		glDrawElements( poly_mode, count, GL_UNSIGNED_INT, (GLvoid*) 0 );

		vao->unbind();
		unbind();
	}
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
LerpMesh::LerpMesh( std::vector<vertex> vertices, std::vector<GLuint> indices, GLenum poly_mode ) :
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
 * Returns a pointer to a cube mesh with size and position specified.
 */
Mesh* Mesh::createCube( glm::vec3 position, glm::vec3 size )
{
	std::vector<vertex> v{
		// Front
		{ -0.5, -0.5,  0.5,  0.0, 0.0, 0.0,   0.0,  0.0,  1.0 },
		{  0.5, -0.5,  0.5,  1.0, 0.0, 0.0,   0.0,  0.0,  1.0 },
		{  0.5,  0.5,  0.5,  1.0, 1.0, 0.0,   0.0,  0.0,  1.0 },
		{ -0.5,  0.5,  0.5,  0.0, 1.0, 0.0,   0.0,  0.0,  1.0 },
		// Back
		{  0.5, -0.5, -0.5,  0.0, 0.0, 0.0,   0.0,  0.0, -1.0 },
		{ -0.5, -0.5, -0.5,  1.0, 0.0, 0.0,   0.0,  0.0, -1.0 },
		{ -0.5,  0.5, -0.5,  1.0, 1.0, 0.0,   0.0,  0.0, -1.0 },
		{  0.5,  0.5, -0.5,  0.0, 1.0, 0.0,   0.0,  0.0, -1.0 },
		// Right
		{  0.5, -0.5,  0.5,  0.0, 0.0, 0.0,   1.0,  0.0,  0.0 },
		{  0.5, -0.5, -0.5,  1.0, 0.0, 0.0,   1.0,  0.0,  0.0 },
		{  0.5,  0.5, -0.5,  1.0, 1.0, 0.0,   1.0,  0.0,  0.0 },
		{  0.5,  0.5,  0.5,  0.0, 1.0, 0.0,   1.0,  0.0,  0.0 },
		// Left
		{ -0.5, -0.5, -0.5,  0.0, 0.0, 0.0,  -1.0,  0.0,  0.0 },
		{ -0.5, -0.5,  0.5,  1.0, 0.0, 0.0,  -1.0,  0.0,  0.0 },
		{ -0.5,  0.5,  0.5,  1.0, 1.0, 0.0,  -1.0,  0.0,  0.0 },
		{ -0.5,  0.5, -0.5,  0.0, 1.0, 0.0,  -1.0,  0.0,  0.0 },
		// Top
		{ -0.5,  0.5,  0.5,  0.0, 0.0, 0.0,   0.0,  1.0,  0.0 },
		{  0.5,  0.5,  0.5,  1.0, 0.0, 0.0,   0.0,  1.0,  0.0 },
		{  0.5,  0.5, -0.5,  1.0, 1.0, 0.0,   0.0,  1.0,  0.0 },
		{ -0.5,  0.5, -0.5,  0.0, 1.0, 0.0,   0.0,  1.0,  0.0 },
		// Bottom
		{ -0.5, -0.5, -0.5,  0.0, 0.0, 0.0,   0.0, -1.0,  0.0 },
		{  0.5, -0.5, -0.5,  1.0, 0.0, 0.0,   0.0, -1.0,  0.0 },
		{  0.5, -0.5,  0.5,  1.0, 1.0, 0.0,   0.0, -1.0,  0.0 },
		{ -0.5, -0.5,  0.5,  0.0, 1.0, 0.0,   0.0, -1.0,  0.0 }
	};

	std::vector<GLuint> i{
		 0,  1,  2,  3,  0xffffffff,
		 4,  5,  6,  7,  0xffffffff,
		 8,  9, 10, 11,  0xffffffff,
		12, 13, 14, 15,  0xffffffff,
		16, 17, 18, 19,  0xffffffff,
		20, 21, 22, 23
	};

	Mesh* m = new Mesh( v, i, GL_TRIANGLE_FAN );

	m->setPosition( position );
	m->setScale( size );

	return m;
}


/*!
 * Macro for easy calculation of the indicies of a cube face based on the curent index offset.
 */
#define FACE_INDICES {             \
			offset,                \
			offset + 1,            \
			offset + 2,            \
			offset + 3, 0xffffffff \
		}


/*!
 * Appends the vertices and indices of a full cube to a pair of vectors.
 */
void Mesh::appendCube( glm::vec3 position, float size, std::vector<vertex>* v, std::vector<GLuint>* i )
{
	cube c{ true, true, true, true, true, true };
	appendCube( position, size, c, v, i );
}


/*!
 * Appends the vertices and indices of chosen faces of a cube to a pair of vectors.
 */
void Mesh::appendCube( glm::vec3 position, float size, cube faces, std::vector<vertex>* v, std::vector<GLuint>* i )
{
	GLuint offset = (GLuint) v->size();

	if ( faces.front )
	{
		vertex v_front[4] = {
			{        position.x,        position.y, size + position.z,  0.0, 0.0, 0.0,   0.0,  0.0,  1.0 },
			{ size + position.x,        position.y, size + position.z,  1.0, 0.0, 0.0,   0.0,  0.0,  1.0 },
			{ size + position.x, size + position.y, size + position.z,  1.0, 1.0, 0.0,   0.0,  0.0,  1.0 },
			{        position.x, size + position.y, size + position.z,  0.0, 1.0, 0.0,   0.0,  0.0,  1.0 }
		};
		GLuint i_a[5] = FACE_INDICES;
		offset += 4;
		
		v->insert( v->end(), v_front, v_front + 4 );
		i->insert( i->end(), i_a,     i_a     + 5 );
	}

	if ( faces.back )
	{
		vertex v_back[4] = {
			{ size + position.x,        position.y,        position.z,  0.0, 0.0, 0.0,   0.0,  0.0, -1.0 },
			{        position.x,        position.y,        position.z,  1.0, 0.0, 0.0,   0.0,  0.0, -1.0 },
			{        position.x, size + position.y,        position.z,  1.0, 1.0, 0.0,   0.0,  0.0, -1.0 },
			{ size + position.x, size + position.y,        position.z,  0.0, 1.0, 0.0,   0.0,  0.0, -1.0 }
		};
		GLuint i_a[5] = FACE_INDICES;
		offset += 4;
		
		v->insert( v->end(), v_back, v_back + 4 );
		i->insert( i->end(), i_a,    i_a    + 5 );
	}

	if ( faces.right )
	{
		vertex v_right[4] = {
			{ size + position.x,        position.y, size + position.z,  0.0, 0.0, 0.0,   1.0,  0.0,  0.0 },
			{ size + position.x,        position.y,        position.z,  1.0, 0.0, 0.0,   1.0,  0.0,  0.0 },
			{ size + position.x, size + position.y,        position.z,  1.0, 1.0, 0.0,   1.0,  0.0,  0.0 },
			{ size + position.x, size + position.y, size + position.z,  0.0, 1.0, 0.0,   1.0,  0.0,  0.0 }
		};
		GLuint i_a[5] = FACE_INDICES;
		offset += 4;
		
		v->insert( v->end(), v_right, v_right + 4 );
		i->insert( i->end(), i_a,     i_a     + 5 );
	}

	if ( faces.left )
	{
		vertex v_left[4] = {
			{        position.x,        position.y,        position.z,  0.0, 0.0, 0.0,  -1.0,  0.0,  0.0 },
			{        position.x,        position.y, size + position.z,  1.0, 0.0, 0.0,  -1.0,  0.0,  0.0 },
			{        position.x, size + position.y, size + position.z,  1.0, 1.0, 0.0,  -1.0,  0.0,  0.0 },
			{        position.x, size + position.y,        position.z,  0.0, 1.0, 0.0,  -1.0,  0.0,  0.0 }
		};
		GLuint i_a[5] = FACE_INDICES;
		offset += 4;
		
		v->insert( v->end(), v_left, v_left + 4 );
		i->insert( i->end(), i_a,    i_a    + 5 );
	}

	if ( faces.top )
	{
		vertex v_top[4] = {
			{        position.x, size + position.y, size + position.z,  0.0, 0.0, 0.0,   0.0,  1.0,  0.0 },
			{ size + position.x, size + position.y, size + position.z,  1.0, 0.0, 0.0,   0.0,  1.0,  0.0 },
			{ size + position.x, size + position.y,        position.z,  1.0, 1.0, 0.0,   0.0,  1.0,  0.0 },
			{        position.x, size + position.y,        position.z,  0.0, 1.0, 0.0,   0.0,  1.0,  0.0 }
		};
		GLuint i_a[5] = FACE_INDICES;
		offset += 4;
		
		v->insert( v->end(), v_top, v_top + 4 );
		i->insert( i->end(), i_a,   i_a   + 5 );
	}

	if ( faces.bottom )
	{
		vertex v_bottom[4] = {
			{        position.x,        position.y,        position.z,  0.0, 0.0, 0.0,   0.0, -1.0,  0.0 },
			{ size + position.x,        position.y,        position.z,  1.0, 0.0, 0.0,   0.0, -1.0,  0.0 },
			{ size + position.x,        position.y, size + position.z,  1.0, 1.0, 0.0,   0.0, -1.0,  0.0 },
			{        position.x,        position.y, size + position.z,  0.0, 1.0, 0.0,   0.0, -1.0,  0.0 }
		};
		GLuint i_a[5] = FACE_INDICES;
		offset += 4;
		
		v->insert( v->end(), v_bottom, v_bottom + 4 );
		i->insert( i->end(), i_a,      i_a      + 5 );
	}
}


/*!
 * Appends the vertices and indices of a single given 2d quad to a 3d mesh, using an up vector.
 */
void Mesh::appendQuad( quad q, std::vector<vertex>* v, std::vector<GLuint>* i )
{
	vertex v_face[4] = {
		{ q.p0.x, q.p0.y, q.p0.z,   0.0,  0.0, (GLfloat)  q.t,   q.n.x, q.n.y, q.n.z },
		{ q.p1.x, q.p1.y, q.p1.z,   q.w,  0.0, (GLfloat)  q.t,   q.n.x, q.n.y, q.n.z },
		{ q.p2.x, q.p2.y, q.p2.z,   q.w,  q.h, (GLfloat)  q.t,   q.n.x, q.n.y, q.n.z },
		{ q.p3.x, q.p3.y, q.p3.z,   0.0,  q.h, (GLfloat)  q.t,   q.n.x, q.n.y, q.n.z }
	};
	GLuint offset = (GLuint) v->size();
	GLuint i_a[5] = FACE_INDICES;
		
	v->insert( v->end(), v_face, v_face + 4 );
	i->insert( i->end(), i_a,    i_a    + 5 );
}


/*!
 * Returns a pointer to a torus mesh with given dimensions.
 */
Mesh* Mesh::createTorus(
	glm::vec3 position,
	glm::vec3 size,
	float radius,
	float tube_radius,
	int resolution
)
{
	float two_pi = 2 * glm::pi<float>();
	
	// Vertices.
	std::vector<vertex> vertices;
	for ( int i = resolution; i >= 0; i-- )
	{
		float u = i * ( two_pi / resolution );

		for ( int j = 0; j <= resolution; j++ )
		{
			float v = j * ( two_pi / resolution );

			glm::vec3 p(
				radius * glm::cos( u ) + tube_radius * glm::cos( v ) * glm::cos( u ),
				radius * glm::sin( u ) + tube_radius * glm::cos( v ) * glm::sin( u ),
				tube_radius * glm::sin( v )
			);

			glm::vec3 t(
				u / two_pi,
				v / two_pi,
				0.0
			);

			glm::vec3 n(
				glm::cos( u ) * glm::cos( v ),
				glm::sin( u ) * glm::cos( v ),
				glm::sin( v )
			);

			vertex q = {
				p.x, p.y, p.z,
				t.x, t.y, t.z,
				n.x, n.y, n.z
			};
			vertices.push_back( q );
		}
	}
	
	// Quad indices.
	std::vector<GLuint> indices;
	int index = 0;
	for ( int i = 0; i <= resolution; i++ )
		for ( int j = 0; j < resolution; j++ )
		{
			GLuint quad[8] = {
				index,
				index + 1,
				index + resolution + 1,  0xffffffff,
				index + resolution + 1,
				index + 1,
				index + resolution + 2,  0xffffffff
			};
			indices.insert( indices.end(), quad, quad + 8 );

			index++;
		}

	Mesh* m = new Mesh( vertices, indices, GL_TRIANGLES );

	m->setPosition( position );
	m->setScale( size );

	return m;
}

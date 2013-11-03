#include "Base.h"
#include "Chunk.h"

#include "Mesh.h"


Chunk::Chunk( glm::ivec3 position, int size ) :
	changed( true ),
	position( position ),
	size( size ),
	id( (int)( glfwGetTime() * 1000 ) )
{
	blocks = new Block**[size];
	for ( int i = 0; i < size; i++ )
	{
		blocks[i] = new Block*[size];
		for ( int j = 0; j < size; j++ )
		{
			blocks[i][j] = new Block[size];
			for ( int k = 0; k < size; k++ )
			{
				blocks[i][j][k] = Block();
				blocks[i][j][k].type =
					( glm::sqrt((32-i)*(32-i)+(32-j)*(32-j)+(32-k)*(32-k)) < 32 ) ? 1 : 0;
			}
		}
	}
}


int Chunk::getID( void )
{
	return id;
}


Mesh* Chunk::getMesh( void )
{
	if ( changed )
	{
		changed = false;
		mesh = generateMesh();
	}

	return mesh;
}


Mesh* Chunk::generateMesh( void )
{
	std::vector<vertex> vertices;
	std::vector<GLuint>  indices;

	for ( int i = 0; i < size; i++ )
		for ( int j = 0; j < size; j++ )
			for ( int k = 0; k < size; k++ )
			{
				cube c = {
					k == size - 1 || !blocks[i][j][k + 1].type,
					k == 0        || !blocks[i][j][k - 1].type,
					i == size - 1 || !blocks[i + 1][j][k].type,
					i == 0        || !blocks[i - 1][j][k].type,
					j == size - 1 || !blocks[i][j + 1][k].type,
					j == 0        || !blocks[i][j - 1][k].type
				};
				switch ( blocks[i][j][k].type )
				{
				case 0:
					break;
				case 1:
					Mesh::appendCube(
						glm::vec3 ( position ) + glm::vec3( i, j, k ),
						1, c, &vertices, &indices
					);
					break;
				}
			}

	return new Mesh( vertices, indices, GL_TRIANGLE_FAN );
}

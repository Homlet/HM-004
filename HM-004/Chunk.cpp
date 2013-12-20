#include "Base.h"
#include "Chunk.h"

#include "Mesh.h"
#include "Terrain.h"


Chunk::Chunk( glm::ivec3 position, int size, Terrain* terrain ) :
	terrain( terrain ),
	changed( true ),
	position( position ),
	positionAbs( position * size ),
	size( size ),
	id( (int)( glfwGetTime() * 100000 ) )
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
				int x = i + position.x * size;
				int y = j + position.y * size;
				int z = k + position.z * size;

				if ( y < ( glm::simplex( glm::vec2( x / 100.0, z / 100.0 ) ) + 1 ) * 32 )
					blocks[i][j][k].type = 1;
				else
					blocks[i][j][k].type = 0;
			}
		}
	}
}


/*!
 * Returns the block type at this position in the chunk.
 */
Block Chunk::getBlockAt( glm::ivec3 pos )
{
	return blocks[pos.x][pos.y][pos.z];
}


/*!
 * Returns the unique integer ID for this chunk.
 */
int Chunk::getID( void )
{
	return id;
}


/*!
 * Returns a pointer to the mesh for this chunk.
 */
Mesh* Chunk::getMesh( void )
{
	if ( changed )
	{
		changed = false;
		mesh = generateMesh();
	}

	return mesh;
}


/*!
 * Generates a mesh for the chunk using a greedy alogrithm.
 */
Mesh* Chunk::generateMesh( void )
{
	std::vector<vertex> vertices;
	std::vector<GLuint> indices;
	
	for ( int d = 0; d < 3; d++ )
	{
		glm::ivec3 p, q;
		int u = ( d + 1 ) % 3;
		int v = ( d + 2 ) % 3;
		char** type = new char*[size];
		bool** face = new bool*[size];
		for ( int m = 0; m < size; m++ )
		{
			type[m] = new char[size];
			face[m] = new bool[size];
		}

		q[d] = 1;

		// Perform algorithm on set of 2D slices along axis d.
		for ( p[d] = 0; p[d] <= size; p[d]++ )
		{
			// Compute mask for slice.
			for ( p[u] = 0; p[u] < size; p[u]++ )
			for ( p[v] = 0; p[v] < size; p[v]++ )
			{
				char near = (
					p[d] == 0 ?
					terrain->getBlockAt( p - q + positionAbs ).type :
					blocks[p[0]-q[0]][p[1]-q[1]][p[2]-q[2]].type
				);
				char far = (
					p[d] == size ?
					terrain->getBlockAt( p + positionAbs ).type :
					blocks[p[0]][p[1]][p[2]].type
				);
				type[p[u]][p[v]] = ( near != 0 ) ^ ( far != 0 ) ? near | far : 0;
				face[p[u]][p[v]] = ( near != 0 );
			}

			// Generate mesh for slice lexicographically.
			for ( int j = 0; j < size; j++ )
			for ( int i = 0; i < size; )
			{
				char t;
				if ( t = type[i][j] )
				{
					// Whether the quad faces backwards along the axis.
					bool f = face[i][j];

					// Compute width and height of quad.
					int w, h = size;
					for ( w = 0; i + w < size && type[i+w][j] == t && face[i+w][j] == f; w++ )
					{
						int th;
						for ( th = 1; j + th < size && type[i+w][j+th] == t && face[i+w][j+th] == f; th++ );
						if ( h > th )
							h = th;
					}

					// Add quad.
					p[u] = i; p[v] = j;
					glm::vec3 wd; wd[f?u:v] = (float)( f ? w : h );
					glm::vec3 hd; hd[f?v:u] = (float)( f ? h : w );
					Mesh::appendQuad(
						quad(
							glm::vec3( positionAbs ) + glm::vec3( p ),
							glm::vec3( positionAbs ) + glm::vec3( p[0]+wd[0]      , p[1]+wd[1]      , p[2]+wd[2]       ),
							glm::vec3( positionAbs ) + glm::vec3( p[0]+wd[0]+hd[0], p[1]+wd[1]+hd[1], p[2]+wd[2]+hd[2] ),
							glm::vec3( positionAbs ) + glm::vec3( p[0]      +hd[0], p[1]      +hd[1], p[2]      +hd[2] ),
							glm::vec3( f ? q : -q ),
							(float)( f ? w : h ),
							(float)( f ? h : w )
						),
						&vertices,
						&indices
					);

					// Mark this area clear on mask.
					for ( int l = i; l < i + w; l++ )
					for ( int k = j; k < j + h; k++ )
						type[l][k] = 0;

					// Advance along by width of quad.
					i += w;
				} else
					// Advance along by one.
					i++;
			}
		}

		// Deallocate mask.
		for ( int m = 0; m < size; m++ )
		{
			delete[] type[m];
			delete[] face[m];
		}
		delete[] type;
		delete[] face;
	}

	return new Mesh( vertices, indices, GL_TRIANGLE_FAN );
}

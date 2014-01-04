#include "Base.h"
#include "Terrain.h"

#include "Renderer.h"
#include "Chunk.h"


Terrain::Terrain( void ) :
	csize( 16 ),
	blockEmpty( new Block() )
{
	blockEmpty->type = 0;

	int count = 0;
	for ( int i = 0; i < 12; i++ )
	{
		for ( int j = 0; j < 8; j++ )
		{
			for ( int k = 0; k < 12; k++ )
			{
				chunks[glm::ivec3( i, j, k )] = new Chunk( glm::ivec3( i, j, k ), csize, this );

				// Stop the window from becoming unresponsive.
				Core::cheapProgress( "Generating terrain.", (float) count++ / ( 12 * 8 * 12 ) );
			}
		}
	}
}


Terrain::~Terrain( void )
{
	for ( auto c : chunks )
		delete c.second;

	delete blockEmpty;
}


/*!
 * Adds the meshes for all current chunks to the renderer.
 * TODO: store renderer for auto adding future chunks.
 */
void Terrain::addToRenderer( Renderer* renderer )
{
	int count = 0;
	for ( auto c : chunks )
	{
		renderer->addTerrain( c.second );

		// Stop the window from becoming unresponsive while large batches are meshed.
		Core::cheapProgress( "Meshing chunks.", (float) count++ / ( 12 * 8 * 12 ) );
	}
}


/*!
 * Returns the chunk at the specificed position from the map.
 */
Chunk* Terrain::getChunkAt( glm::ivec3 pos )
{
	return chunks[pos];
}


/*!
 * Returns the block type at this position in the terrain.
 */
Block Terrain::getBlockAt( glm::ivec3 pos )
{
	glm::ivec3 cpos = glm::ivec3( glm::floor( glm::vec3( pos ) / (float) csize ) );
	if ( chunks.find( cpos ) != chunks.end() )
		return chunks[cpos]->getBlockAt( pos % csize );
	else
		return *blockEmpty;
}

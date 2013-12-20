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
	for ( int i = 0; i < 4; i++ )
	{
		for ( int j = 0; j < 4; j++ )
		{
			for ( int k = 0; k < 4; k++ )
			{
				chunks[glm::ivec3( i, j, k )] = new Chunk( glm::ivec3( i, j, k ), csize, this );

				// Stop the window from becoming unresponsive.
				Core::cheapProgress( "Generating terrain.", (float) count++ / ( 4 * 4 * 4 ) );
			}
		}
	}
}


/*!
 * Adds the meshes for all current chunks to the renderer.
 * TODO: store renderer for auto adding future chunks.
 */
void Terrain::addToRenderer( Renderer* renderer )
{
	int count = 0;
	for ( auto itr = chunks.begin(); itr != chunks.end(); itr++ )
	{
		renderer->addTerrain( itr->second );

		// Stop the window from becoming unresponsive while large batches are meshed.
		Core::cheapProgress( "Meshing chunks.", (float) count++ / ( 4 * 4 * 4 ) );
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

#include "Base.h"
#include "Terrain.h"

#include "Renderer.h"
#include "Chunk.h"


Terrain::Terrain( void ) :
	csize( 16 ),
	width( 12 ),
	height( 8 ),
	depth( 12 ),
	total( width * height * depth ),
	blockEmpty( new Block() )
{
	blockEmpty->type = 0;

	int count = 0;
	for ( int i = 0; i < width; i++ )
		for ( int j = 0; j < height; j++ )
			for ( int k = 0; k < depth; k++ )
			{
				chunks[glm::ivec3( i, j, k )] = new Chunk( glm::ivec3( i, j, k ), csize, this );

				// Stop the window from becoming unresponsive.
				Core::cheapProgress( "Generating hills...", (float) count++ / total );
			}

	count = 0;
	for ( int i = 0; i < width; i++ )
		for ( int j = 0; j < height; j++ )
			for ( int k = 0; k < depth; k++ )
			{
				for ( int x = 0; x < csize; x++ )
					for ( int y = 0; y < csize; y++ )
						for ( int z = 0; z < csize; z++ )
						{
							Block& b = chunks[glm::ivec3( i, j, k )]->getBlockAt( x, y, z );
							int ix = ( csize * i + x );
							int jy = ( csize * j + y );
							int kz = ( csize * k + z );

							if ( b.type > 0 )
								b.type -= glm::simplex( glm::vec3( ix / 30.0, jy / 30.0, kz / 30.0 ) ) - jy / 96.0 > 0;
						}

				// Stop the window from becoming unresponsive.
				Core::cheapProgress( "Generating caves...", (float) count++ / total );
			}

	for ( int i = 0; i < width; i++ )
		for ( int k = 0; k < depth; k++ )
		{
			for ( int x = 0; x < csize; x++ )
				for ( int z = 0; z < csize; z++ )
				{
					int ix = ( csize * i + x );
					int kz = ( csize * k + z );
					int height = (int) ( ( glm::simplex( glm::vec2( ix / 90.0, kz / 90.0 ) ) + 1 ) * 4 + 1 );
					for ( int y = 0; y < height; y++ )
						chunks[glm::ivec3( i, 0, k )]->getBlockAt( x, y, z ).type = 1;
				}
			
			// Stop the window from becoming unresponsive.
			Core::cheapProgress( "Sealing in the goodness...", (float) count++ / ( width * depth ) );
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
		Core::cheapProgress( "Meshing chunks...", (float) count++ / total );
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

#include "Base.h"
#include "Terrain.h"

#include "Chunk.h"


Terrain::Terrain( void ) :
	chunks( new std::map<glm::ivec3, Chunk*, ivec3_compare>() )
{
	Chunk* chunk = new Chunk( glm::ivec3( 0, 0, 0 ), 128 );
	(*chunks)[glm::ivec3( 0, 0, 0 )] = chunk;
}

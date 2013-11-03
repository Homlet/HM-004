#include "Base.h"
#include "Terrain.h"

#include "Chunk.h"


Terrain::Terrain( void ) :
	chunks( new std::map<glm::ivec3, Chunk*, ivec3_compare>() )
{
	Chunk* chunk = new Chunk( glm::ivec3( -32, -32, -32 ), 64 );
	(*chunks)[glm::ivec3( -32, -32, -32 )] = chunk;
}

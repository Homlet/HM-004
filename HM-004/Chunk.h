#pragma once


class Mesh;
class Terrain;


struct Block {
	char type;
};


class Chunk {
private:
	Block*** blocks;

	Terrain* terrain;

	Mesh* mesh;
	Mesh* generateMesh( void );
	bool changed;

	glm::ivec3 position;
	glm::ivec3 positionAbs;
	int size;
	int id;

public:
	Chunk( glm::ivec3 position, int size, Terrain* terrain );
	
	Block  getBlockAt( glm::ivec3 pos );

	int   getID( void );
	Mesh* getMesh( void );
};

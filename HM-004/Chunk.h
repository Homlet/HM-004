#pragma once


class Mesh;

struct Block {
	char type;
};


class Chunk {
private:
	Block*** blocks;

	Mesh* mesh;
	Mesh* generateMesh( void );
	bool changed;

	glm::ivec3 position;
	int size;
	int id;

public:
	Chunk( glm::ivec3 position, int size );

	int getID( void );

	Mesh* getMesh( void );
};

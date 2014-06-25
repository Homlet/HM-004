#pragma once


class Chunk;
class Renderer;
struct Block;

static enum Face {
	RIGHT = 0,
	LEFT,
	BOTTOM,
	TOP,
	FRONT,
	BACK
};


struct BlockType {
	int textures[6];
};


class ivec3_compare {
public:
	bool operator()( glm::ivec3 const& l, glm::ivec3 const& r )
	{
		return l.x  < r.x ||
			   l.x == r.x && l.y  < r.y ||
			   l.x == r.x && l.y == r.y && l.z < r.z;
	};
};


class Terrain {
private:
	std::map<glm::ivec3, Chunk*, ivec3_compare> chunks;
	int csize;
	int width, height, depth, total;

	BlockType* blockTypes;

	Block* blockEmpty;

public:
	Terrain( void );
	~Terrain( void );

	void generateIsland( void );
	void loadBlockTypes( std::string path );

	void addToRenderer( Renderer* renderer );

	Chunk* getChunkAt( glm::ivec3 pos );
	Block  getBlockAt( glm::ivec3 pos );

	const BlockType getBlockTypeFromId( char id );
};

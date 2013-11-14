#pragma once


class Chunk;
class Renderer;
struct Block;


class ivec3_compare {
public:
	bool operator()(glm::ivec3 const& l, glm::ivec3 const& r )
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

	Block* blockEmpty;

public:
	Terrain( void );

	void addToRenderer( Renderer* renderer );

	Chunk* getChunkAt( glm::ivec3 pos );
	Block  getBlockAt( glm::ivec3 pos );
};

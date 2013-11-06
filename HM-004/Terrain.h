#pragma once


class Chunk;


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
public:
	Terrain( void );

	std::map<glm::ivec3, Chunk*, ivec3_compare>* chunks;
};

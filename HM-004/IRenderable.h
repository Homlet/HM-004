#pragma once


class Mesh;


class IRenderable {
public:
	virtual Mesh* getMesh( void ) = 0;
};

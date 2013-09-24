#pragma once


#include "IRenderable.h"


class Terrain : public IRenderable {
private:
public:
	int getID();

	Mesh* getMesh( void );
};

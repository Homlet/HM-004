#pragma once


#include "IRenderable.h"


class Entity : public IRenderable {
private:
public:
	int getID();

	Mesh* getMesh( void );
};

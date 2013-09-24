#pragma once


#include "IRenderable.h"


class Entity : public IRenderable {
private:
public:
	Mesh* getMesh( void );
};

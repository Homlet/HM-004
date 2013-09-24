#pragma once


#include "IRenderable.h"


class GUIElement : public IRenderable {
private:
public:
	int getID();

	Mesh* getMesh( void );
};

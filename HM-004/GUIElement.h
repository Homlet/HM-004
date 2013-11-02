#pragma once


class Mesh;


class GUIElement {
private:
public:
	GUIElement( void );

	int getID( void );

	Mesh* getMesh( void );
};

#pragma once


class LerpMesh;


class Entity {
private:
public:
	Entity( void );

	int getID( void );

	LerpMesh* getMesh( void );
};

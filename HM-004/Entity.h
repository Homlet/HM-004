#pragma once


class LerpMesh;


class Entity {
private:
public:
	Entity( void );

	int getID( void );

	virtual void update( double delta, double elapsed ) = 0;

	LerpMesh* getMesh( void );
};

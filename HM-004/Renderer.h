#pragma once


class     Mesh;
class LerpMesh;

class Entity;
class Terrain;
class GUIElement;


class Renderer {
private:
	std::map<int, LerpMesh*>* entities;
	std::map<int,     Mesh*>* terrain;
	std::map<int,     Mesh*>* gui;

public:
	Renderer( GLFWwindow* window );

	void render( double alpha );

	void renderEntities( double alpha );
	void renderTerrain( void );
	void renderGUI( void );

	void addEntity(  Entity* entity );
	void addTerrain( Terrain* terrain );
	void addGUI(     GUIElement* gui );

	void removeEntity( int id );
	void removeTerrain( int id );
	void removeGUI( int id );

	GLFWwindow* const window;
};

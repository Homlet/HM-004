#pragma once


template <typename T>
class ResourceCache;

class Matrices;
class Shader;
class Texture;

class     Mesh;
class LerpMesh;

class Entity;
class Chunk;
class GUIElement;


class Renderer {
private:
	std::map<int, LerpMesh*>* entities;
	std::map<int,     Mesh*>* terrain;
	std::map<int,     Mesh*>* gui;

	ResourceCache<Shader>*   shaderCache;
	ResourceCache<Texture>* textureCache;

	Matrices* matrices;
	Shader* shader;
	Mesh* mesh;

	void setupOGL( void );

public:
	Renderer( GLFWwindow* window );

	void render( double alpha );

	void renderEntities( double alpha );
	void  renderTerrain( void );
	void      renderGUI( void );

	void  addEntity( Entity* entity   );
	void addTerrain( Chunk* chunk     );
	void     addGUI( GUIElement* gui  );

	void  removeEntity( int id );
	void removeTerrain( int id );
	void     removeGUI( int id );

	Shader* getShader( std::string url );

	GLFWwindow* const window;
};

#pragma once


template <typename T>
class ResourceCache;

class Matrices;
class Shader;
class ShadowMap;
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

	Shader* shaderEntity;
	Shader* shaderTerrain;
	Shader* shaderGUI;
	Shader* shaderShadow;

	ShadowMap* shadow;
	Matrices*  shadowMatrices;

	glm::vec3  lightDir;
	glm::vec3  lightColor;

	void setupOGL( void );

public:
	float dist;

	Renderer( GLFWwindow* window );

	void render( double alpha );

	void renderEntities(
		double alpha,
		Shader* shader,
		Matrices* mat,
		Matrices* shadowMat = 0
	);
	void renderTerrain(
		Shader* shader,
		Matrices* mat,
		Matrices* shadowMat = 0
	);
	void renderGUI(
		Shader* shader,
		Matrices* mat
	);

	void  addEntity( Entity* entity   );
	void addTerrain( Chunk* chunk     );
	void     addGUI( GUIElement* gui  );

	void  removeEntity( int id );
	void removeTerrain( int id );
	void     removeGUI( int id );

	Shader* getShader( std::string url );

	GLFWwindow* const window;
};

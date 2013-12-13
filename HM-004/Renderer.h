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
	friend void scroll( GLFWwindow* window, double x, double y );

	std::map<int, LerpMesh*>* entities;
	std::map<int,     Mesh*>* terrain;
	std::map<int,     Mesh*>* gui;

	ResourceCache<Shader>*   shaderCache;
	ResourceCache<Texture>* textureCache;

	struct FONScontext* stash;
	int consola;

	// DEBUG STUFF ---------------¬
	float dist;
	
	Texture* textureTerrain;
	Mesh* torus;
	// ---------------------------'

	Shader* shaderEntity;
	Shader* shaderTerrain;
	Shader* shaderGUI;
	Shader* shaderShadow;
	Shader* shaderFont;
	
	Matrices* matrices;
	Matrices*  shadowMatrices;
	ShadowMap* shadowMap;

	glm::vec3 lightDir;
	glm::vec3 lightColor;

	void setupOGL( void );
	void setupFontStash( void );

public:
	Renderer( GLFWwindow* window );

	void render( double alpha );

	void renderTorus(
		Shader* shader,
		Matrices* mat,
		Matrices* shadowMat = 0
	);
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
	void renderProgress(
		std::string name,
		float progress,
		glm::vec4 color = glm::vec4( 0.0, 0.7, 0.1, 1.0 )
	);
	void renderString(
		std::string text,
		glm::vec2 pos
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

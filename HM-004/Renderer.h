#pragma once


template <typename T>
class ResourceCache;

class Matrices;
class Camera;
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
	// Meshes for rendering.
	std::map<int, LerpMesh*>* entities;
	std::map<int,     Mesh*>* terrain;
	std::map<int,     Mesh*>* gui;

	// Resource caches.
	ResourceCache<Shader>*   shaderCache;
	ResourceCache<Texture>* textureCache;

	// Fonts.
	struct FONScontext* stash;
	int consola;

	// DEBUG STUFF ---------------¬
	Texture* textureTerrain;
	Mesh* torus;
	// ---------------------------'

	// Shaders.
	Shader* shaderEntity;
	Shader* shaderTerrain;
	Shader* shaderGUI;
	Shader* shaderShadow;
	
	// Light and shadow.
	Matrices*  shadowMatrices;
	ShadowMap* shadowMap;

	glm::vec3 lightDir;
	glm::vec3 lightColor;

	// Initial setup of libraries.
	void setupOpenGL( void );
	void setupFontStash( void );

public:
	Renderer( GLFWwindow* window );

	void setup( void );
	void render( double alpha, Camera* camera );

	void renderTorus(
		Shader* shader,
		Matrices* mat,
		Matrices* shadowMat = nullptr
	);
	void renderEntities(
		double alpha,
		Shader* shader,
		Matrices* mat,
		Matrices* shadowMat = nullptr
	);
	void renderTerrain(
		Shader* shader,
		Matrices* mat,
		Matrices* shadowMat = nullptr
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
		float size,
		glm::vec2 pos,
		glm::vec3 color = glm::vec3( 1.0, 1.0, 1.0 ),
		float blur = 0.0f
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

#include "MacroTime.h"
#include "MacroWindow.h"

#include "Base.h"
#include "Renderer.h"

#include "ResourceCache.h"

#include "Matrices.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"

#include "Entity.h"
#include "Chunk.h"
#include "GUIElement.h"


/*!
 * Sets up the OpenGL state.
 */
void Renderer::setupOGL( void )
{
	glEnable( GL_PRIMITIVE_RESTART );
	glPrimitiveRestartIndex( 0xffffffff );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}


/*!
 * Sets up the rendering context.
 *
 * @param window GLFWwindow whose context to render to. Once set, this cannot be changed.
 */
Renderer::Renderer( GLFWwindow* window ) :
	window( window ),
	entities( new std::map<int, LerpMesh*>() ),
	 terrain( new std::map<int,     Mesh*>() ),
	     gui( new std::map<int,     Mesh*>() ),
	 shaderCache( new ResourceCache<Shader>()  ),
	textureCache( new ResourceCache<Texture>() ),
	matrices( new Matrices() )
{
	// Setup matrices.
	matrices->setProjection(
		90,
		(float) WIN_W / WIN_H,
		0.1,
		1000
	);
	matrices->lookAt(
		glm::vec3( 0.0, 0.0, -1.0 ),
		glm::vec3( 0.0 ),
		glm::vec3( 0.0, 1.0, 0.0 )
	);
	
	// Dummy shader.
	shader = shaderCache->getResource( "shader/test_phong.prog" );

	// Dummy texture.
	textureCache->getResource( "texture/block_grass_top.tga" )->bind();

	shader->bind();
	shader->sendProjection( matrices->getProjection() );
	Shader::unbind();

	setupOGL();
}


/*!
 * Renders one frame to the back buffer, blending entity meshes by given alpha.
 */
void Renderer::render( double alpha )
{
	double mx, my;
	glfwGetCursorPos( Core::getRenderer()->window, &mx, &my );

	my = glm::clamp<double>( WIN_H - my, 0.1, WIN_H );

	matrices->lookAt(
		glm::vec3(
			glm::cos( mx / 200 ) * 40.0 * glm::sin( my / 200 ),
			glm::cos( my / 200 ) * 40.0,
			glm::sin( mx / 200 ) * 40.0 * glm::sin( my / 200 )
		),
		glm::vec3( 0.0 ),
		glm::vec3( 0.0, 1.0, 0.0 )
	);

	renderEntities( alpha );
	renderTerrain();
	renderGUI();
}


/*!
 * Draw entities to the back buffer with interpolation.
 *
 * @param alpha amount to blend between last and current frame.
 */
void Renderer::renderEntities( double alpha )
{
	// TODO.
}


/*!
 * Draw terrain to the back buffer.
 */
void Renderer::renderTerrain( void )
{
	shader->bind();

	for ( auto itr = terrain->begin(); itr != terrain->end(); itr++ )
	{
		Mesh* m = itr->second;

		// Reset the model matrix.
		matrices->loadIdentity();

		// Perform mesh-wise matrix transformations.
		matrices->translate( m->getPosition() );
		matrices->scale( m->getScale() );

		// Send the precomputed modelview and normal matrices to GPU.
		shader->sendModelView( matrices->getModelView() );
		shader->sendNormal( matrices->getNormal() );

		m->draw();
	}

	Shader::unbind();
}


/*!
 * Draw GUI to the back buffer, ignoring the depth buffer.
 */
void Renderer::renderGUI( void )
{
	// TODO.
}


/*!
 * Add an entity to be rendered with interpolation.
 */
void Renderer::addEntity( Entity* entity )
{
	entities->insert(
		std::pair<int, LerpMesh*>(
			entity->getID(),
			entity->getMesh()
		)
	);
}


/*!
 * Add a terrain section to be rendererd.
 */
void Renderer::addTerrain( Chunk* chunk )
{
	this->terrain->insert(
		std::pair<int, Mesh*>(
			chunk->getID(),
			chunk->getMesh()
		)
	);
}


/*!
 * Add a GUI element to be rendered infront of 3d geometry.
 */
void Renderer::addGUI( GUIElement* gui )
{
	this->gui->insert(
		std::pair<int, Mesh*>(
			gui->getID(),
			gui->getMesh()
		)
	);
}


/*!
 * Queries the shader resource cache for a shader.
 */
Shader* Renderer::getShader( std::string url )
{
	return shaderCache->getResource( url );
}

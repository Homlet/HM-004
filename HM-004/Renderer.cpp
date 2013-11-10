#include "MacroTime.h"
#include "MacroWindow.h"

#include "Base.h"
#include "Renderer.h"

#include "ResourceCache.h"

#include "Matrices.h"
#include "Shader.h"
#include "FBO.h"
#include "Texture.h"
#include "Mesh.h"

#include "Entity.h"
#include "Chunk.h"
#include "GUIElement.h"


void scroll( GLFWwindow* window, double x, double y )
{
	Core::getRenderer()->dist -= (float) y;
}


/*!
 * Sets up the OpenGL state.
 */
void Renderer::setupOGL( void )
{
	glEnable( GL_TEXTURE_2D );

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
	matrices( new Matrices() ),
//	 shaderEntity( shaderCache->getResource( "shader/entity.prog"  ) ),
	shaderTerrain( shaderCache->getResource( "shader/terrain.prog" ) ),
//	    shaderGUI( shaderCache->getResource( "shader/gui.prog"     ) ),
	 shaderShadow( shaderCache->getResource( "shader/shadow.prog"  ) ),
	shadow( new ShadowMap() ),
	shadowMatrices( new Matrices() )
{
	// Setup matrices.
	matrices->setProjection(
		80,
		(float) WIN_W / WIN_H,
		0.1,
		1000
	);
	matrices->lookAt(
		glm::vec3( 0.0, 0.0, 43.0 ),
		glm::vec3( 0.0 ),
		glm::vec3( 0.0, 1.0, 0.0 )
	);

	// Setup shadow map matrices.
	lightDir   = glm::vec3( 1.0, 0.7, 0.9 );
	lightColor = glm::vec3( 1.0, 1.0, 0.8 );
	shadowMatrices->setOrtho(
		-64, 63,
		-64, 63,
		-64, 63
	);
	shadowMatrices->lookAt(
		glm::vec3( 0.0 ),
		lightDir,
		glm::vec3( 0.0, 1.0, 0.0 )
	);
	glm::mat4 shadowBias(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	
	// Send projection matrices to shaders.
	/* TODO: if ( shaderEntity->usesProjection() )
		shaderTerrain->sendProjection( matrices->getProjection() );
	if ( shaderEntity->usesShadowMatrices() )
		shaderEntity->sendShadowProjection( shadowBias * shadowMatrices->getProjection() ); */

	if ( shaderTerrain->usesProjection() )
		shaderTerrain->sendProjection( matrices->getProjection() );
	if ( shaderTerrain->usesShadowMatrices() )
		shaderTerrain->sendShadowProjection( shadowBias * shadowMatrices->getProjection() );

	/* TODO: if ( shaderGUI->usesProjection() )
		shaderGUI->sendProjection( matrices->getProjection() ); */

	if ( shaderShadow->usesProjection() )
		shaderShadow->sendProjection( shadowMatrices->getProjection() );

	setupOGL();

	// Dummy texture.
	textureCache->getResource( "texture/block_sand.tga" )->bind();

	glfwSetScrollCallback( window, scroll );
	dist = 46.0f;
}


/*!
 * Renders one frame to the back buffer, blending entity meshes by given alpha.
 */
void Renderer::render( double alpha )
{
	double mx, my;
	glfwGetCursorPos( Core::getRenderer()->window, &mx, &my );

	my = glm::clamp<double>( WIN_H - my, 0.1, WIN_H );

	if ( glfwGetMouseButton( window, 0 ) )
	{
		matrices->lookAt(
			glm::vec3(
				glm::cos( mx / 200 ) * dist * glm::sin( my / 200 ) + 64.0,
				glm::cos( my / 200 ) * dist + 64.0,
				glm::sin( mx / 200 ) * dist * glm::sin( my / 200 ) + 64.0
			),
			glm::vec3( 64.0, 64.0, 64.0 ),
			glm::vec3( 0.0, 1.0, 0.0 )
		);
	} else if( glfwGetMouseButton( window, 1 ) )
	{
		lightDir = glm::vec3(
			glm::cos( mx / 200 ) * glm::sin( my / 200 ),
			glm::cos( my / 200 ),
			glm::sin( mx / 200 ) * glm::sin( my / 200 )
		);
	}
	
	// Render shadow map as a normal scene.
	shadowMatrices->lookAt(
		glm::vec3( 0.0 ),
		lightDir,
		glm::vec3( 0.0, 1.0, 0.0 )
	);

	shadow->bind();

	glClear( GL_DEPTH_BUFFER_BIT );
	glDisable( GL_CULL_FACE );
	
	renderTerrain(         shaderShadow, shadowMatrices );
	renderEntities( alpha, shaderShadow, shadowMatrices );

	FBO::unbind();
	
	// Render final scene using the shadow matrices to cast shadows.
	shadow->bindTexture();

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_CULL_FACE );

	renderTerrain(         shaderTerrain, matrices, shadowMatrices );
	renderEntities( alpha, shaderEntity,  matrices, shadowMatrices ); // TODO: switch shader back.
	renderGUI(             shaderGUI,     matrices );

	FBO::unbindTexture();
}


/*!
 * Draw entities to the back buffer with interpolation.
 *
 * @param alpha amount to blend between last and current frame.
 */
void Renderer::renderEntities( double alpha, Shader* shader, Matrices* mat, Matrices* shadowMat )
{
	// TODO.
}


/*!
 * Draw terrain to the back buffer.
 */
void Renderer::renderTerrain( Shader* shader, Matrices* mat, Matrices* shadowMat )
{
	shader->bind();

	// Send directional light data.
	if ( shader->usesLightDir() )
	{
		shader->sendLightDir( glm::normalize( glm::mat3( mat->getView() ) * lightDir ) );
		shader->sendLightColor( lightColor );
	}

	// Render chunks.
	for ( auto itr = terrain->begin(); itr != terrain->end(); itr++ )
	{
		Mesh* m = itr->second;

		// Reset the model matrix.
		mat->loadIdentity();

		// Only translation is required for terrain.
		mat->translate( m->getPosition() );

		// Send the precomputed modelview and normal (inverse-
		// transpose modelview) matrices to GPU.
		if ( shader->usesModelView() )
			shader->sendModelView( mat->getModelView() );

		if ( shader->usesNormal() )
			shader->sendNormal( mat->getNormal() );

		// Compute and send the shadow matrices if needed.
		if ( shadowMat != 0 &&
			 shader->usesShadowMatrices() )
		{
			shadowMat->loadIdentity();
			shadowMat->translate( m->getPosition() );
			shader->sendShadowModelView( shadowMat->getModelView() );
		}

		m->draw();
	}

	Shader::unbind();
}


/*!
 * Draw GUI to the back buffer, ignoring the depth buffer.
 */
void Renderer::renderGUI( Shader* shader, Matrices* mat )
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

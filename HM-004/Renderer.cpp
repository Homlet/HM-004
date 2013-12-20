#include "MacroTime.h"
#include "MacroWindow.h"

#include "Base.h"
#include "Renderer.h"

#include "ResourceCache.h"

#include "Matrices.h"
#include "Camera.h"
#include "Shader.h"
#include "FBO.h"
#include "Texture.h"
#include "Mesh.h"

#include "Entity.h"
#include "Chunk.h"
#include "GUIElement.h"

#define   FONTSTASH_IMPLEMENTATION
#define GLFONTSTASH_IMPLEMENTATION
#include "font/fontstash.h"
#include "font/glfontstash.h"


/*!
 * Sets up the OpenGL state.
 */
void Renderer::setupOpenGL( void )
{
	glEnable( GL_TEXTURE_2D );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	glEnable( GL_PRIMITIVE_RESTART );
	glPrimitiveRestartIndex( 0xffffffff );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, WIN_W, 0, WIN_H, -1, 1 );
	glMatrixMode( GL_MODELVIEW );

	glViewport( 0, 0, WIN_W, WIN_H );
}


/*!
 * Sets up the FreeType font renderer.
 */
void Renderer::setupFontStash( void )
{
	stash = glfonsCreate( 512, 512, FONS_ZERO_BOTTOMLEFT );
	consola = fonsAddFont( stash, "consola", "font/ConsolaMono.ttf" );
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
	textureTerrain( textureCache->getResource( "texture/block_sand.tga" ) ),
//	 shaderEntity( shaderCache->getResource( "shader/entity.prog"     ) ),
	shaderTerrain( shaderCache->getResource( "shader/terrain.prog"    ) ),
//	    shaderGUI( shaderCache->getResource( "shader/gui.prog"        ) ),
	 shaderShadow( shaderCache->getResource( "shader/shadow.prog"     ) ),
	shadowMatrices( new Matrices()  ),
	     shadowMap( new ShadowMap() )
{
	// Setup shadow map matrices.
	lightDir   = glm::vec3( 1.0, 0.7, 0.9 );
	lightColor = glm::vec3( 1.0, 1.0, 0.8 );
	shadowMatrices->setOrtho(
		-128, 127,
		-128, 127,
		-128, 127
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
	
	// Send shadow projection matrices to shaders.
	/* TODO:
	if ( shaderEntity->usesShadowMatrices() )
		shaderEntity->sendShadowProjection( shadowBias * shadowMatrices->getProjection() ); */

	if ( shaderTerrain->usesShadowMatrices() )
		shaderTerrain->sendShadowProjection( shadowBias * shadowMatrices->getProjection() );

	if ( shaderShadow->usesProjection() )
		shaderShadow->sendProjection( shadowMatrices->getProjection() );

	setupOpenGL();
	setupFontStash();

	// DEBUG STUFF -----------------------------------------------------------------------¬
	torus = Mesh::createTorus( glm::vec3( 0.0 ), glm::vec3( 10.0, 10.0, 10.0 ), 8, 1, 4 );
}


/*!
 * Renders one frame to the back buffer, blending entity meshes by given alpha.
 */
void Renderer::render( double alpha, Camera* camera )
{
	double mx, my;
	glfwGetCursorPos( Core::getRenderer()->window, &mx, &my );
	my = glm::clamp<double>( WIN_H - my, 0.1, WIN_H - 0.1 );
	float sy = (float) ( M_PI / WIN_H );

	if ( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_RIGHT ) )
	{
		lightDir = glm::vec3(
			glm::cos( mx / 200 ) * glm::sin( my / 200 ),
			glm::cos( my / 200 ),
			glm::sin( mx / 200 ) * glm::sin( my / 200 )
		);
	} else
	{
		camera->setDirection( glm::vec3(
			glm::cos( mx / 200 ) * glm::sin( my * sy ),
			glm::cos( my * sy + M_PI ),
			glm::sin( mx / 200 ) * glm::sin( my * sy )
		) );
	}

	float speed = 0.1f;
	if ( glfwGetKey( Core::getRenderer()->window, GLFW_KEY_W ) )
		camera->moveRelative( glm::vec3( 0.0, 0.0, -speed ) );
	if ( glfwGetKey( Core::getRenderer()->window, GLFW_KEY_A ) )
		camera->moveRelative( glm::vec3( -speed, 0.0, 0.0 ) );
	if ( glfwGetKey( Core::getRenderer()->window, GLFW_KEY_S ) )
		camera->moveRelative( glm::vec3( 0.0, 0.0, speed ) );
	if ( glfwGetKey( Core::getRenderer()->window, GLFW_KEY_D ) )
		camera->moveRelative( glm::vec3( speed, 0.0, 0.0 ) );

	// Send projection matrices.
	if ( shaderTerrain->usesProjection() )
		shaderTerrain->sendProjection( camera->getMatrices()->getProjection() );
	/* TODO: if ( shaderEntity->usesProjection() )
		shaderTerrain->sendProjection( camera->getMatrices()->getProjection() ); */
	
	// Render shadow map as a normal scene.
	shadowMatrices->lookAt(
		glm::vec3( 0.0 ),
		lightDir,
		glm::vec3( 0.0, 1.0, 0.0 )
	);

	shadowMap->bind();

	glClear( GL_DEPTH_BUFFER_BIT );
	glDisable( GL_CULL_FACE );
	
	renderTerrain(         shaderShadow, shadowMatrices );
	renderEntities( alpha, shaderShadow, shadowMatrices );

	FBO::unbind();
	
	// Render final scene using the shadow matrices to cast shadows.
	shadowMap->bindTexture();

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_CULL_FACE );

	textureTerrain->bind();
	renderTerrain(         shaderTerrain, camera->getMatrices(), shadowMatrices );
//	renderEntities( alpha, shaderEntity,  camera->getMatrices(), shadowMatrices );
	renderGUI(             shaderGUI,     camera->getMatrices() );

	FBO::unbindTexture();

	// Print mouse x and y values.
	std::stringstream s;
	s << "x: " << std::setw( 4 ) << mx << " y: " << std::setw( 4 ) << my;
	renderString( s.str(), 24.0f, glm::vec2( 5, 5 ), glm::vec3( 0 ), 2 );
	renderString( s.str(), 24.0f, glm::vec2( 5, 5 ), glm::vec3( 0 ), 3 );
	renderString( s.str(), 24.0f, glm::vec2( 5, 5 ), glm::vec3( 0 ), 4 );
	renderString( s.str(), 24.0f, glm::vec2( 5, 5 ) );
}


/*!
 * Draw a torus to the back buffer.
 */
void Renderer::renderTorus( Shader* shader, Matrices* mat, Matrices* shadowMat )
{
	shader->bind();

	// Send directional light data.
	if ( shader->usesLightDir() )
	{
		shader->sendLightDir( glm::normalize( glm::mat3( mat->getView() ) * lightDir ) );
		shader->sendLightColor( lightColor );
	}
	
	// Reset the model matrix.
	mat->loadIdentity();

	// Only translation is required for terrain.
	mat->translate( torus->getPosition() );

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
		shadowMat->translate( torus->getPosition() );
		shader->sendShadowModelView( shadowMat->getModelView() );
	}

	torus->draw();

	Shader::unbind();
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
 * Draw a progress bar to the back buffer, via the fixed-function pipeline.
 */
void Renderer::renderProgress( std::string name, float progress, glm::vec4 color )
{
	renderString( name, 24.0f, glm::vec2( 4, 21 ) );

	glDisable( GL_TEXTURE_2D );
	glBegin( GL_QUADS );
	{
		glColor4f( color.r, color.g, color.b, color.a );
		glVertex2f( 0, 0 );
		glVertex2f( WIN_W * progress, 0  );
		glVertex2f( WIN_W * progress, 15 );
		glVertex2f( 0, 15 );
	}
	glEnd();
	glEnable( GL_TEXTURE_2D );
}


/*!
 * Draw a string to the back buffer, via the fixed-function pipeline.
 */
void Renderer::renderString( std::string text, float size, glm::vec2 pos, glm::vec3 color, float blur )
{
	glDisable( GL_TEXTURE_2D );

	fonsSetFont(  stash, consola );
	fonsSetSize(  stash, size );
	fonsSetColor( stash, glfonsRGBA(
		(unsigned char) color.r * 255,
		(unsigned char) color.g * 255,
		(unsigned char) color.b * 255,
		255
	) );
	fonsSetBlur(  stash, blur );
	fonsDrawText( stash, pos.x, pos.y, text.c_str(), 0 );

	glEnable( GL_TEXTURE_2D );
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

#include "MacroTime.h"
#include "MacroWindow.h"

#include "Base.h"
#include "Renderer.h"

#include "Matrices.h"
#include "Shader.h"
#include "Mesh.h"
#include "Entity.h"
#include "Terrain.h"
#include "GUIElement.h"


/*!
 * Sets up the OpenGL state.
 */
void Renderer::setupOGL( void )
{
	glEnable( GL_PRIMITIVE_RESTART );
	glPrimitiveRestartIndex( 0xffff );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glFrontFace( GL_CW );

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
	matrices( new Matrices )
{
	// Setup matrices.
	matrices->setProjection(
		90,
		(float) WIN_W / WIN_H,
		1,
		100
	);
	matrices->lookAt(
		glm::vec3( 0.0, 0.0, -1.0 ),
		glm::vec3( 0.0 ),
		glm::vec3( 0.0, 1.0, 0.0 )
	);

	// Dummy mesh.
	m = Mesh::createCube( glm::vec3(), glm::vec3( 0.5, 2.0, 1.0 ) );
	
	// Dummy shader.
	std::string vert_src = GLSL(
		layout(location = 0) in vec3 i_pos;
		layout(location = 1) in vec3 i_normal;
		layout(location = 2) in vec3 i_tex;

		out vec3 f_pos;

		uniform mat4 u_MV;
		uniform mat4 u_P;

		void main( void )
		{
			f_pos = i_pos;

			gl_Position = u_P * u_MV * vec4( i_pos, 1.0 );
		}
	);

	std::string frag_src = GLSL(
		out vec4 o_color;

		in vec3 f_pos;

		void main( void )
		{
			o_color = vec4( f_pos + 0.5, 1.0 );
		}
	);

	shader = new Shader( "Basic", vert_src, frag_src );

	setupOGL();
}


/*!
 * Renders one frame to the back buffer, blending entity meshes by given alpha.
 */
void Renderer::render( double alpha )
{
	matrices->lookAt(
		glm::vec3(
			glm::sin( alpha * glm::pi<float>() * 2 ) * 2.0,
			1.5,
			glm::cos( alpha * glm::pi<float>() * 2 ) * 2.0
		),
		glm::vec3( 0.0 ),
		glm::vec3( 0.0, 1.0, 0.0 )
	);

	shader->bind();
	shader->sendProjection( matrices->getProjection() );

	// Reset the model matrix.
	matrices->loadIdentity();

	// Perform mesh-wise matrix transformations.
	matrices->translate( m->getPosition() );
	matrices->scale( m->getScale() );

	// Send the modelview matrix to GPU.
	matrices->computeModelView();
	shader->sendModelView( matrices->getModelView() );

	m->draw();

	Shader::unbind();
}


/*!
 * Draw entities to the back buffer with interpolation.
 *
 * @param alpha amount to blend between last and current frame
 */
void Renderer::renderEntities( double alpha )
{

}


/*!
 * Draw terrain to the back buffer.
 */
void Renderer::renderTerrain( void )
{

}


/*!
 * Draw GUI to the back buffer, ignoring the depth buffer.
 */
void Renderer::renderGUI( void )
{

}


/*!
 * Add an entity to be rendered with interpolation.
 */
void Renderer::addEntity( Entity* entity )
{
	entities->insert(
		std::pair<int, LerpMesh*>(
			1, //entity->getID(),
			static_cast<LerpMesh*> ( entity->getMesh() )
		)
	);
}


/*!
 * Add a terrain section to be rendererd.
 */
void Renderer::addTerrain( Terrain* terrain )
{
	this->terrain->insert(
		std::pair<int, Mesh*>(
			1, //terrain->getID(),
			terrain->getMesh()
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
			1, //gui->getID(),
			gui->getMesh()
		)
	);
}

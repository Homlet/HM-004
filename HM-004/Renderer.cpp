#include "MacroTime.h"
#include "MacroWindow.h"

#include "Base.h"
#include "Renderer.h"

#include "Mesh.h"
#include "Entity.h"
#include "Terrain.h"
#include "GUIElement.h"


/*!
 * Sets up the OpenGL context.
 *
 * @param window GLFWwindow whose context to render to. Once set, this cannot be changed.
 */
Renderer::Renderer( GLFWwindow* window ) :
	window( window )
{
}


/*!
 * Renders one frame to the back buffer, blending entity meshes by given alpha.
 */
void Renderer::render( double alpha )
{
	// DO NOWT
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

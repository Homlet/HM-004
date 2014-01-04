#include "Base.h"
#include "State.h"

#include "Player.h"
#include "Entity.h"


State::State( void ) :
	entities( new std::list<Entity*>() )
{
}


State::~State( void )
{
	for ( auto e : *entities )
		delete e;
	delete entities;

	if ( player )
		delete player;
}


/*!
 * Create state specific objects.
 */
void State::create( void )
{
	player = new DebugPlayer();
}


/*!
 * Updates all children of the state.
 */
void State::update( double delta, double elapsed )
{
	for ( auto e : *entities )
		e->update( delta, elapsed );

	if ( player )
		player->update( delta, elapsed );
}


/*!
 * Adds an entity to the update list.
 */
void State::add( Entity* e )
{
	entities->push_back( e );
}


/*!
 * Removes an entity from the update list.
 */
void State::remove( Entity* e )
{
	entities->remove( e );
}


/*!
 * Return a reference to the player.
 */
Player* State::getPlayer( void )
{
	if ( !player )
		player = new DebugPlayer();

	return player;
}

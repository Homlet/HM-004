#pragma once


class Entity;
class Player;


class State {
private:
	std::list<Entity*>* entities;

protected:
	Player* player{ nullptr };

public:
	State( void );
	virtual ~State( void );

	virtual void create( void ); // TODO: make pure virtual.
	virtual void update( double delta, double elapsed );

	void add( Entity* e );
	void remove( Entity* e );

	Player* getPlayer( void );
};

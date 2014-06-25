#pragma once


class Camera;


class Player {
protected:
	Player( bool directional = true );

	Camera* camera;

public:
	virtual ~Player( void );
	
	virtual void interpolate( void ) = 0;
	virtual void update( double delta, double elapsed ) = 0;

	Camera* getCamera( void ) const;
};


class DebugPlayer : public Player {
private:
	double speed{ 4.3 };

	double mx{ 0 }, my{ 0 };

public:
	DebugPlayer( void );
	
	void interpolate( void );
	void update( double delta, double elapsed );
};

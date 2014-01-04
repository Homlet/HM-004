#pragma once


class Renderer;
class State;
class Input;


class Core {
private:
	Core( void );

	static bool exists;
	static Core* instance;
	static Core* getInstance( void );

	Renderer* renderer;
	State*    state{ nullptr };
	Input*    input;

public:
	static void run( void );
	static void cheapUpdate( void );
	static void cheapProgress( std::string name, float progress = 0 );

	static Renderer* getRenderer( void );

	static State* getState( void );
	static void   setState( State* next );

	static Input* getInput( void );
};

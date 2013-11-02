#pragma once


#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#define DEBUG_MODE


#include <fstream>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>

#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <ext.hpp>


class Renderer;


bool readTextFile( std::string url, std::string& output );
bool  getFilename( std::string url, std::string& output );


class Core {
private:
	Core( void );
	
	static bool exists;
	static Core* instance;
	static Core* getInstance( void );

	GLFWwindow* window;
	Renderer* renderer;

public:
	static void run( void );
	static Renderer* getRenderer( void );
};

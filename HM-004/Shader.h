#pragma once


#define GLSL(src) "#version 400\n" #src


#include "pugixml/pugixml.hpp"


template <typename T>
class ResourceLoader;


class Shader {
private:
	GLuint ID;
	std::string name;

	bool compiled;

	GLint uniformLoc_MV,
		  uniformLoc_P,
		  uniformLoc_N;

	GLuint compileShader( std::string src, GLenum type );

public:
	Shader( std::string name, std::string vert_src, std::string frag_src );

	       void   bind( void );
	static void unbind( void );
	
	void  sendModelView( glm::mat4 mv );
	void sendProjection( glm::mat4 p );
	void     sendNormal( glm::mat3 n );

	bool isCompiled( void );
};


template <>
class ResourceLoader<Shader> {
public:
	Shader* load( std::string url );
};

#pragma once


#define GLSL(src) "#version 400\n" #src


#include "pugixml/pugixml.hpp"


template <typename T>
class ResourceLoader;


class Shader {
private:
	static GLuint bound;
	       GLuint ID;
	  std::string name;

	bool compiled;

	GLint uniformLoc_MV,
		  uniformLoc_P,
		  uniformLoc_N,
		  uniformLoc_lightDir,
		  uniformLoc_lightColor,
		  uniformLoc_sMV,
		  uniformLoc_sP;

	GLuint compileShader( std::string src, GLenum type );

public:
	Shader( std::string name, std::string vert_src, std::string frag_src );

	       void   bind( void );
	static void unbind( void );
	      bool isBound( void );
	
	bool  usesModelView( void );
	void  sendModelView( glm::mat4 mv );

	bool usesProjection( void );
	void sendProjection( glm::mat4 p );

	bool     usesNormal( void );
	void     sendNormal( glm::mat3 n );

	bool   usesLightDir( void );
	void   sendLightDir( glm::vec3 light_dir );
	void sendLightColor( glm::vec3 light_color );

	bool   usesShadowMatrices( void );
	void  sendShadowModelView( glm::mat4 smv );
	void sendShadowProjection( glm::mat4 sp );

	bool isCompiled( void );
};


template <>
class ResourceLoader<Shader> {
public:
	Shader* load( std::string url );
};

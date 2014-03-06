#pragma once


#include "pugixml\pugixml.hpp"


template <typename T>
class ResourceLoader;


class Texture {
private:
	GLuint ID;
	std::string name;

protected:
	static unsigned char* flipY(
		unsigned char* data,
		int width,
		int height,
		int depth
	);

	virtual GLenum getType( void ) = 0;

public:
	Texture( std::string name );

	       void   bind( void );
	static void unbind( void );
};

class Texture_2D : public Texture {
private:
	GLenum getType( void )
	{
		return GL_TEXTURE_2D;
	}

	void bufferData(
		unsigned char* data,
		glm::ivec2 dimensions,
		GLenum format_inner,
		GLenum format_outer
	);

public:
	Texture_2D( std::string name, std::string url );
};


class Texture_2D_Array : public Texture {
private:
	GLenum getType( void )
	{
		return GL_TEXTURE_2D_ARRAY;
	}

	void bufferData(
		unsigned char* data,
		glm::ivec2 dimensions,
		int level,
		GLenum format_inner,
		GLenum format_outer
	);

public:
	Texture_2D_Array( std::string name, std::vector<std::string> urls );
};


template <>
class ResourceLoader<Texture> {
public:
	Texture* load( std::string url );
};

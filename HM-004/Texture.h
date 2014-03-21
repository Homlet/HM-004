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

	GLenum type;

public:
	Texture( std::string name, GLenum type );

	       void   bind( void );
	static void unbind( void );
};

class Texture_2D : public Texture {
public:
	Texture_2D( std::string name, std::string url );
};


class Texture_2D_Array : public Texture {
public:
	Texture_2D_Array( std::string name, std::vector<std::string> urls );
};


template <>
class ResourceLoader<Texture> {
public:
	Texture* load( std::string url );
};

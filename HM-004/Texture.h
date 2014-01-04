#pragma once


template <typename T>
class ResourceLoader;


class Texture {
private:
	GLuint ID;
	std::string name;

public:
	Texture( std::string name, std::string url );
	Texture(
		std::string name,
		unsigned char* data,
		int width,
		int height,
		GLenum format_inner,
		GLenum format_outer
	);

	void bufferData( 
		unsigned char* data,
		int width,
		int height,
		GLenum format_inner,
		GLenum format_outer
	);

	unsigned char* flipY(
		unsigned char* data,
		int width,
		int height,
		int depth
	);

	       void   bind( void );
	static void unbind( void );
};


template <>
class ResourceLoader<Texture> {
public:
	Texture* load( std::string url );
};

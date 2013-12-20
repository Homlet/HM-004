#include "Base.h"
#include "Texture.h"

#include "File.h"


/**
 * Returns raw RGBA/RGB image data as an unsigned char array, loaded from a .tga image file
 *
 * @param url Path to a .tga image file
 * @param width  Integer pointer for width
 * @param height Integer pointer for height
 * @param depth  Integer pointer for bit depth
 */
unsigned char* Texture::loadTGA( std::string url, int* width, int* height, int* depth )
{
	std::ifstream img_stream( url );
	
	if ( img_stream.is_open() )
	{
		// Read and store header.
		unsigned char header[18];

		for ( int i = 0; i < 18; i++ )
			header[i] = (unsigned char) img_stream.get();

		// Check image is type 2 (colour).
		if ( header[2] != 2 )
		{
			img_stream.close();
			throw std::exception( ( "Invalid image type: " + url ).c_str() );

			return 0;
		}

		// Get image size and pixel depth.
		*width  = header[13] * 256 + header[12];
		*height = header[15] * 256 + header[14];
		*depth  = header[16] / 8;

		// Check bit depth is 3 (BGR) or 4 (BGRA) bytes per pixel.
		if ( *depth != 3 && *depth != 4 )
		{
			img_stream.close();
			throw std::exception( ( "Invalid image bit depth: " + url ).c_str() );

			return 0;
		}

		int size = *width * *height * *depth;
		unsigned char* data = new unsigned char[size];

		// Ignore Image ID field.
		img_stream.ignore( header[0] );

		// Read uncompressed data.
		for ( int i = 0; i < size; i++ )
			data[i] = (unsigned char) img_stream.get();

		// Swap red and blue colour components (BGR/BGRA to RGB/RGBA).
		unsigned char temp;
		for ( int i = 0; i < size; i += *depth )
		{
			temp = data[i];
			data[i] = data[i + 2];
			data[i + 2] = temp;
		}

		img_stream.close();
		
		return data;
	}

	throw std::exception( ( "Unable to open file: " + url ).c_str() );
	return 0;
}


/*!
 * Creates a texture object and fills it with data from a targa file.
 */
Texture::Texture( std::string name, std::string url ) :
	name( name )
{
	int width, height, depth;
	unsigned char* data = loadTGA( url, &width, &height, &depth );

	GLenum format = ( depth == 3 ) ? GL_RGB : GL_RGBA;

	bufferData( data, width, height, GL_RGBA8, format );
}


/*!
 * Creates a texture object using data.
 */
Texture::Texture(
	std::string name,
	unsigned char* data,
	int width,
	int height,
	GLenum format_inner,
	GLenum format_outer
) :
	name( name )
{
	glGenTextures( 1, &ID );

	bufferData( data, width, height, format_inner, format_outer );
}


/*!
 * Buffers image data to the texture object.
 */
void Texture::bufferData( unsigned char* data, int width, int height, GLenum format_inner, GLenum format_outer )
{
	bind();
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		glTexImage2D( GL_TEXTURE_2D, 0, format_inner, width, height, 0, format_outer, GL_UNSIGNED_BYTE, &data[0] );
		glGenerateMipmap( GL_TEXTURE_2D );
	}
	unbind();
}


/*!
 * Binds the texture object for rendering.
 */
void Texture::bind( void )
{
	glActiveTexture( GL_TEXTURE0       );
	glBindTexture(   GL_TEXTURE_2D, ID );
}


/*!
 * Unbind the current texture object.
 */
void Texture::unbind( void )
{
	glActiveTexture( GL_TEXTURE0      );
	glBindTexture(   GL_TEXTURE_2D, 0 );
}


/*!
 * Loads and returns a texture object from a .tga file.
 */
Texture* ResourceLoader<Texture>::load( std::string url )
{
	std::string name;
	getFilename( url, name );

	return new Texture( name, url );
}

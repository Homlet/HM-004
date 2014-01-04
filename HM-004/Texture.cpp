#include "Base.h"
#include "Texture.h"

#include "File.h"

#include "stb_image.h"


/*!
 * Creates a texture object and fills it with data from a targa file.
 */
Texture::Texture( std::string name, std::string url ) :
	name( name )
{
	int width, height, depth;
	unsigned char* data = stbi_load( url.c_str(), &width, &height, &depth, 0 );
	data = flipY( data, width, height, depth );

	GLenum format = ( depth == 3 ) ? GL_RGB : GL_RGBA;

	bufferData( data, width, height, GL_RGBA8, format );

	stbi_image_free( data );
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
 * Flips an image file along its y axis.
 */
unsigned char* Texture::flipY( unsigned char* data, int width, int height, int depth )
{
	int w_comp = width * depth;

	for ( int i = 0, j = height - 1; i < height / 2; i++, j-- )
	{
		// Tranfer top line to buffer.
		unsigned char* buffer = new unsigned char[w_comp];
		for ( int k = 0; k < w_comp; k++ )
			buffer[k] = data[i*w_comp+k];

		// Transfer bottom line to top.
		for ( int k = 0; k < w_comp; k++ )
			data[i*w_comp+k] = data[j*w_comp+k];

		// Transfer buffer to bottom line.
		for ( int k = 0; k < w_comp; k++ )
			data[j*w_comp+k] = buffer[k];
	}

	return data;
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

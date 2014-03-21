#include "Base.h"
#include "Texture.h"

#include "File.h"

#include "stb_image.h"


/*!
 * Creates an empty OpenGL texture object.
 */
Texture::Texture( std::string name, GLenum type ) :
	name( name ), type( type )
{
	glGenTextures( 1, &ID );

	bind();

	glTexParameteri( type, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );
	glTexParameteri( type, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( type, GL_TEXTURE_WRAP_T, GL_REPEAT );

	float anisotropy;
	glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy );
	glTexParameterf( type, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy );

	unbind();
}


/*!
 * Creates a 2D texture object and fills it with data from a targa file.
 */
Texture_2D::Texture_2D( std::string name, std::string url ) :
	Texture( name, GL_TEXTURE_2D )
{
	int width, height, depth;
	GLenum format;

	unsigned char* data = stbi_load( url.c_str(), &width, &height, &depth, 0 );
	data = flipY( data, width, height, depth );
	format = ( depth == 3 ) ? GL_RGB : GL_RGBA;

	glTexImage2D( type, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, &data[0] );
	glGenerateMipmap( type );

	stbi_image_free( data );
}


/*!
 * Create a 2D array texture object and fills it with data from targa files.
 */
Texture_2D_Array::Texture_2D_Array( std::string name, std::vector<std::string> urls ) :
	Texture( name, GL_TEXTURE_2D_ARRAY )
{
	int width, height, depth;
	GLenum format;

	unsigned char* data = stbi_load( urls[0].c_str(), &width, &height, &depth, 0 );
	data = flipY( data, width, height, depth );
	format = ( depth == 3 ) ? GL_RGB : GL_RGBA;

	bind();
	glTexImage3D( type, 0, GL_RGBA8, width, height, (GLsizei) urls.size(), 0, format, GL_UNSIGNED_BYTE, 0 );
	glTexSubImage3D(
		type, 0, 0, 0, 0,
		width, height, 1,
		format, GL_UNSIGNED_BYTE, &data[0]
	);

	for ( size_t i = 1; i < urls.size(); i++ )
	{
		data = stbi_load( urls[i].c_str(), &width, &height, &depth, 0 );
		data = flipY( data, width, height, depth );

		glTexSubImage3D(
			type, 0,
			0, 0, (int) i,
			width, height, 1,
			format, GL_UNSIGNED_BYTE, &data[0]
		);
	}
	
	stbi_image_free( data );
	glGenerateMipmap( type );
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
	unbind();

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( type, ID );
}


/*!
 * Unbind the current texture object.
 */
void Texture::unbind( void )
{
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, 0 );
	glBindTexture( GL_TEXTURE_2D_ARRAY, 0 );
}


/*!
 * Loads and returns a texture object from a .tga file.
 */
Texture* ResourceLoader<Texture>::load( std::string url )
{
	pugi::xml_document xml;
	pugi::xml_parse_result result = xml.load_file( url.c_str() );

	if ( !result )
	{
		throw std::exception( (
			"Failed to parse texture xml file at \"" + url + "\".\n" +
			result.description() + "."
		).c_str() );
	}

	pugi::xml_node root = xml.first_child();

	std::string name = root.attribute( "name" ).as_string();
	std::string type = root.attribute( "type" ).as_string();

	if ( !strcmp( type.c_str(), "GL_TEXTURE_2D" ) )
	{
		std::string img_url = root.child( "image" ).attribute( "url" ).as_string();
		return new Texture_2D( name, img_url );
	} else if ( !strcmp( type.c_str(), "GL_TEXTURE_2D_ARRAY" ) )
	{
		std::vector<std::string> img_urls;
		pugi::xml_node img = root.child( "image" );
		while ( img )
		{
			img_urls.push_back( img.attribute( "url" ).as_string() );
			img = img.next_sibling();
		}
		return new Texture_2D_Array( name, img_urls );
	} else
	{
		throw std::exception( (
			"Invalid texture type in file \"" + url + "\"."
		).c_str() );
	}
}

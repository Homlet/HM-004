#include "Base.h"
#include "Shader.h"


/*!
 * Attempts to compile a shader.
 *
 * @return ID of shader if successful, 0 if not.
 */
GLuint Shader::compileShader( std::string src, GLenum type )
{
	GLuint id = glCreateShader( type );

	const GLchar* char_src = src.c_str();
	glShaderSource( id, 1, &char_src, 0 );

	glCompileShader( id );

	// Print shader info log.
	GLint result, length;
	glGetShaderiv( id, GL_COMPILE_STATUS, &result );
	glGetShaderiv( id, GL_INFO_LOG_LENGTH, &length );
	std::vector<char> log( length );
	glGetShaderInfoLog( id, length, 0, &log[0] );

	std::cout << "    " << &log[0] << std::endl;

	return ( result == GL_FALSE ) ? 0 : id;
}


/*!
 * Attempts to compile the given shaders, and links them to a program.
 */
Shader::Shader( std::string name, std::string vert_src, std::string frag_src ) :
	ID( glCreateProgram() ),
	name( name )
{
	std::cout << "  Compiling shader " + name << std::endl;

	// Compile vertex shader.
	GLuint vert;
	if ( !( vert = compileShader( vert_src, GL_VERTEX_SHADER ) ) )
	{
		glDeleteShader( vert );

		glDeleteProgram( ID );

		throw std::exception( "Shader compilation failed." );
	}

	// Compile fragment shader.
	GLuint frag;
	if ( !( frag = compileShader( frag_src, GL_FRAGMENT_SHADER ) ) )
	{
		glDeleteShader( vert );
		glDeleteShader( frag );

		glDeleteProgram( ID );

		throw std::exception( "Shader compilation failed." );
	}

	// Attach shaders to program, and link program.
	glAttachShader( ID, vert );
	glAttachShader( ID, frag );
	glLinkProgram( ID );

	// Print program info log
	GLint result, length;
	glGetProgramiv( ID, GL_LINK_STATUS, &result );
	glGetProgramiv( ID, GL_INFO_LOG_LENGTH, &length );
	std::vector<char> log( length );
	glGetProgramInfoLog( ID, length, 0, &log[0] );
	std::cout << "  " << &log[0];
	
	// Dispose of shaders.
	glDeleteShader( vert );
	glDeleteShader( frag );

	// Setup uniforms.
	bind();
	{
		uniformLoc_MV = glGetUniformLocation( ID, "u_MV" );
		uniformLoc_P  = glGetUniformLocation( ID, "u_P"  );
		uniformLoc_N  = glGetUniformLocation( ID, "u_N"  );
	}
	unbind();

	compiled = true;
}


/*!
 * Use the shader associated with this object for rendering.
 */
void Shader::bind( void )
{
	if ( compiled )
	{
		glUseProgram( ID );
	}
}


/*!
 * Unbind the current shader.
 */
void Shader::unbind( void )
{
	glUseProgram( 0 );
}


/*!
 * Send modelview matrix to the GPU as a uniform.
 */
void Shader::sendModelView( glm::mat4 mv )
{
	glUniformMatrix4fv( uniformLoc_MV, 1, false, glm::value_ptr( mv ) );
}


/*!
 * Send projection matrix to the GPU as a uniform.
 */
void Shader::sendProjection( glm::mat4 p )
{
	glUniformMatrix4fv( uniformLoc_P, 1, false, glm::value_ptr( p ) );
}


/*!
 * Send inverse-transpose model (normal) matrix to the GPU as a uniform.
 */
void Shader::sendNormal( glm::mat3 n )
{
	glUniformMatrix3fv( uniformLoc_N, 1, false, glm::value_ptr( n ) );
}


/*!
 * Returns true if the shader has successfully compiled.
 */
bool Shader::isCompiled( void )
{
	return compiled;
}



/*!
 * Loads a shader from an xml file.
 */
Shader* ResourceLoader<Shader>::load( std::string url )
{
	pugi::xml_document xml;
	pugi::xml_parse_result result = xml.load_file( url.c_str() );

	if ( !result )
	{
		throw std::exception( (
			"Failed to parse shader xml file at \"" + url + "\".\n" +
			result.description() + "."
		).c_str() );
	}

	pugi::xml_node root = xml.first_child();

	std::string name     = root.attribute( "name" ).as_string();
	std::string vert_url =   root.child( "vertex" ).attribute( "url" ).as_string();
	std::string frag_url = root.child( "fragment" ).attribute( "url" ).as_string();
	std::string vert, frag;

	readTextFile( vert_url, vert );
	readTextFile( frag_url, frag );

	if ( vert.empty() || frag.empty() )
	{
		throw std::exception( (
			"Failed to read one or more shader sources for \"" + name + "\".\n"
		).c_str() );
	}

	return new Shader( name, vert, frag );
}

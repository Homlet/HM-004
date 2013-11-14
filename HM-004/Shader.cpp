#include "Base.h"
#include "Shader.h"


/*!
 * Stores the ID of the currently bound shader.
 */
GLuint Shader::bound = 0;


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

	std::cout << "    " << &log[0] << "\n";

	return ( result == GL_FALSE ) ? 0 : id;
}


/*!
 * Attempts to compile the given shaders, and links them to a program.
 */
Shader::Shader( std::string name, std::string vert_src, std::string frag_src ) :
	ID( glCreateProgram() ),
	name( name )
{
	std::cout << "  Compiling shader " + name + "\n";

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
		uniformLoc_MV         = glGetUniformLocation( ID, "u_MV"         );
		uniformLoc_P          = glGetUniformLocation( ID, "u_P"          );
		uniformLoc_N          = glGetUniformLocation( ID, "u_N"          );
		uniformLoc_lightDir   = glGetUniformLocation( ID, "u_lightDir"   );
		uniformLoc_lightColor = glGetUniformLocation( ID, "u_lightColor" );
		uniformLoc_sMV        = glGetUniformLocation( ID, "u_sMV"        );
		uniformLoc_sP         = glGetUniformLocation( ID, "u_sP"         );
		
		GLuint sampler_2D       = glGetUniformLocation( ID, "u_2D"       );
		GLuint sampler_2DShadow = glGetUniformLocation( ID, "u_2DShadow" );

		if ( sampler_2D != -1 )
			glUniform1i( sampler_2D, 0 );
		if ( sampler_2DShadow != -1 )
			glUniform1i( sampler_2DShadow, 1 );
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
		glUseProgram( bound = ID );
	}
}


/*!
 * Unbind the current shader.
 */
void Shader::unbind( void )
{
	glUseProgram( bound = 0 );
}


/*!
 * Returns true if this shader is currently bound for rendering.
 */
bool Shader::isBound( void )
{
	return bound == ID;
}


/*!
 * Returns true if this shader uses the modelview matrix uniform.
 */
bool Shader::usesModelView( void )
{
	return uniformLoc_MV != -1;
}


/*!
 * Macro to bind shaders when sending uniforms, then unbind only if they weren't
 * bound previously.
 */
#define BIND_LOCK(a)	bool wasBound = true;      \
						if ( !isBound() )          \
						{                          \
							bind();                \
							wasBound = false;      \
						}                          \
						a                          \
						if ( !wasBound ) unbind();


/*!
 * Send modelview matrix to the GPU as a uniform.
 */
void Shader::sendModelView( glm::mat4 mv )
{
	BIND_LOCK( glUniformMatrix4fv( uniformLoc_MV, 1, false, glm::value_ptr( mv ) ); )
}


/*!
 * Returns true if this shader uses the projection matrix uniform.
 */
bool Shader::usesProjection( void )
{
	return uniformLoc_P != -1;
}


/*!
 * Send projection matrix to the GPU as a uniform.
 */
void Shader::sendProjection( glm::mat4 p )
{
	BIND_LOCK( glUniformMatrix4fv( uniformLoc_P, 1, false, glm::value_ptr( p ) ); )
}


/*!
 * Returns true if this shader uses the normal matrix uniform.
 */
bool Shader::usesNormal( void )
{
	return uniformLoc_N != -1;
}


/*!
 * Send inverse-transpose modelview (normal) matrix to the GPU as a uniform.
 */
void Shader::sendNormal( glm::mat3 n )
{
	BIND_LOCK( glUniformMatrix3fv( uniformLoc_N, 1, false, glm::value_ptr( n ) ); )
}


/*!
 * Returns true if this shader uses directional lighting uniforms.
 */
bool Shader::usesLightDir( void )
{
	return uniformLoc_lightDir != -1 && uniformLoc_lightColor != -1;
}


/*!
 * Send the light direction vector to the GPU as a uniform.
 */
void Shader::sendLightDir( glm::vec3 light_dir )
{
	BIND_LOCK( glUniform3fv( uniformLoc_lightDir, 1, glm::value_ptr( light_dir ) ); )
}


/*!
 * Send the light color to the GPU as a uniform.
 */
void Shader::sendLightColor( glm::vec3 light_color )
{
	BIND_LOCK( glUniform3fv( uniformLoc_lightColor, 1, glm::value_ptr( light_color ) ); )
}


/*!
 * Returns true if this shader uses matrices to sample a shadow map.
 */
bool Shader::usesShadowMatrices( void )
{
	return uniformLoc_sMV != -1 && uniformLoc_sP != -1;
}


/*!
 * Send shadow modelview matrix to the GPU as a uniform.
 */
void Shader::sendShadowModelView( glm::mat4 smv )
{
	BIND_LOCK( glUniformMatrix4fv( uniformLoc_sMV, 1, false, glm::value_ptr( smv ) ); )
}


/*!
 * Send shadow projection matrix to the GPU as a uniform.
 */
void Shader::sendShadowProjection( glm::mat4 sp )
{
	BIND_LOCK( glUniformMatrix4fv( uniformLoc_sP, 1, false, glm::value_ptr( sp ) ); )
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

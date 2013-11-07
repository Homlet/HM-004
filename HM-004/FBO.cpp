#include "MacroWindow.h"

#include "Base.h"
#include "FBO.h"


/*!
 * Stores the ID of the currently bound FBO.
 */
GLuint FBO::bound = 0;


/*!
 * Creates a new FBO on the GPU.
 *
 * @param derived If the FBO is actually an instance of a derived type (eg. ShadowMap). In
 *                this case, no setup is performed as this is the job of the derived constructor.
 *                It is false by default, so you usually don't need to bother with it.
 */
FBO::FBO( bool derived )
{
	if ( !derived )
	{
		glGenFramebuffers(  1, &ID        );
		glGenTextures(      1, &textureID );
		glGenRenderbuffers( 1, &depthID   );

		bind();
		bindTexture();
		{
			glActiveTexture( GL_TEXTURE0 + 1 );
			glTexImage2D(    GL_TEXTURE_2D, 0, GL_RGB, WIN_W, WIN_H, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

			glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIN_W, WIN_H );

			glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_RENDERBUFFER, depthID );
			glFramebufferTexture(      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureID, 0 );

			GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
			glDrawBuffers( 1, draw_buffers );

			if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
				throw std::exception( "Failed to create framebuffer." );
		}
		unbind();
		unbindTexture();

		glActiveTexture( GL_TEXTURE0 );
	}
}


/*!
 * Binds this FBO on the GPU for rendering.
 */
void FBO::bind( void )
{
	glBindFramebuffer(  GL_FRAMEBUFFER,  bound = ID );
	glBindRenderbuffer( GL_RENDERBUFFER, depthID    );
	glViewport( 0, 0, WIN_W, WIN_H );
}


/*!
 * Unbinds the current FBO.
 */
void FBO::unbind( void )
{
	glBindFramebuffer(  GL_FRAMEBUFFER,  bound = 0 );
	glBindRenderbuffer( GL_RENDERBUFFER, 0         );
	glViewport( 0, 0, WIN_W, WIN_H );
}


/*!
 * Binds the texture to texture unit 1 for rendering.
 */
void FBO::bindTexture( void )
{
	glActiveTexture( GL_TEXTURE0 + 1          );
	glBindTexture(   GL_TEXTURE_2D, textureID );
	glActiveTexture( GL_TEXTURE0              );
}


/*!
 * Unbinds the texture in texture unit 1.
 */
void FBO::unbindTexture( void )
{
	glActiveTexture( GL_TEXTURE0 + 1  );
	glBindTexture(   GL_TEXTURE_2D, 0 );
	glActiveTexture( GL_TEXTURE0      );
}


/*!
 * Creates a new FBO for shadow mapping.
 */
ShadowMap::ShadowMap( void ) :
	FBO( true )
{
	glGenFramebuffers(  1, &ID        );
	glGenTextures(      1, &textureID );
	
	bind();
	bindTexture();
	{
		glActiveTexture( GL_TEXTURE0 + 1 );
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_DEPTH_COMPONENT16,
			2048,
			2048,
			0,
			GL_DEPTH_COMPONENT,
			GL_FLOAT,
			0
		);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
		glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE,   GL_INTENSITY );

		glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureID, 0 );

		glDrawBuffer( GL_NONE );

		if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
			throw std::exception( "Failed to create shadow map framebuffer." );
	}
	unbind();
	unbindTexture();

	glActiveTexture( GL_TEXTURE0 );
}


/*!
 * Binds the shadow map FBO, unbinds the renderbuffer.
 */
void ShadowMap::bind( void )
{
	glBindFramebuffer(  GL_FRAMEBUFFER,  bound = ID );
	glBindRenderbuffer( GL_RENDERBUFFER, 0          );
	glViewport( 0, 0, 2048, 2048 );
}

#pragma once


class FBO {
protected:
	static GLuint bound;
	       GLuint ID, textureID, depthID;

public:
	FBO( bool derived = false );

	virtual void   bind( void );
	static  void unbind( void );
	        void   bindTexture( void );
	static  void unbindTexture( void );
};


class ShadowMap : public FBO {
public:
	ShadowMap( void );

	void bind( void );
};

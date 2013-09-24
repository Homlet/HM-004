#pragma once


class VAO {
private:
	GLuint ID;

public:
	VAO( void );

	       void   bind( void );
	static void unbind( void );
};

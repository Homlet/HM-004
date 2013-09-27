#pragma once


class VAO;


struct vertex {
	GLfloat x,  y,  z,
		   nx, ny, nz,
			s,  t , p;
};


class Mesh {
private:
	GLuint vertexID;
	GLuint indexID;
	GLenum poly_mode;
	int count;

	VAO* vao;

	std::vector<vertex>   vertices;
	std::vector<GLushort> indices;
	
protected:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec4 orientation;

public:
	Mesh(
		std::vector<vertex> vertices,
		std::vector<GLushort> indices,
		GLenum poly_mode
	);

	       void   bind( void );
	static void unbind( void );

	void draw( void );

	virtual void    setPosition( glm::vec3 position );
	virtual void       setScale( glm::vec3 scale );
	virtual void setOrientation( glm::vec4 orientation );

	glm::vec3 getPosition( void );
	glm::vec3 getScale( void );
	glm::vec4 getOrientation( void );

	virtual void translate( glm::vec3 offset );
	virtual void addScale( glm::vec3 factor );
	virtual void rotate( float amount );

	static Mesh* createCube(
		glm::vec3 position,
		glm::vec3 size
	);
};


class LerpMesh : public Mesh {
private:
	glm::vec3 positionOld;
	glm::vec3 scaleOld;
	glm::vec4 orientationOld;

public:
	LerpMesh(
		std::vector<vertex> vertices,
		std::vector<GLushort> indices,
		GLenum poly_mode
	);

	void    setPosition( glm::vec3 position );
	void       setScale( glm::vec3 scale );
	void setOrientation( glm::vec4 orientation );

	glm::vec3 getOldPosition( void );
	glm::vec3 getOldScale( void );
	glm::vec4 getOldOrientation( void );

	void translate( glm::vec3 offset );
	void addScale( glm::vec3 factor );
	void rotate( float amount );
};

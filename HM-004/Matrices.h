#pragma once


class Matrices {
private:
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 modelView;

public:
	Matrices( void );

	void loadIdentity( void );
	void    translate( glm::vec3 translation );
	void        scale( glm::vec3 factor );
	void       rotate( glm::vec3 axis, float angle );

	void lookAt(
		glm::vec3 position,
		glm::vec3 target,
		glm::vec3 up
	);
	void setProjection(
		double fov,
		double ratio,
		double near,
		double far
	);

	void computeModelView( void );
	glm::mat4 getModelView( void ) const;
	glm::mat4 getProjection( void ) const;
};

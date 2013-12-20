#pragma once


class Matrices;


class Camera {
private:
	Matrices* matrices;

	bool directional;
	glm::vec3 position;
	glm::vec3 target, direction;
	glm::vec3 up;
	double fov;
	double ratio;
	double near, far;

public:
	Camera( bool directional = false );

	Matrices* getMatrices( void ) const;

	void setProjection(
		double fov,
		double ratio,
		double near,
		double far
	);

	void setDirectional( bool directional );
	void setDirection( glm::vec3 direction );

	void setTarget( glm::vec3 target );

	void moveTo( glm::vec3 position );
	void moveBy( glm::vec3 offset );
	void moveRelative( glm::vec3 offset );
};

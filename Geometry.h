#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include <glm\glm.hpp>
#include"ShaderProgram.h"
#include "Mesh.h"


struct Geometry{
	virtual void setPosition(const glm::vec3& newPos) = 0;
	virtual bool isInside(const glm::vec3& point) = 0;
};

struct Plane : public Geometry {
	glm::vec3 normal;
	glm::vec3 plane_point;
	int _case;
	float dconst;
	Plane(){};
	~Plane() {};
	Plane(const glm::vec3& point, const glm::vec3& normalVect );
	Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2);




	void setPosition(const glm::vec3& newPos);
	bool isInside(const glm::vec3& point);
	float distPoint2Plane(const glm::vec3& point);
	glm::vec3 closestPointInPlane(const glm::vec3& point);
	bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
	Mesh *model;
	 
	ShaderProgram prog;
	glm::mat4 loc;
	//void render_plane();

	

};	

struct Triangle : public Plane {
	glm::vec3 vertex1, vertex2, vertex3;
	Triangle(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2, ShaderProgram &program);
	~Triangle() {};
	//void setPosition(const glm::vec3& newPos);
	bool isInside(const glm::vec3& point);
//	bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
	void render_triangle();
	GLuint VAO1;

};

struct Sphere : public Geometry {
	glm::vec3 center;
	float radi;
	Sphere(const glm::vec3& point, const float& radious , ShaderProgram &Prog);
	~Sphere() {};
	void setPosition(const glm::vec3& newPos);
	bool isInside(const glm::vec3& point);
	Mesh *model;

	ShaderProgram prog;
	glm::mat4 loc;
	void render_ball();
	float a, b, c;
	Plane *Contact_plane;
	glm::vec3 normal_;
	
	//bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
};

#endif

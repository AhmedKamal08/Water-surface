#pragma once
#include "Geometry.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

//****************************************************
// Plane
//****************************************************

Plane::Plane(const glm::vec3& point, const glm::vec3& normalVect) {
	plane_point = point;
	normal = glm::normalize(normalVect);
	dconst = -glm::dot(point, normal);
	//_case = plane_case_;
	//prog = program;
	  	 
};

Plane::Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2){
	plane_point = point0;
	glm::vec3 v1 = point1 - point0;
	glm::vec3 v2 = point2 - point0;
	normal = glm::normalize(glm::cross(v1, v2));
	dconst = -glm::dot(point0, normal);
};

void Plane::setPosition(const glm::vec3& newPos){
	dconst = -glm::dot(newPos, normal);
};

bool Plane::isInside(const glm::vec3& point){
	float dist;
	dist = glm::dot(point, normal) + dconst;
	if (dist > 1.e-7)
		return false;
	else
		return true;
};

float Plane::distPoint2Plane(const glm::vec3& point){
	float dist;
	//return dist = glm::dot(normal,(point - plane_point ));
	/*glm::vec3 temp;


	float    sb, sn, sd;

	sn = -glm::dot(normal, (point - plane_point));
	sd = glm::dot(normal, normal);
	sb = sn / sd;

	temp = point + sb * normal;
	return glm::distance(point, temp);*/
	float d = glm::dot(normal, plane_point);
	return dist = (glm::dot(normal, point) - d) / glm::dot(normal, normal);
};

glm::vec3 Plane::closestPointInPlane(const glm::vec3& point){
	glm::vec3 closestP;
	float r = (-dconst - glm::dot(point, normal));
	return closestP = point + r*normal;
};

bool Plane::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall){
	if (distPoint2Plane(point1)*distPoint2Plane(point2) > 0)	return false;
	float r = (-dconst - glm::dot(point1, normal)) / glm::dot((point2 - point1), normal);
	pTall = (1 - r)*point1 + r*point2;
	return true;
};

 

//****************************************************
// Triangle
//****************************************************

Triangle::Triangle(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2, ShaderProgram &program) {
	
	vertex1 = point0;
	vertex2 = point1;
	vertex3 = point2;
	prog = program;
	
		
	
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);

	GLfloat vertice[] =
	{
		  vertex1.x,vertex1.y,vertex1.z, // triangle 1 : begin
		  vertex2.x,vertex2.y,vertex2.z,
		  vertex3.x,vertex3.y,vertex3.z,
	};
	GLint posLocation;
	GLuint vbo_vertices;
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertice), vertice, GL_STATIC_DRAW);
	posLocation = prog.bindVertexAttribute("position", 3, 3 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(posLocation);

	

}
void Triangle::render_triangle() {
	glBindVertexArray(VAO1);
	loc = glm::mat4(1);
	//glm::mat4 transform = (loc);
	prog.setUniformMatrix4f("model_mat", loc);
	prog.setUniform4f("color", 1, 0, 0.0f, 0.5f);

	glDrawArrays(GL_TRIANGLES, 0, 3);

};

bool Triangle::isInside(const glm::vec3 & point)
{
	float x = 0.5f*glm::length(glm::cross((vertex2 - point), (vertex3-point)))+
			  0.5f*glm::length(glm::cross((point - vertex1), (vertex3 - vertex1)))+
			  0.5f*glm::length(glm::cross((vertex2 - vertex1), (point - vertex1)))-
		      0.5f*glm::length(glm::cross((vertex2 - vertex1), (vertex3 - vertex1)));


	if (x >=0.001f) {
		return false;
	}
	else {
		return true;
	}
}




// Sphere
//****************************************************

Sphere::Sphere(const glm::vec3 & point, const float & radious, ShaderProgram &Prog)
{
	center = point;
	prog = Prog;
	loc = glm::translate(glm::mat4(1.0f), center);
	radi = radious;
	model = new  Mesh("models/ball2.obj", prog, loc);
}

bool Sphere::isInside(const glm::vec3 & point)
{
	//line_direc = glm::normalize(line_direc);
	//glm::vec3 test = point - center;
	float temp = glm::length(point- center);
	if (temp <= radi+radi) {
		

		 normal_ = point - center;
		 normal_ = glm::normalize(normal_);
		Contact_plane = new Plane(point, normal_);

		return true;

	}
	else {
		return false;
		}
	}

void Sphere::render_ball()
{
	prog.setUniform4f("color", 1, 0, 1.0f, 0.5f);

	model->render();
}
void Sphere::setPosition(const glm::vec3& newPos) {
	 
};
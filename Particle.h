#ifndef _PARTICLE_H
#define _PARTICLE_H

#include <glm\glm.hpp>
#include"ShaderProgram.h"
#include "Mesh.h"
#include<random>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
class Particle
{
public:
	enum class UpdateMethod : std::int8_t { EulerOrig, EulerSemi, Verlet };

	Particle();
	Particle(const float& x, const float& y, const float& z , ShaderProgram &Prog);
//	Particle(glm::vec3 pos, glm::vec3 vel, float bouncing = 1.0f, bool fixed = false, int lifetime = -1, glm::vec3 force = glm::vec3(0, 0, 0));
	~Particle();
	//setters
	void setPosition(const float& x, const float& y, const float& z);
	void setPosition(glm::vec3 pos);
	void setPreviousPosition(const float& x, const float& y, const float& z);
	void setPreviousPosition(glm::vec3 pos);
	void addSpringForce(Particle *p1);
	
	void setVelocity(const float& x, const float& y, const float& z);
	void setVelocity(glm::vec3 vel);
	void addVelocity(glm::vec3 vel);
	void setForce(const float& x, const float& y, const float& z);
	void setForce(glm::vec3 force);
	void setBouncing(float bouncing);
	void setMass(float mass);
	void setLifetime(float lifetime);
	void setFixed(bool fixed);


	Particle *bot;
	Particle *top;
	Particle *right;
	Particle *left;
	Particle *bot_right;
	Particle *bot_left;
	Particle *top_right;
	Particle *top_left;
	
	Particle *top_top;
	Particle *left_left;
	Particle *bot_bot;
	Particle *right_right;

	Particle *front;
	Particle *back;
	 





	//getters
	glm::vec3 getCurrentPosition();
	glm::vec3 getPreviousPosition();

	glm::vec3 getOriginPos();
	glm::vec3 getForce();
	glm::vec3 getVelocity();
	float getBouncing();
	float getMass();
	float getLifetime();
	bool isFixed();

	//other
	void addSpringForce(int springforce,int damp);
	void addSpringForce2(int springforce, int damp);

	void addSpringForce3(int springforce, int damp);

	void addForce(glm::vec3 force);
	void addForce(const float& x, const float& y, const float& z);
	void updateParticle(const float& dt, UpdateMethod method = UpdateMethod::Verlet);
	void updateSpringForce();

private:
	glm::vec3 m_newpos;
	glm::vec3 m_currentPosition;
	glm::vec3 m_previousPosition;
	glm::vec3 m_force;
	glm::vec3 m_Springforce_bot;
	glm::vec3 m_Springforce_top;
	glm::vec3 m_previous;
	glm::vec3 m_Springforce_right;
	glm::vec3 m_Springforce_left;
	glm::vec3 m_Springforce_bot_right;
	glm::vec3 m_Springforce_bot_left;
	glm::vec3 m_Springforce_top_right;
	glm::vec3 m_Springforce_top_left;
	
	glm::vec3 m_Springforce_right_right;
	glm::vec3 m_Springforce_top_top;
	glm::vec3 m_Springforce_left_left;
	glm::vec3 m_Springforce_bot_bot;

	
	glm::vec3 m_spring_force1;
	glm::vec3 m_spring_force2;
	glm::vec3 m_spring_force3;



	glm::vec3 m_velocity;
	glm::vec3 originPos;
	float m_mass;
	float m_bouncing;
	float m_lifetime;
	bool  m_fixed;
	Mesh *model;
	ShaderProgram prog;
	glm::mat4 loc;
	 

};

#endif
#include "Particle.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "Scene.h"
Particle::Particle()
{
}

Particle::Particle(const float& x, const float& y, const float& z,ShaderProgram &program ) :
m_previousPosition(0, 0, 0), m_velocity(0, 0, 0), m_force(0, 0, 0), m_bouncing(1), m_lifetime(50), m_fixed(false)
{
	prog = program;
	loc = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));

	originPos.x = x;
	originPos.y = y;
	originPos.z = z;

	m_Springforce_top = glm::vec3(0);
	m_Springforce_bot = glm::vec3(0);
	m_Springforce_right = glm::vec3(0);
	m_Springforce_left = glm::vec3(0);
	m_Springforce_top_left = glm::vec3(0);
	m_Springforce_bot_left = glm::vec3(0);
	
	m_Springforce_bot_bot = glm::vec3(0);
	m_Springforce_left_left = glm::vec3(0);
	m_Springforce_right_right = glm::vec3(0);
	m_Springforce_top_top = glm::vec3(0);




	m_spring_force1= glm::vec3(0);
	m_spring_force2= glm::vec3(0);
	m_spring_force3 = glm::vec3(0);


	m_Springforce_bot_right = glm::vec3(0);
	m_Springforce_top_right = glm::vec3(0);
	m_currentPosition.x = x;
	m_currentPosition.y = y;
	m_currentPosition.z = z;
	model = new  Mesh("models/ball.obj", prog, loc);
	



}

/*
Particle::Particle(glm::vec3 pos, glm::vec3 vel, float bouncing, bool fixed, int lifetime, glm::vec3 force) :
m_currentPosition(pos), m_previousPosition(pos), m_force(force), m_velocity(vel), m_bouncing(bouncing), m_lifetime(lifetime), m_fixed(fixed)
{
}
*/

Particle::~Particle()
{
}

//setters
void Particle::setPosition(const float& x, const float& y, const float& z)
{
	glm::vec3 pos(x,y,z);
	m_currentPosition =  pos;


	 
}
void Particle::setPosition(glm::vec3 pos)
{
	m_currentPosition = pos;
	
	 
}

void Particle::setPreviousPosition(const float& x, const float& y, const float& z)
{
	glm::vec3 pos(x, y, z);
	m_previousPosition = pos;
}

void Particle::setPreviousPosition(glm::vec3 pos)
{
	m_previousPosition = pos;
}

void Particle::setForce(const float& x, const float& y, const float& z)
{
	glm::vec3 force(x, y, z);
	m_force = force;
}

void Particle::setForce(glm::vec3 force)
{
	m_force = force;
}

void Particle::addForce(const float& x, const float& y, const float& z)
{
	glm::vec3 force(x,y,z);
	m_force += force;
}

void Particle::addForce(glm::vec3 force)
{
	m_force += force;
}

void Particle::addSpringForce(int springforce,int damp)
{
	//m_previous = m_Springforce_top;
	//Particle p1 = p1;
	glm::vec3 currentLength_top;
	glm::vec3 currentLength_bot;
	glm::vec3 currentLength_right;
	glm::vec3 currentLength_left;
	glm::vec3 currentLength_top_left;
	glm::vec3 currentLength_top_right;
	glm::vec3 currentLength_bot_left;
	glm::vec3 currentLength_bot_right;

	glm::vec3 currentLength_top_top;
	glm::vec3 currentLength_bot_bot;
	glm::vec3 currentLength_left_left;
	glm::vec3 currentLength_right_right;





	//glm::vec3 OriginaLength_top;
	float ke = springforce;
	float kd =10;
	if(top !=NULL){
		currentLength_top = top->getCurrentPosition() - m_currentPosition;
		m_Springforce_top = (ke * (glm::length(currentLength_top) - .1f) + kd * glm::dot((top->getVelocity() - m_velocity), (currentLength_top) / (glm::length(currentLength_top))))   *(currentLength_top) / (glm::length(currentLength_top));

	}
	if (top_top != NULL) {
		currentLength_top_top = top_top->getCurrentPosition() - m_currentPosition;
		m_Springforce_top_top = (ke * (glm::length(currentLength_top_top) - .1f) + kd * glm::dot((top_top->getVelocity() - m_velocity), (currentLength_top_top) / (glm::length(currentLength_top_top))))   *(currentLength_top_top) / (glm::length(currentLength_top_top));

	}



	if (bot != NULL) {
		currentLength_bot = bot->getCurrentPosition() - m_currentPosition;

		m_Springforce_bot = (ke * (glm::length(currentLength_bot) - .1f) + kd * glm::dot((bot->getVelocity() - m_velocity), (currentLength_bot) / (glm::length(currentLength_bot))))   *(currentLength_bot) / (glm::length(currentLength_bot));
	}


	if (bot_bot != NULL) {
		currentLength_bot_bot = bot_bot->getCurrentPosition() - m_currentPosition;

		m_Springforce_bot_bot = (ke * (glm::length(currentLength_bot_bot) - .1f) + kd * glm::dot((bot_bot->getVelocity() - m_velocity), (currentLength_bot_bot) / (glm::length(currentLength_bot_bot))))   *(currentLength_bot_bot) / (glm::length(currentLength_bot_bot));
	}

	if (right != NULL) {
		currentLength_right = right->getCurrentPosition() - m_currentPosition;
		m_Springforce_right = (ke * (glm::length(currentLength_right) - .1f) + kd * glm::dot((right->getVelocity() - m_velocity), (currentLength_right) / (glm::length(currentLength_right))))   *(currentLength_right) / (glm::length(currentLength_right));

	}

	if (right_right != NULL) {
		currentLength_right_right = right_right->getCurrentPosition() - m_currentPosition;
		m_Springforce_right_right = (ke * (glm::length(currentLength_right_right) - .1f) + kd * glm::dot((right_right->getVelocity() - m_velocity), (currentLength_right_right) / (glm::length(currentLength_right_right))))   *(currentLength_right_right) / (glm::length(currentLength_right_right));

	}


	if (left != NULL) {
		currentLength_left = left->getCurrentPosition() - m_currentPosition;
		m_Springforce_left = (ke * (glm::length(currentLength_left) - .1f) + kd * glm::dot((left->getVelocity() - m_velocity), (currentLength_left) / (glm::length(currentLength_left))))   *(currentLength_left) / (glm::length(currentLength_left));

	}
	if (left_left != NULL) {
		currentLength_left_left = left_left->getCurrentPosition() - m_currentPosition;
		m_Springforce_left_left = (ke * (glm::length(currentLength_left_left) - .1f) + kd * glm::dot((left_left->getVelocity() - m_velocity), (currentLength_left_left) / (glm::length(currentLength_left_left))))   *(currentLength_left_left) / (glm::length(currentLength_left_left));

	}



	if (top_left != NULL) {
		currentLength_top_left = top_left->getCurrentPosition() - m_currentPosition;
		m_Springforce_top_left = (ke * (glm::length(currentLength_top_left) - .14f) + kd * glm::dot((top_left->getVelocity() - m_velocity), (currentLength_top_left) / (glm::length(currentLength_top_left))))   *(currentLength_top_left) / (glm::length(currentLength_top_left));

	}

	if (top_right != NULL) {
		currentLength_top_right = top_right->getCurrentPosition() - m_currentPosition;
		m_Springforce_top_right = (ke * (glm::length(currentLength_top_right) - .14f) + kd * glm::dot((top_right->getVelocity() - m_velocity), (currentLength_top_right) / (glm::length(currentLength_top_right))))   *(currentLength_top_right) / (glm::length(currentLength_top_right));

	}
	if (bot_left != NULL) {
		currentLength_bot_left = bot_left->getCurrentPosition() - m_currentPosition;
		m_Springforce_bot_left = (ke * (glm::length(currentLength_bot_left) - .14f) + kd * glm::dot((bot_left->getVelocity() - m_velocity), (currentLength_bot_left) / (glm::length(currentLength_bot_left))))   *(currentLength_bot_left) / (glm::length(currentLength_bot_left));

	}
	if (bot_right != NULL) {
		currentLength_bot_right = bot_right->getCurrentPosition() - m_currentPosition;
		m_Springforce_bot_right = (ke * (glm::length(currentLength_bot_right) - .14f) + kd * glm::dot((bot_right->getVelocity() - m_velocity), (currentLength_bot_right) / (glm::length(currentLength_bot_right))))   *(currentLength_bot_right) / (glm::length(currentLength_bot_right));

	}
	m_spring_force1 = m_Springforce_bot + m_Springforce_top + m_Springforce_left + m_Springforce_right;
	m_spring_force2 = m_Springforce_bot_left + m_Springforce_bot_right + m_Springforce_top_left + m_Springforce_top_right;
	m_spring_force3 = m_Springforce_top_top + m_Springforce_right_right + m_Springforce_left_left + m_Springforce_bot_bot;


	float x = 2;
}
void Particle::addSpringForce2(int springforce, int damp)
{
	//m_previous = m_Springforce_top;
	//Particle p1 = p1;
	glm::vec3 currentLength_top;
	glm::vec3 currentLength_bot;
	glm::vec3 currentLength_right;
	glm::vec3 currentLength_left;
	glm::vec3 currentLength_top_left;
	glm::vec3 currentLength_top_right;
	glm::vec3 currentLength_bot_left;
	glm::vec3 currentLength_bot_right;

	 





	//glm::vec3 OriginaLength_top;
	float ke = 60;
	float kd = 10;
	if (top != NULL) {
		currentLength_top = top->getCurrentPosition() - m_currentPosition;
		m_Springforce_top = (ke * (glm::length(currentLength_top) - .1f) + kd * glm::dot((top->getVelocity() - m_velocity), (currentLength_top) / (glm::length(currentLength_top))))   *(currentLength_top) / (glm::length(currentLength_top));

	}
	 


	if (bot != NULL) {
		currentLength_bot = bot->getCurrentPosition() - m_currentPosition;

		m_Springforce_bot = (ke * (glm::length(currentLength_bot) - .1f) + kd * glm::dot((bot->getVelocity() - m_velocity), (currentLength_bot) / (glm::length(currentLength_bot))))   *(currentLength_bot) / (glm::length(currentLength_bot));
	}


	 

	if (right != NULL) {
		currentLength_right = right->getCurrentPosition() - m_currentPosition;
		m_Springforce_right = (ke * (glm::length(currentLength_right) - .1f) + kd * glm::dot((right->getVelocity() - m_velocity), (currentLength_right) / (glm::length(currentLength_right))))   *(currentLength_right) / (glm::length(currentLength_right));

	}

	 


	if (left != NULL) {
		currentLength_left = left->getCurrentPosition() - m_currentPosition;
		m_Springforce_left = (ke * (glm::length(currentLength_left) - .1f) + kd * glm::dot((left->getVelocity() - m_velocity), (currentLength_left) / (glm::length(currentLength_left))))   *(currentLength_left) / (glm::length(currentLength_left));

	}
	 


	if (top_left != NULL) {
		currentLength_top_left = top_left->getCurrentPosition() - m_currentPosition;
		m_Springforce_top_left = (ke * (glm::length(currentLength_top_left) - .14f) + kd * glm::dot((top_left->getVelocity() - m_velocity), (currentLength_top_left) / (glm::length(currentLength_top_left))))   *(currentLength_top_left) / (glm::length(currentLength_top_left));

	}

	if (top_right != NULL) {
		currentLength_top_right = top_right->getCurrentPosition() - m_currentPosition;
		m_Springforce_top_right = (ke * (glm::length(currentLength_top_right) - .14f) + kd * glm::dot((top_right->getVelocity() - m_velocity), (currentLength_top_right) / (glm::length(currentLength_top_right))))   *(currentLength_top_right) / (glm::length(currentLength_top_right));

	}
	if (bot_left != NULL) {
		currentLength_bot_left = bot_left->getCurrentPosition() - m_currentPosition;
		m_Springforce_bot_left = (ke * (glm::length(currentLength_bot_left) - .14f) + kd * glm::dot((bot_left->getVelocity() - m_velocity), (currentLength_bot_left) / (glm::length(currentLength_bot_left))))   *(currentLength_bot_left) / (glm::length(currentLength_bot_left));

	}
	if (bot_right != NULL) {
		currentLength_bot_right = bot_right->getCurrentPosition() - m_currentPosition;
		m_Springforce_bot_right = (ke * (glm::length(currentLength_bot_right) - .14f) + kd * glm::dot((bot_right->getVelocity() - m_velocity), (currentLength_bot_right) / (glm::length(currentLength_bot_right))))   *(currentLength_bot_right) / (glm::length(currentLength_bot_right));

	}
	m_spring_force1 = m_Springforce_bot + m_Springforce_top + m_Springforce_left + m_Springforce_right;
	m_spring_force2 = m_Springforce_bot_left + m_Springforce_bot_right + m_Springforce_top_left + m_Springforce_top_right;
	 

	float x = 2;
}

void Particle::addSpringForce3(int springforce, int damp)
{
	//m_previous = m_Springforce_top;
	//Particle p1 = p1;
	 
	 
	glm::vec3 currentLength_right;
	glm::vec3 currentLength_left;
	 


	glm::vec3 currentLength_new_;





	//glm::vec3 OriginaLength_top;
	float ke = 11;
	float kd = 10;
	 



	 




	if (right != NULL) {
		currentLength_right = right->getCurrentPosition() - m_currentPosition;
		m_Springforce_right = (ke * (glm::length(currentLength_right) - .1f) + kd * glm::dot((right->getVelocity() - m_velocity), (currentLength_right) / (glm::length(currentLength_right))))   *(currentLength_right) / (glm::length(currentLength_right));

	}




	if (left != NULL) {
		currentLength_left = left->getCurrentPosition() - m_currentPosition;
		m_Springforce_left = (ke * (glm::length(currentLength_left) - .1f) + kd * glm::dot((left->getVelocity() - m_velocity), (currentLength_left) / (glm::length(currentLength_left))))   *(currentLength_left) / (glm::length(currentLength_left));

	}

	 
	m_spring_force1 =   m_Springforce_left + m_Springforce_right      ;
	//m_spring_force2 = m_Springforce_bot_left + m_Springforce_bot_right + m_Springforce_top_left + m_Springforce_top_right;


	float x = 2;
}


void Particle::setVelocity(const float& x, const float& y, const float& z)
{
	glm::vec3 vel(x,y,z);
	m_velocity = vel;
}

void Particle::setVelocity(glm::vec3 vel)
{
	m_velocity = vel;
}
void Particle::addVelocity(glm::vec3 vel)
{
	m_velocity += vel;
}
void Particle::setBouncing(float bouncing)
{
	m_bouncing = bouncing;
}

void Particle::setMass(float mass)
{
	m_mass = mass;
}

void Particle::setLifetime(float lifetime)
{
	m_lifetime = lifetime;
}

void Particle::setFixed(bool fixed)
{
	m_fixed = fixed;
}

//getters
glm::vec3 Particle::getCurrentPosition()
{
	return m_currentPosition;
}

glm::vec3 Particle::getPreviousPosition()
{
	return m_previousPosition;
}


glm::vec3 Particle::getOriginPos()
{
	return originPos;
}

glm::vec3 Particle::getForce()
{
	return m_force;
}

glm::vec3 Particle::getVelocity()
{
	return m_velocity;
}

float Particle::getBouncing()
{
	return m_bouncing;
}

float Particle::getMass()
{
	return m_mass;
}

float Particle::getLifetime()
{
	return m_lifetime;
}

bool Particle::isFixed()
{
	return m_fixed;
}

void Particle::updateParticle(const float& dt, UpdateMethod method)
{
	glm::vec3 R= glm::vec3(0);;
	glm::vec3 L=glm::vec3(0);;
	glm::vec3 T= glm::vec3(0);;
	glm::vec3 B= glm::vec3(0);;
	if (right != NULL) R = this->right->getCurrentPosition();
	if (left != NULL) L =  this->left->getCurrentPosition();
	if (top != NULL)  T =  this->top->getCurrentPosition();
	if (bot != NULL)  B =  this->bot->getCurrentPosition();
	glm::vec3 cur = m_currentPosition;

	if (m_fixed) {
		model->SetPosition(m_currentPosition);
		model->render();
		return;
	}
		
	if (!m_fixed & m_lifetime > 0)
	{
		//updateSpringForce();
		switch (method)
		{
		case UpdateMethod::EulerOrig:
		{
			m_previousPosition = m_currentPosition;
			
			m_velocity +=  90.0f*(((R+L+B+T  )*0.25f)  - cur) * dt;
			 m_velocity *= .99f;
			m_currentPosition.y += m_velocity.y * dt;
			 
			
			model->SetPosition(m_currentPosition);
	
			prog.setUniform4f("color", m_currentPosition.y / 10, m_currentPosition.y / 20, 1.0f, 0.5f);
				}
		model->render();
		break;
		case UpdateMethod::EulerSemi:
		{
			m_previousPosition = m_currentPosition;

			m_velocity += m_force * dt;
			m_currentPosition += m_velocity * dt;
			//glm::mat4 prev = glm::mat4(1.0f);
		//	prev = glm::translate(prev, m_previousPosition);



			model->SetPosition(m_currentPosition);



			//prog.setUniformMatrix4f("model_mat", loc);
			prog.setUniform4f("color", m_currentPosition.y / 10, m_currentPosition.y / 20, 1.0f, 0.5f);
			//prog.setUniformMatrix4f("modelview", glm::translate( glm::mat4(1), glm::vec3(0,20,0)));


		}
		model->render();
		break;
		case UpdateMethod::Verlet:
		{
			 
			
			m_previousPosition = m_currentPosition - dt *m_velocity;
			glm::vec3 temp =  m_currentPosition + (m_currentPosition - m_previousPosition)*.990f + (m_force+ m_spring_force1 + m_spring_force2+ m_spring_force3) * ((dt*dt) / 1);

			m_velocity = (temp - m_currentPosition) / dt;
		
			prog.setUniform4f("color",0, 0,1.0f, 0.5f);
			
			m_currentPosition = temp;
			model->SetPosition(m_currentPosition);


		}
		model->render();
		break;
		


		}
		return;
	}
}

void Particle::updateSpringForce()
{
	m_force += (m_Springforce_top-m_previous);
	float x = 2;
}

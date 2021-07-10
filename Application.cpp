#include <GL/glew.h>
#include <GL/glut.h>
#include "Application.h"
#include <iostream>


void Application::init()
{
	bPlay = true;
	glClearColor(1.f, 1.f, 1.f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	scene.init();

	for (unsigned int i = 0; i < 256; i++)
	{
		keys[i] = false;
		specialKeys[i] = false;
	}
	mouseButtons[0] = false;
	mouseButtons[1] = false;
	//lastMousePos = glm::ivec2(-1, -1);
}

bool Application::loadMesh(const char *filename)
{
	return scene.loadMesh(filename);
}

bool Application::update(int deltaTime)
{
	scene.update(deltaTime);

	if (keys[119])//w
	{
		scene.getCamera().position += 0.516f *scene.getCamera().look;
	}
	if (keys[115])//s
	{
		scene.getCamera().position += -0.516f * scene.getCamera().look;
	}
	if (keys[97])//a
	{

		scene.getCamera().position += -0.516f * scene.getCamera().right;
	}
	if (keys[100])//d
	{

		scene.getCamera().position += 0.516f * scene.getCamera().right;
	}
	if (keys[101])//d
	{

		scene.getCamera().position += 0.6f * scene.getCamera().up;
	}


	if (keys[107])//k
	{

		scene.switchsplash();
	}
	if (keys[106])//j
	{

		scene.Addwave();
	}
	///////////////////////////////////////
	if (keys[99])//c
	{

		//scene.switchSpring();
		//cout << scene.springdamp << endl;

		//scene.getCamera().computeModelViewMatrix();
	}
	if (keys[118])//v
	{

		
		scene.addspringforce();
		//scene.getCamera().computeModelViewMatrix();
	}
	if (keys[98])//v
	{


		scene.addspringforce2();
		//scene.getCamera().computeModelViewMatrix();
	}

///////////////////////////////////////
	if (keys[122])//z
	{

		scene.springforce+=1;
		cout << scene.springforce<<endl;
	}
	if (keys[120])//x
	{

		scene.springforce -= 1;
		cout << scene.springforce << std::endl;

	}

	scene.getCamera().computeModelViewMatrix();

	return bPlay;
}

void Application::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Application::resize(int width, int height)
{
	glViewport(0, 0, width, height);
	scene.getCamera().resizeCameraViewport(width, height);
}

void Application::keyPressed(int key)
{
	if (key == 27) // Escape code
		bPlay = false;
	keys[key] = true;



}


void Application::keyReleased(int key)
{
	keys[key] = false;
}

void Application::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Application::specialKeyReleased(int key)
{
	specialKeys[key] = false;
	if (key == GLUT_KEY_F1)
		scene.switchPolygonMode();

	if (key == GLUT_KEY_F7)
		scene.switchParticle();
	//if (key == GLUT_KEY_F3)
		//scene.switchSpring();
	 
}

void Application::mouseMove(int x, int y)
{


	// Rotation
	scene.getCamera().rotateCamera((x - lastMousePos.x) / 155, (y - lastMousePos.y) / 155);
	lastMousePos = glm::vec2(x, y);
	//cout << x - lastMousePos.y << endl;

}

void Application::mousePress(int button)
{
	// mouseButtons[button] = true;
}

void Application::mouseRelease(int button)
{
	// mouseButtons[button] = false;
	 //if(!mouseButtons[0] && !mouseButtons[1])
	   //lastMousePos = glm::ivec2(-1, -1);
}

bool Application::getKey(int key) const
{
	return keys[key];
}

bool Application::getSpecialKey(int key) const
{
	return specialKeys[key];
}






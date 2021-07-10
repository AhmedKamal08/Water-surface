#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "Scene.h"
#include "PLYReader.h"
#include "GL/glew.h"
#include <random>
#include<time.h>
#include  "Mesh.h"
#define rand01() ((float)std::rand()/RAND_MAX)

Scene::Scene()
{
	mesh = NULL;
}

Scene::~Scene()
{
	if(mesh != NULL)
		delete mesh;
}
float Scene::RandomGenerator(int LO, int HI)
{
	float r3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	return r3;
}

void Scene::init()
{
	//std::random_device rd; // obtain a random number from hardware
	//std::mt19937 eng(rd()); // seed the generator
	//std::uniform_int_distribution<> distr(-4,4);
	//

	springforce = 10;
	springdamp = 10;
	particles = true;
	//spring = false;
	cloth = false;

	initShaders();
	mesh = new TriangleMesh();
	mesh->buildCube();
	mesh->sendToOpenGL(basicProgram);
	currentTime = 0.0f;
	camera.init(1.0f);
	bPolygonFill = true;
	
	loc = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0,  0)); 
    man = new  Mesh("models/cube.obj", basicProgram, loc);
	 
 
	
 
	///////////////////////////////////////////////
	float height = 15;
	 	parts = 20;
	int size = 5;
	 rows =31;
	 cols = 31;
	for (float i = 0; i < cols; i += 1) {
		vector<Particle> temp; {
			for (float j = 0; j < rows; j +=1) {
				Particle p = Particle( j/11  , 0,   i/11, basicProgram);
				temp.push_back(p);
			}
			spring.push_back(temp);
		}
	}
	//[2][2].setVelocity(0, 2, 0);
	
	spring[0][0].setVelocity(0, RandomGenerator(-height,height), 0);
	spring[0][cols-1].setVelocity(0, RandomGenerator(-height, height), 0);
	spring[rows-1][0].setVelocity(0, RandomGenerator(-height, height), 0);
	spring[rows-1][cols-1].setVelocity(0, RandomGenerator(-height, height), 0);

	float roughness = 0.60f;
	// s is the stepsize
	for (int s = parts / 2; s >= 1; s /= 2)
	{
		// iterate diamond step
		for (int y = s; y < parts; y += (s * 2))
		{
			for (int x = s; x < parts; x += (s * 2))
			{
				diamondStep(x, y, s, height);
			}
		}

		// iterate square step
		for (int y = 0; y < parts; y += (s * 2))
		{
			for (int x = 0; x < parts; x += (s * 2))
			{
				squareStep(x, y, s, height);
			}
		}

		// decrease the roughness
		height *= 0.55f;
	}




	CalcNeighbours2(spring);
	
	


	////////////////////////////////////////////////

 
	 

 
	////////////////////////////////////////
	s = new Sphere(glm::vec3(-5.5f, 6,0), 1, basicProgram);
	s2 = new Sphere(glm::vec3(2,4.5f , -2.5f), 1, basicProgram);

	t = new Triangle(  glm::vec3(5, 5, 1), glm::vec3(-5, 5, 1),glm::vec3(0, 5, 5), basicProgram);
	t2 = new Triangle(glm::vec3(-5, 5, -1), glm::vec3(5, 5, -1), glm::vec3(0, 5, -5), basicProgram);

	
	if(!text.init("fonts/OpenSans-Regular.ttf"))
	//if(!text.init("fonts/OpenSans-Bold.ttf"))
	//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;


	 
	punt =  glm::vec3(0.0f);
	normal= glm::vec3 (0, 1, 0);

	punt_top = glm::vec3(0.0f, 10, 0);
	normal_top = glm::vec3(0, -1, 0);
	
	punt_right = glm::vec3(5.0f,0,0);
	normal_right = glm::vec3(-1, 0, 0.0f);

	punt_left = glm::vec3(-5.0f, 0, 0);
	normal_left = glm::vec3(1, 0, 0);
	
	punt_back = glm::vec3(0.0f, 0, -5);
	normal_back = glm::vec3(0, 0, -1);

	punt_front = glm::vec3(0.0f, 0, 5);
	normal_front = glm::vec3(0, 0, 1);

	plane_floor = new Plane(punt, normal);
	plane_right = new Plane(punt_right, normal_right);
	plane_left = new Plane(punt_left, normal_left);
	plane_back = new Plane(punt_back, normal_back);
	plane_front = new Plane(punt_front, normal_front);
	plane_top = new Plane(punt_top, normal_top);

	Tri_plane = new Plane(glm::vec3(5, 5, 1), glm::vec3(-5, 5, 1), glm::vec3(0, 5, 5));
	Tri2_plane = new Plane(glm::vec3(-5, 5, -1), glm::vec3(5, 5, -1), glm::vec3(0, 5, -5));

	
	// simulation loop
	int count = 0;
	
	

	 
}

bool Scene::loadMesh(const char *filename)
{
	PLYReader reader;
	
	mesh->free();
	bool bSuccess = reader.readMesh(filename, *mesh);
	if(bSuccess)
	  mesh->sendToOpenGL(basicProgram);
	
	return bSuccess;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
}


void Scene::render()
{
	//glm::mat4 trans = glm::mat4(1.0f);
	// = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));
	glm::mat3 normalMatrix;
	basicProgram.use();
	basicProgram.setUniformMatrix4f("projection",camera.getProjectionMatrix());
	basicProgram.setUniformMatrix4f("modelview", camera.getModelViewMatrix());
	normalMatrix = glm::inverseTranspose(camera.getModelViewMatrix());
	basicProgram.setUniformMatrix3f("normalMatrix", normalMatrix);

	basicProgram.setUniform1i("bLighting", bPolygonFill ? 1 : 0);
	
	if (bPolygonFill)
	{
		basicProgram.setUniform4f("color", 0.9f, 0.9f, 0.95f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		basicProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.5f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		mesh->render();
		glTranslatef(4.0f, 0, 0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_POLYGON_OFFSET_FILL);
		basicProgram.setUniform4f("color", 0.0f, 0.0f, 0.0f, 1.0f);

	}
	
	basicProgram.setUniform4f("color", 0.02f, 1, 0.2f, 0.5f);
	 
	//man->render();
	 
	//t->render_triangle();
	//t2->render_triangle();

	//s->render_ball();
	//s2->render_ball();

	int count = 0;


	disant = disact;
	disant2 = disact2;
	disant3 = disact3;
	disant4 = disact4;
	disant5 = disact5;
	disant6 = disact6;
	disantTri = disactTri;
	disantTri2 = disactTri2;

	float time = tini;
			

	float Spread = 1.0f;
	for (int i = 0; i < stones.size(); i++)
	{
		stones[i].updateParticle(dt, Particle::UpdateMethod::Verlet);
	
	}
	for (int i = 0; i < stones.size(); i++)
	{
		glm::vec3 pos = stones[i].getCurrentPosition();
		if (pos.y < 0.01f&&pos.y>-0.01f) {
			MakeAsplash((int)pos.x*5,(int)pos.z*5);
		}
	}
	if (splash) {

		dropStone();

		splash = false;

	}
	for (int i = 0; i < spring.size(); i++) {
		for (int j = 0; j < spring[i].size(); j++) {
			 

			//spring[i][j].addSpringForce3(springforce, springdamp);
    		spring[i][j].updateParticle(dt, Particle::UpdateMethod::EulerOrig);
           
			


		}
	
	}
	if (wave) {

		Addwave();

		wave = false;

	}
	

	 
}
void Scene::switchsplash()
{
	
	splash =true;
}
void Scene::dropStone()
{
	if (splash) {
		int x = RandomGenerator(2,rows-6);
		int z = RandomGenerator(2, cols-6);
		Particle stone = Particle(x / 5, 5, z / 5, basicProgram);
		stone.addForce(0, -6, 0);
		stone.right = NULL;
		stone.left = NULL;
		stone.top = NULL;
		stone.bot = NULL;
		stones.push_back(stone);
		splash = false;
	}
}

Camera &Scene::getCamera()
{
  return camera;
}

void Scene::switchPolygonMode()
{
  bPolygonFill = !bPolygonFill;
}
void Scene::MakeAsplash(int x ,int z)
{
	
	
		int power = RandomGenerator(1, 2);

		
		
		
		glm::vec3 f = spring[x][z].getVelocity();
		glm::vec3 S = glm::vec3(f.x, f.y - power, f.z);
		spring[x][z].addVelocity(S);
		if (spring[x][z].right != NULL) {
			spring[x][z].right->addVelocity(glm::vec3(f.x, f.y -power * 0.4f, f.z));
		}
		if (spring[x][z].left != NULL) {
			spring[x][z].right->addVelocity(glm::vec3(f.x, f.y - power * 0.4f, f.z));

		}
		if (spring[x][z].top != NULL) {
			spring[x][z].right->addVelocity(glm::vec3(f.x, f.y - power * 0.4f, f.z));

		}
		if (spring[x][z].bot != NULL) {
			spring[x][z].right->addVelocity(glm::vec3(f.x, f.y - power * 0.4f, f.z));

		}
		//////////
		if (spring[x][z].top_left != NULL) {
			spring[x][z].top_left->addVelocity(glm::vec3(0, f.y - power*0.2f,0));
		}
		if (spring[x][z].top_right != NULL) {
			spring[x][z].top_right->addVelocity(glm::vec3(0, f.y - power * 0.2f ,0));

		}
		 
	 
		if (spring[x][z].bot_right != NULL) {
			spring[x][z].bot_right->addVelocity(glm::vec3(0, f.y - power * 0.2f, 0));

		}
		if (spring[x][z].bot_left != NULL) {
			spring[x][z].bot_left->addVelocity(glm::vec3(f.x, f.y - power * 0.2f, f.z));

		}
		
		;
	
}

void Scene::switchParticle()
{  // spring = false;
	cloth = false;
	particles = true;
	int k = 0;
}

void Scene::Addwave()
{
	for (int i = 0; i < spring.size(); i++) {
		for (int j = 0; j < spring[i].size(); j++) {
			
			spring[j][0].addVelocity(glm::vec3(0,-.02f,0));
		}
	}

}

void Scene::addspringforce()
{
	for (int i = 0; i < spring.size(); i++) {
		for (int j = 0; j < spring[i].size(); j++) {
			spring[spring.size() - 1][j].setVelocity(0, 3, 0);

		}

	}
}
void Scene::addspringforce2()
{
	for (int i = 0; i < spring.size(); i++) {
		for (int j = 0; j < spring[i].size(); j++) {
			spring[spring.size() - 1][j].setVelocity(1,0, 0);

		}

	}
}
void Scene::addvelo()
{

	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid[i].size(); j++) {
			grid[grid.size()-1][j].setVelocity(2, 0, 0);
		
		}
	
	}
	
}

void Scene::addvelo2()
{

	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid[i].size(); j++) {
			grid[grid.size() - 1][j].setVelocity(-2, 0, 0);

		}

	}

}
void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/basic.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/basic.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	basicProgram.init();
	basicProgram.addShader(vShader);
	basicProgram.addShader(fShader);
	basicProgram.link();
	if(!basicProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << basicProgram.log() << endl << endl;
	}
	basicProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::CalcNeighbours(vector<vector<Particle>> &grid)
{
	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid[i].size(); j++) {
			if (j >= 0 && j< grid[i].size() -1) { ////////// bot
				grid[i][j].bot = &grid[i ][j+1];
			 }
			else {
				grid[i][j].bot = NULL; //////////// no bot 
			}

			if (j >= 0 && j < grid[i].size() - 2) { //////////bot_bot
				grid[i][j].bot_bot = &grid[i][j + 2];
			}
			else {
				grid[i][j].bot_bot = NULL; //////////// no bot 
			}


			if (j > 0 &&j < grid[i].size() ) { /////////// top 
				grid[i][j].top = &grid[i ][j-1];

			}
			else {////////// no top 
				grid[i][j].top = NULL;

			}
			if (j > 1 && j < grid[i].size()) { /////////// top top
				grid[i][j].top_top = &grid[i][j - 2];

			}
			else {////////// no top top
				grid[i][j].top_top = NULL;

			}
			
			if (i >= 0 && i < grid.size() - 1) {////////////right 
				grid[i][j].right = &grid[i+1][j];
			}
			else {//////no right
				grid[i][j].right = NULL;

			}
			if (i >= 0 && i < grid.size() - 2) {////////////right right
				grid[i][j].right_right = &grid[i +2][j];
			}
			else {//////no right
				grid[i][j].right_right = NULL;

			}
			if (i > 0 && i < grid.size() ) {/////////////left
				grid[i][j].left = &grid[i - 1][j];
			}
			else {
				grid[i][j].left = NULL;

			}
			if (i > 1 && i < grid.size()) {/////////////leftleft
				grid[i][j].left_left = &grid[i - 2][j];
			}
			else {
				grid[i][j].left_left = NULL;

			}
///////////////////////////////////////////////////////////
			if ((j >= 0 && j < grid[i].size() - 1)&& (i >= 0 && i < grid.size() - 1)) {
				 
				
					grid[i][j].bot_right= &grid[i + 1][j+1];
			}
			else {
					grid[i][j].bot_right = NULL;

				}
			
			if ((j >= 0 && j < grid[i].size() - 1)&& (i > 0 && i < grid.size())) {
				///////////bot_left
					grid[i][j].bot_left = &grid[i - 1][j+1];
				}
			
			else {
					grid[i][j].bot_left = NULL; //////////// no bot_left 
				}
			
			
			if ((j > 0 && j < grid[i].size()) && (i >= 0 && i < grid.size() - 1)) 
			{ ///////// top_right

				grid[i][j].top_right = &grid[i + 1][j - 1];
			}
			else {
					grid[i][j].top_right = NULL; //////////// no top_right 
			}
			


			if( (j > 0 && j < grid[i].size())&& (i > 0 && i < grid.size() )) { ///////// top_left

				grid[i][j].top_left = &grid[i - 1][j - 1];
				}
			else {
					grid[i][j].top_left = NULL; //////////// no top_left 
				}
			


		}
	}

}
void Scene::CalcNeighbours2(vector<vector<Particle>> &grid)
{
	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid[i].size(); j++) {
			if (j >= 0 && j < grid[i].size() - 1) { ////////// bot
				grid[i][j].bot = &grid[i][j + 1];
			}
			else {
				grid[i][j].bot = NULL; //////////// no bot 
			}

			 

			if (j > 0 && j < grid[i].size()) { /////////// top 
				grid[i][j].top = &grid[i][j - 1];

			}
			else {////////// no top 
				grid[i][j].top = NULL;

			}
			 

			if (i >= 0 && i < grid.size() - 1) {////////////right 
				grid[i][j].right = &grid[i + 1][j];
			}
			else {//////no right
				grid[i][j].right = NULL;

			}
			 
			if (i > 0 && i < grid.size()) {/////////////left
				grid[i][j].left = &grid[i - 1][j];
			}
			else {
				grid[i][j].left = NULL;

			}
			 
			///////////////////////////////////////////////////////////
			if ((j >= 0 && j < grid[i].size() - 1) && (i >= 0 && i < grid.size() - 1)) {


				grid[i][j].bot_right = &grid[i + 1][j + 1];
			}
			else {
				grid[i][j].bot_right = NULL;

			}

			if ((j >= 0 && j < grid[i].size() - 1) && (i > 0 && i < grid.size())) {
				///////////bot_left
				grid[i][j].bot_left = &grid[i - 1][j + 1];
			}

			else {
				grid[i][j].bot_left = NULL; //////////// no bot_left 
			}


			if ((j > 0 && j < grid[i].size()) && (i >= 0 && i < grid.size() - 1))
			{ ///////// top_right

				grid[i][j].top_right = &grid[i + 1][j - 1];
			}
			else {
				grid[i][j].top_right = NULL; //////////// no top_right 
			}



			if ((j > 0 && j < grid[i].size()) && (i > 0 && i < grid.size())) { ///////// top_left

				grid[i][j].top_left = &grid[i - 1][j - 1];
			}
			else {
				grid[i][j].top_left = NULL; //////////// no top_left 
			}



		}
	}

}

void Scene::diamondStep(int x, int y, int s,float roughness) {


	 
	 	// take the four values around (x,y) and calculate the average.
		float avg = (spring[x - s][ y - s].getVelocity().y +
			spring[x + s][ y - s].getVelocity().y
		  + spring[x - s][ y + s].getVelocity().y
		  + spring[x + s][ y + s].getVelocity().y) * .25f;

		// add a random value to the average in dependence of the roughness.
		avg += (RandomGenerator(-1, 1)* roughness);

		// don't let the average get to small or to big.
		

		// set the value for that point.
		spring[x][y ].setVelocity(0,avg,0);
	 


}

void Scene::squareStep(int x, int y, int s, float roughness) {

	// left(a1)/top(b1) corner
   // starting point can't be out of bounds
	float corner_1 = spring[x][y].getVelocity().y;
	float corner_a2;
	float corner_a3;
	float corner_a4;
	float corner_b2;
	float corner_b3;
	float corner_b4;

	// bottom(a2)/right(b2) corner
	corner_a2 = spring[x + s][y + s].getVelocity().y;
	corner_b2 = spring[x + s][y + s].getVelocity().y;

	// right(a3)/bottom(b3) corner
	
	corner_a3 = spring[x + (2 * s)][y].getVelocity().y;
	corner_b3 = spring[x][y + (2 * s)].getVelocity().y;

	// top(a4)/left(b4) corner
	if (y == 0)
	{
		
		corner_a4 = spring[x + s][parts - s].getVelocity().y;
	}
	else
	{
	
		corner_a4 = spring[x + s][y - s].getVelocity().y;
	}

	if (x == 0)
	{
		
		corner_b4 = spring[parts - s][y + s].getVelocity().y;;
	}
	else
	{
		
		corner_b4 = spring[x - s][y + s].getVelocity().y;
	}


	// fill new averaged positions
	float avg_a = (corner_1 + corner_a2 + corner_a3 + corner_a4) / 4.0f;
	avg_a = avg_a + (RandomGenerator(-1, 1)* roughness);

	if (avg_a > 1.0)
	{
		avg_a = 1.0f;
	}
	else if (avg_a < 0.0)
	{
		avg_a = 0.0f;
	}
	spring[x +s][y ].setVelocity(0, avg_a,0);

	//seamless mapping
	if (y == 0) {
		spring[x + s][parts].setVelocity(0, avg_a, 0);
	}




	float avg_b = (corner_1 + corner_b2 + corner_b3 + corner_b4) / 4.0f;
	avg_b = avg_b + (RandomGenerator(-1,1) * roughness);

	if (avg_b > 1.0f)
	{
		avg_b = 1.0f;
	}
	else if (avg_b < 0.0f)
	{
		avg_b = 0.0f;
	}
	spring[x  ][y + s].setVelocity(0, avg_a, 0);
	 
	//seamless mapping
	if (x == 0) {
		spring[parts][y + s].setVelocity(0, avg_a, 0);
		 
	}




}

void Scene::CalcNeighbours3(vector<vector<vector<Particle>>> &grid)
{

	 
	for (int i = 0; i < grid.size(); i++) {
	
		
		for (int j = 0; j < grid[i].size(); j++) {

			for (size_t k = 0; k < grid[i][j].size(); k++) {
				if (j >= 0 && j < grid[i].size() - 1) { ////////// bot
					grid[i][j][k].bot = &grid[i][j + 1][k];
				}
				else {
					grid[i][j][k].bot = NULL; //////////// no bot 
				}



				if (j > 0 && j < grid[i].size()) { /////////// top 
					grid[i][j][k].top = &grid[i][j - 1][k];

				}
				else {////////// no top 
					grid[i][j][k].top = NULL;

				}


				if (i >= 0 && i < grid.size() - 1) {////////////right 
					grid[i][j][k].right = &grid[i + 1][j][k];
				}
				else {//////no right
					grid[i][j][k].right = NULL;

				}

				if (i > 0 && i < grid.size()) {/////////////left
					grid[i][j][k].left = &grid[i - 1][j][k];
				}
				else {
					grid[i][j][k].left = NULL;

				}
				if (k >= 0 && k < grid[i][j].size() - 1) { ////////// front
					grid[i][j][k].front = &grid[i][j][k+1];
				}
				else {
					grid[i][j][k].front = NULL; //////////// no bot 
				}

				if (k> 0 && k < grid[i][j].size()) { /////////// top 
					grid[i][j][k].back = &grid[i][j ][k-1];

				}
				else {////////// no top 
					grid[i][j][k].back = NULL;

				}
				///////////////////////////////////////////////////////////
				if ((k >= 0 && k < grid[i][j].size() - 1) && (i >= 0 && i < grid.size() - 1)) {


					grid[i][j][k].bot_right = &grid[i+1 ][j][k+1];
				}
				else {
					grid[i][j][k].bot_right = NULL;

				}



			}
		}
	}

}
#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE
#include <random>


#include <glm/glm.hpp>
#include "Camera.h"
#include "ShaderProgram.h"
#include "TriangleMesh.h"
#include "Text.h"
#include "Mesh.h"
#include "D:/3rd semester/CA/water/Particle.h"
#include "D:/3rd semester/CA/water/Geometry.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();
	float RandomGenerator(int range1, int range2);
	void addspringforce();
	void addspringforce2();
	void addvelo();
	void addvelo2();
	void init();
	bool loadMesh(const char *filename);
	void update(int deltaTime);
	void render();
	void dropStone();
	void switchsplash();
	 
	int springforce;
	int springdamp;
  Camera &getCamera();
public: int n; 
  void switchPolygonMode();

  void MakeAsplash(int x , int z);
  bool wave = false;
  bool splash = false;
  void switchParticle();
  void Addwave();
  void switchCloth();
  
private:
	void initShaders();
	void computeModelViewMatrix();

private:
  Camera camera;
	TriangleMesh *mesh;
	ShaderProgram basicProgram;
	float currentTime;
	bool bPolygonFill;

	bool particles;
	//bool spring;
	bool cloth;
	Mesh *man ;
    Text text;
	glm::mat4 loc;

	int rows ; 
	int cols;
	float dt = 0.01f;  //simulation time
	float tini = 0.0f;
	float tfinal = 8.0f; //final time of simulation 
	// particle inicialization
	Particle *p;
	std::vector<Particle> _particles;
	Particle *p1;
	Particle *p2;
	Particle *p3;


	void CalcNeighbours(vector <vector<Particle>> &grid);
	void CalcNeighbours2(vector<vector<Particle>>& grid);
	 
	void diamondStep(int x, int y, int s,float r);
	void squareStep(int x, int y, int s, float roughness);
	void CalcNeighbours3(vector<vector<vector<Particle>>>& grid);
	vector <vector<Particle>> grid;
	vector<Particle> stones;
	vector <vector<Particle>> spring;
	vector <vector<vector<Particle>>>jelly;

	int parts;;
	glm::vec3 punt ;
	glm::vec3 normal ;
	
	glm::vec3 punt_right;
	glm::vec3 normal_right;
	
	glm::vec3 punt_left;
	glm::vec3 normal_left;

	glm::vec3 punt_back;
	glm::vec3 normal_back;
	
	glm::vec3 punt_front;
	glm::vec3 normal_front;

	glm::vec3 punt_top;
	glm::vec3 normal_top;
	vector<float> leftDeltas;
	vector<float> rightDeltas;;
	 
	Plane *plane_floor;
	Plane *plane_right;
	Plane *plane_left;
	Plane *plane_back ;
	Plane *plane_front;
	Plane *plane_top;
	Plane*Tri_plane;
	Plane*Tri2_plane;


	glm::vec3 newPos;
	glm::vec3 newCorrectPos;


	glm::vec3 newVelo;
	glm::vec3 newCorrectVelo;
	Sphere *s2;

	Sphere *s;
	Triangle *t;
	Triangle *t2;

	bool isInside;
	// simulation loop
	int count = 0;
	float disact, disant;
	 
	float disact2, disant2;
	float disact3, disant3;
	float disact4, disant4;
	float disact5, disant5;
	float disact6, disant6;
	float disactTri, disantTri;
	float disactTri2, disantTri2;



};


#endif // _SCENE_INCLUDE


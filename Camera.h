#ifndef _CAMERA_INCLUDE
#define _CAMERA_INCLUDE


#include <glm/glm.hpp>


// Camera contains the properies of the camera the scene is using
// It is responsible for computing the associated GL matrices


class Camera
{

public:
	Camera();
	~Camera();


	//view
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 look;
	glm::vec3 right;
	glm::vec2 lastMousePos;
	glm::vec3 direction_;

	void init(float initDistance, float initAngleX = 0.0f, float initAngleY = 0.0f);

	void resizeCameraViewport(int width, int height);
	void yaw(float angle);
	void pitch(float angle);
	void rotateCamera(float xRotation, float yRotation);

	void moveCamera(float front, float left);
	float sensitvity;
	glm::mat4 &getProjectionMatrix();
	glm::mat4 &getModelViewMatrix();

	 
	
	//void updateCameraVectors();

	void computeModelViewMatrix();

	void KeyPressed(int key);


private:
	bool first_mouse_call_;
	/*
		The mouse is made to be invisible, and after each movement it's warped to the center of the screen again
		so that the movement is continous
	*/
	bool warp_call_;
	
	float last_cursor_position_x_;
	float last_cursor_position_y_;

	int width_;
	int height_;
	
	float	yaw_;
	float pitch_;
	glm::vec3 eyevector;
	float angleX, angleY, distance;   // Camera parameters
	float rangeDistanceCamera[2];
	glm::mat4 projection, modelview, modelview2;  // OpenGL matrices

};


#endif // _CAMERA_INCLUDE


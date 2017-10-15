#pragma once
#include "btBulletDynamicsCommon.h"
#include "ModelPhysicsCoordinator.h"
#include "EmptyModel.h"
#include <learnopengl/camera.h>
#include <GLFW/glfw3.h>
#include <string>



class Player {

public:
	static std::string written;

	EmptyModel e;
	ModelPhysicsCoordinator mpc = ModelPhysicsCoordinator(&e, CollisionType::capsule, 1);
	Camera cam;
	bool isWriting = false;
	Player() { 
	mpc.btModel->setAngularFactor(0);
	//mpc.btModel->setLinearFactor(btVector3(1,0,1));
	//mpc.btModel->setGravity(btVector3(0,0,0));
	//mpc.btModel->setRestitution(0);
	}
	float movementSpeed = 1000;
	double lastJump = 0.0;
	double lastEnter = 0.0;

	void processInput(GLFWwindow *window, float time);

};
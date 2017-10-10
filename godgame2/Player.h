#pragma once
#include "btBulletDynamicsCommon.h"
#include "ModelPhysicsCoordinator.h"
#include "EmptyModel.h"
#include <learnopengl/camera.h>
#include <GLFW/glfw3.h>

class Player {

public:
	EmptyModel e;
	ModelPhysicsCoordinator mpc = ModelPhysicsCoordinator(&e, CollisionType::capsule, 1);
	Camera cam;
	Player() { 
	mpc.btModel->setAngularFactor(0);
	//mpc.btModel->setLinearFactor(btVector3(1,0,1));
	//mpc.btModel->setGravity(btVector3(0,0,0));
	//mpc.btModel->setRestitution(0);
	}
	float movementSpeed = 1000;
	float lastJump = 0;

	void processInput(GLFWwindow *window, float time);

};
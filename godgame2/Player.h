#pragma once
#include "btBulletDynamicsCommon.h"
#include "ModelPhysicsCoordinator.h"
#include "EmptyModel.h"
#include <learnopengl/camera.h>
#include <GLFW/glfw3.h>

class Player {

public:
	EmptyModel e;
	ModelPhysicsCoordinator mpc = NULL;
	Camera cam;
	Player() { mpc = ModelPhysicsCoordinator(&e, CollisionType::capsule, 0); }

	void processInput(GLFWwindow *window);

};
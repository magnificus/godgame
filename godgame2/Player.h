#pragma once
#include "btBulletDynamicsCommon.h"
#include "ModelPhysicsCoordinator.h"
#include "EmptyModel.h"
#include <learnopengl/camera.h>
#include <GLFW/glfw3.h>
#include <string>
#include "ModelHandler.h"
#include "PhysicsHandler.h"
#include "KeyStruct.h"

class Player {

public:
	std::string written;
	bool isWriting;
	//bool isCarrying = false;
	btRigidBody *carrying = nullptr;
	int prevFlags;

	EmptyModel e;
	ModelPhysicsCoordinator mpc = ModelPhysicsCoordinator(&e, CollisionType::capsule, 0.1);
	Camera cam;
	Player() { 
	mpc.btModel->setAngularFactor(0);
	mpc.btModel->setFriction(0);
	//mpc.btModel->setLinearFactor(btVector3(1,0,1));
	//mpc.btModel->setRestitution(0);
	}
	float movementSpeed = 25.0f;
	double lastJump = 0.0;
	double lastEnter = 0.0;

	// returns true if world is changed
	bool processInput(GLFWwindow * window, std::vector<unsigned int>& char_callbacks, std::vector<KeyStruct>& key_callbacks, bool &drawShadows, float time, ModelHandler & modelHandler, PhysicsHandler & physicsHandler, Shader * s);
	bool swapWriting() {};


};
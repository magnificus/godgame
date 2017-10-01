#pragma once
#include <vector>
#include "Model.h"
#include "btBulletDynamicsCommon.h"

class PhysicsHandler {

public:
	PhysicsHandler();
	~PhysicsHandler();
	std::vector<Model*> models;

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
};
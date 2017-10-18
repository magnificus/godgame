#pragma once
#include <vector>
#include "Model.h"
#include "btBulletDynamicsCommon.h"
#include "ModelPhysicsCoordinator.h"

class PhysicsHandler {

public:
	PhysicsHandler();
	~PhysicsHandler();
	std::vector<ModelPhysicsCoordinator> models;

	void simulationTick(float time);
	void addModel(Model *m);
	void addMPC(ModelPhysicsCoordinator mpc);


	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
};
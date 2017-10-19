#pragma once
#include <vector>
#include "Model.h"
#include "btBulletDynamicsCommon.h"
#include "ModelPhysicsCoordinator.h"
#include <map>
#include <set>
class PhysicsHandler {

public:
	PhysicsHandler();
	~PhysicsHandler();
	std::set<ModelPhysicsCoordinator> models;
	std::map<btRigidBody*, ModelPhysicsCoordinator*>  btModelMap;
	void simulationTick(float time);
	void addMPC(ModelPhysicsCoordinator mpc);


	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
};
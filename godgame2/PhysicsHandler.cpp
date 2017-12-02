#include "PhysicsHandler.h"


PhysicsHandler::PhysicsHandler() {
	broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	solver = new btSequentialImpulseConstraintSolver;

	// The world.
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));


}

void PhysicsHandler::clearModels() {
	//for (ModelPhysicsCoordinator mpc : models) {
	//	delete(mpc.btModel);
	//}
	models.clear();

	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;
	broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	solver = new btSequentialImpulseConstraintSolver;

	// The world.
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
}

PhysicsHandler::~PhysicsHandler() {
	clearModels();

}

void PhysicsHandler::simulationTick(float time)
{
	dynamicsWorld->stepSimulation(time);

	// update the real models with new information
	for (ModelPhysicsCoordinator m : models) {
		m.updateModel();
	}
}

void PhysicsHandler::addMPC(ModelPhysicsCoordinator mpc) {
	models.insert(mpc);
	dynamicsWorld->addRigidBody(mpc.btModel);
	btModelMap[mpc.btModel] =  mpc;
	//mpc.btModel->setRestitution(1);

}
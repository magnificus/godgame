#pragma once
#include "Model.h"
#include "btBulletDynamicsCommon.h"

enum CollisionType {custom, cube, sphere, plane, capsule};

class ModelPhysicsCoordinator {

public:
	//ModelPhysicsCoordinator(Model *m) : model(m) { 
	//	btCollisionShape* shape = new btSphereShape(1);;

	//	btDefaultMotionState* motionState =
	//		new btDefaultMotionState(btTransform(btQuaternion(0, 1, 0, 1), btVector3(m->transform[3][0], m->transform[3][1], m->transform[3][2])));
	//	btScalar mass = 1;
	//	btVector3 fallInertia(0, 0, 0);
	//	shape->calculateLocalInertia(mass, fallInertia);
	//	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motionState, shape, fallInertia);
	//	btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
	//	btModel = fallRigidBody;

	//}
	ModelPhysicsCoordinator(Model *m, CollisionType type, float mass, glm::vec3 lookDir = glm::vec3(0,0,0)) : model(m) {

		btCollisionShape* shape = nullptr;

		btQuaternion quat;
		quat.setEuler(lookDir[0], lookDir[1], lookDir[2]);
		btDefaultMotionState* motionState =
			new btDefaultMotionState(btTransform(quat, btVector3(m->transform[3][0], m->transform[3][1], m->transform[3][2])));
		switch (type) {
		case custom: shape = new btBoxShape(btVector3(0.5, 0.5, 0.5)); break;
		case cube: shape = new btBoxShape(btVector3(0.5,0.5,0.5)); break;
		case sphere: shape = new btSphereShape(1); break;
		case plane: shape = new btStaticPlaneShape(btVector3(0, 1, 0), 0); break;
		case capsule: shape = new btCapsuleShape(1, 1.8);
		}
		btVector3 fallInertia(0, 0, 0);
		shape->calculateLocalInertia(mass, fallInertia);
		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motionState, shape, fallInertia);
		btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
		btModel = fallRigidBody;

	}

	ModelPhysicsCoordinator(Model *m, btRigidBody *body) : model(m), btModel(body) {};
	void updateModel();

	Model *model;
	btRigidBody *btModel;
};
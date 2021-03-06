#pragma once
#include "Model.h"
#include "btBulletDynamicsCommon.h"
#include <glm\gtc\type_ptr.hpp>

enum CollisionType {custom, cube, sphere, plane, capsule};

class ModelPhysicsCoordinator {

public:
	ModelPhysicsCoordinator() {}
	~ModelPhysicsCoordinator() {}

	ModelPhysicsCoordinator(Model *m, CollisionType type, float mass, glm::vec3 lookDir = glm::vec3(1,0,0), btCollisionShape* shape = nullptr, float scale = 1.0f) : model(m) {

		btQuaternion quat;
		quat.setEuler(lookDir[0], lookDir[1], lookDir[2]);
		btDefaultMotionState* motionState =
			new btDefaultMotionState(btTransform(quat, btVector3(m->transform[3][0], m->transform[3][1], m->transform[3][2])));


		if (!shape) {
			switch (type) {
			case custom: shape = new btConvexHullShape(glm::value_ptr(m->vertices[0]), int(m->vertices.size()), sizeof(glm::vec3)); break; // this is a simple method, use the one in CustomShapeBuilder for the cool version
			case cube: shape = new btBoxShape(btVector3(0.5, 0.5, 0.5)*scale); break;
			case sphere: shape = new btSphereShape(1* scale); break;
			case plane: shape = new btStaticPlaneShape(btVector3(0, 1, 0)*scale, 0); break;
			case capsule: shape = new btCapsuleShape(1.0*scale, 1.8*scale); break;
			}
		}
		shape->setLocalScaling(btVector3(m->transform[0][0], m->transform[1][1], m->transform[2][2]));
		btVector3 fallInertia(0, 0, 0);
		shape->calculateLocalInertia(mass, fallInertia);
		btRigidBody::btRigidBodyConstructionInfo bodyCI(mass, motionState, shape, fallInertia);
		btRigidBody* body = new btRigidBody(bodyCI);
		//shape->setUserPointer(body);
		btModel = body;

	}

	bool operator==(ModelPhysicsCoordinator rhs) const {
		return rhs.btModel == btModel;
	}
	bool operator==(ModelPhysicsCoordinator *rhs) const {
		return rhs->btModel == btModel;
	}
	bool operator!=(ModelPhysicsCoordinator rhs) const {
		return rhs.btModel != btModel;
	}
	bool operator<(ModelPhysicsCoordinator rhs) const {
		return rhs.btModel < btModel;
	}
	bool operator>(ModelPhysicsCoordinator rhs) const {
		return rhs.btModel > btModel;
	}
	bool operator>=(ModelPhysicsCoordinator rhs) const {
		return rhs.btModel >= btModel;
	}
	bool operator<=(ModelPhysicsCoordinator rhs) const {
		return rhs.btModel <= btModel;
	}


	//ModelPhysicsCoordinator(Model *m, btRigidBody *body) : model(m), btModel(body) {};
	void updateModel();

	void setQuaternion(float w, float x, float y, float z);

	Model *model;
	btRigidBody *btModel;
};
#pragma once
#include "Model.h"
#include "btBulletDynamicsCommon.h"
#include <glm\gtc\type_ptr.hpp>

enum CollisionType {custom, cube, sphere, plane, capsule};

class ModelPhysicsCoordinator {

public:
	ModelPhysicsCoordinator() {}


	ModelPhysicsCoordinator(Model *m, CollisionType type, float mass, glm::vec3 lookDir = glm::vec3(1,0,0)) : model(m) {

		btCollisionShape* shape = nullptr;

		btQuaternion quat;
		quat.setEuler(lookDir[0], lookDir[1], lookDir[2]);
		btDefaultMotionState* motionState =
			new btDefaultMotionState(btTransform(quat, btVector3(m->transform[3][0], m->transform[3][1], m->transform[3][2])));



		//btVector3 *positions = new btVector3[m->vertices.size()];
		//float *radii = new float[m->vertices.size()*3];
		//for (int i = 0; i < m->vertices.size(); i++) {
		//	positions[i] = btVector3(m->vertices[i].x, m->vertices[i].y, m->vertices[i].z);
		//}
		//for (int i = 0; i < m->vertices.size() * 3; i++) {
		//	radii[i] = 0.01f;
		//}
		//std::cout << m->vertices.size() << std::endl;
		//btShapeHull* hull = new btShapeHull(originalConvexShape);
		//btScalar margin = originalConvexShape->getMargin();
		//hull->buildHull(margin);
		//btConvexHullShape* simplifiedConvexShape = new btConvexHullShape(hull->getVertexPointer(), hull->numVertices());


		switch (type) {
		case custom: {
			btCompoundShape *shape2 = new btCompoundShape();
			btCollisionShape *stdSphere = new btSphereShape(0.1f);
			int count = 0;
			for (glm::vec3 v : m->vertices) {

				btTransform trans;
				trans.setIdentity();
				btVector3 origin = btVector3(v.x, v.y, v.z);
				trans.setOrigin(origin);
				//trans.
				//trans->setsc
				//if (count++ % 10 == 0)
				shape2->addChildShape(trans, stdSphere);
			}
			shape = shape2;
			break;
		}
		//case custom: shape = new btMultiSphereShape(positions, radii, int(m->vertices.size())); break;
		//case custom: shape = new btConvexHullShape(*positions, int(m->vertices.size())); break;

		//case custom: shape = new btConvexHullShape(glm::value_ptr(m->vertices[0]), int(m->vertices.size()), sizeof(glm::vec3)); break;
		case cube: shape = new btBoxShape(btVector3(0.5,0.5,0.5)); break;
		case sphere: shape = new btSphereShape(1); break;
		case plane: shape = new btStaticPlaneShape(btVector3(0, 1, 0), 0); break;
		case capsule: shape = new btCapsuleShape(1.0, 1.8);
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

	Model *model;
	btRigidBody *btModel;
};
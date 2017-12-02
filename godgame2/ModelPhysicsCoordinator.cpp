#include "ModelPhysicsCoordinator.h"
#include <glm\gtc\type_ptr.hpp>

void ModelPhysicsCoordinator::updateModel()
{
	btTransform t;
	// Get the transform from Bullet and into 't'
	btModel->getMotionState()->getWorldTransform(t);
	// Convert the btTransform into the GLM matrix using 'glm::value_ptr'
	t.getOpenGLMatrix(glm::value_ptr(model->transform));

}

void ModelPhysicsCoordinator::setQuaternion(float w, float x, float y, float z)
{
	btTransform trans;
	btModel->getMotionState()->getWorldTransform(trans);
	btQuaternion rot = trans.getRotation();
	rot.setW(w);
	rot.setX(x);
	rot.setY(y);
	rot.setZ(z);
	trans.setRotation(rot);
	btModel->getMotionState()->setWorldTransform(trans);
}

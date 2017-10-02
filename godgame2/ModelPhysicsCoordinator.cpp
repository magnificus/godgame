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

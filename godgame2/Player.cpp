#include "Player.h"
#include "BaseLibrary.h"
#include "tinyexpr.h"
#include "CustomShape.h"
#include <set>
#include <list>
#include <algorithm>
bool Player::processInput(GLFWwindow *window, std::vector<unsigned int> &char_callbacks, std::vector<KeyStruct> &key_callbacks, bool &drawShadows, std::list<TextStruct> &texts, float time, ModelHandler &modelHandler, PhysicsHandler &physicsHandler, Shader *s)
{

	float moveTowardsSpeed = 3.0f;
	if (carrying) {
		glm::vec3 offset = cross(cam.Up, cam.Right) * 3.0f;
		btVector3 newLoc = mpc.btModel->getWorldTransform().getOrigin() + btVector3(offset.x, offset.y, offset.z);
		btVector3 oldLoc = carrying->getWorldTransform().getOrigin();
		btVector3 towards = newLoc - oldLoc;
		carrying->setLinearVelocity(towards*5.0f* carrying->getInvMass());
		carrying->activate();
	}
	else {
		if (isOutlining) {
			isOutlining->outline = false;
		}
		btRigidBody *bod = getBodyInFront(physicsHandler);
		
		if (bod && bod->getInvMass() > 0.001f) {
			isOutlining = physicsHandler.btModelMap[bod].model;
			isOutlining->outline = true;

		}

	}

	bool didPlaceObject = false;
	if (isWriting) {
		for (unsigned int i : char_callbacks) {
			written += (char)i;
		}
	}

	// remove duplicates

	for (KeyStruct k : key_callbacks) {
		if (k.key == GLFW_KEY_BACKSPACE && k.action == GLFW_PRESS && isWriting) {
			if (written.size() > 0)
				written.pop_back();
			
		}

		if (k.key == GLFW_KEY_ENTER && k.action == GLFW_PRESS) {
			isWriting = !isWriting;
			if (!Player::isWriting) {
				CustomFunction func(written);
				if (func.expr) {
					// build a new shape
					CustomShape *shape = new CustomShape(s, func);
					if (shape->vertices.size() > 2) {
						glm::vec3 offset = cross(cam.Up, cam.Right) * 5.0f;
						shape->transform[3] = mpc.model->transform[3] + glm::vec4(offset.x, offset.y, offset.z, 0);
						shape->color = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
						modelHandler.addModel(shape);
						ModelPhysicsCoordinator mpc = ModelPhysicsCoordinator(shape, CollisionType::custom, shape->mass, glm::vec3(1,0,0), shape->collisionShape);
						shape->outlineColor = 1 / mpc.btModel->getInvMass() < 2.5f ? glm::vec3(0, 1, 0) : glm::vec3(1,0,0);
						physicsHandler.addMPC(mpc);
					}
					else {
						delete shape;
						texts.push_back(TextStruct{ "Insufficient number of sample points fulfilling equation", float(glfwGetTime() + 3.0)});
					}

				}
				else {
					texts.push_back(TextStruct{ "Invalid expression", float(glfwGetTime() + 1.0) });

				}
				written = "";
				didPlaceObject = true;
			}
		}
		if (k.key == GLFW_KEY_K && k.action == GLFW_PRESS && !isWriting)
			drawShadows = !drawShadows;

		if (k.key == GLFW_KEY_ESCAPE && k.action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);


		if (k.key == GLFW_KEY_E && k.action == GLFW_PRESS && !isWriting) {
			if (!carrying) {
				btRigidBody *body = getBodyInFront(physicsHandler);
				if (body && 1/body->getInvMass() < 2.5f) {
					std::cout << "mass: " << 1 / body->getInvMass() << std::endl;
					carrying = body;
					//physicsHandler.btModelMap[body].model->outline = true;
					prevFlags = body->getCollisionFlags();
					body->setLinearVelocity(btVector3(0, 0, 0));
					body->setAngularVelocity(btVector3(0, 0, 0));
					body->setGravity(btVector3(0, 0, 0));
				}
			}
			else {
				//physicsHandler.btModelMap[carrying].model->outline = false;
				carrying->setGravity(btVector3(0,-9.81, 0));
				carrying = nullptr;
			}
		}
		}
	if (!isWriting) {
		glm::vec3 toMove;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			toMove += cam.Front;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			toMove -= cam.Front;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			toMove -= cam.Right;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			toMove += cam.Right;

		toMove.y = 0;


		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && glfwGetTime() - lastJump > 0.2) {
			btVector3 start = mpc.btModel->getWorldTransform().getOrigin();
			btVector3 end = start + btVector3(0, -2.2, 0);

			btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
			physicsHandler.dynamicsWorld->rayTest(start, end, RayCallback);
			if (RayCallback.hasHit()) {
				toMove += glm::vec3(0, 1, 0) * 5.0f;
				lastJump = glfwGetTime();
			}

		}
		toMove *= movementSpeed / toMove.length();
		mpc.btModel->setLinearVelocity(btVector3(toMove[0], min(toMove[1] + mpc.btModel->getLinearVelocity()[1], 5.0f), toMove[2]));
		mpc.btModel->activate();
		}
	key_callbacks.clear();
	char_callbacks.clear();
	return didPlaceObject;


}

btRigidBody * Player::getBodyInFront(PhysicsHandler &physicsHandler)
{
	glm::vec3 offset = cross(cam.Up, cam.Right) * 5.0f;
	btVector3 start = mpc.btModel->getWorldTransform().getOrigin() + btVector3(offset.x, offset.y, offset.z)*0.1;
	btVector3 end = start + btVector3(offset.x, offset.y, offset.z);
	btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
	physicsHandler.dynamicsWorld->rayTest(start, end, RayCallback);
	if (RayCallback.hasHit()) {
		return (btRigidBody*)RayCallback.m_collisionObject;
	}
	return nullptr;
}

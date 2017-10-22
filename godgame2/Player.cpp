#include "Player.h"
#include "BaseLibrary.h"
#include "tinyexpr.h"
#include "CustomShape.h"
#include <set>
#include <algorithm>
bool Player::processInput(GLFWwindow *window, std::vector<unsigned int> &char_callbacks, std::vector<KeyStruct> &key_callbacks, float time, ModelHandler &modelHandler, PhysicsHandler &physicsHandler, Shader *s)
{

	if (carrying) {
		glm::vec3 offset = cross(cam.Up, cam.Right) * 3.0f;
		btVector3 newLoc = mpc.btModel->getWorldTransform().getOrigin() + btVector3(offset.x, offset.y, offset.z);
		auto &trans = carrying->getWorldTransform();
		trans.setOrigin(newLoc);
		carrying->activate();

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
					CustomShape *shape = new CustomShape(s, func);
					glm::vec3 offset = cross(cam.Up, cam.Right) * 5.0f;
					shape->transform[3] = mpc.model->transform[3] + glm::vec4(offset.x, offset.y, offset.z, 0);
					shape->color = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
					modelHandler.addModel(shape);
					physicsHandler.addMPC(ModelPhysicsCoordinator(shape, CollisionType::custom, getHullVolume(shape->vertices, shape->indicies)));
				}
				written = "";
				didPlaceObject = true;
			}
		}

		if (k.key == GLFW_KEY_ESCAPE && k.action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);


		if (k.key == GLFW_KEY_E && k.action == GLFW_PRESS) {
			if (!carrying) {
				glm::vec3 offset = cross(cam.Up, cam.Right) * 10.0f;
				btVector3 start = mpc.btModel->getWorldTransform().getOrigin() + btVector3(offset.x, offset.y, offset.z)*0.1;
				btVector3 end = start + btVector3(offset.x, offset.y, offset.z);
				btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
				physicsHandler.dynamicsWorld->rayTest(start, end, RayCallback);
				if (RayCallback.hasHit()) {
					btRigidBody *body = (btRigidBody*)RayCallback.m_collisionObject;
					if (body && 1/body->getInvMass() < 1.5f) {
						std::cout << "mass: " << 1 / body->getInvMass() << std::endl;
						carrying = body;
						//body->mass
						prevFlags = body->getCollisionFlags();
						//body->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
						body->setLinearVelocity(btVector3(0, 0, 0));
						body->setAngularVelocity(btVector3(0, 0, 0));
						body->setGravity(btVector3(0, 0, 0));

					}
				}
			}
			else {
				carrying->setGravity(btVector3(0,-9.81, 0));
				carrying = nullptr;
			}
		}
		}
	if (!isWriting) {
		glm::vec3 toMove;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			toMove += cam.Front * movementSpeed * time;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			toMove -= cam.Front * movementSpeed * time;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			toMove -= cam.Right * movementSpeed * time;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			toMove += cam.Right * movementSpeed * time;

		toMove.y = 0;


		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && glfwGetTime() - lastJump > 0.2) {// && mpc.btModel->getLinearVelocity()[1] == 0.0f)
			btVector3 start = mpc.btModel->getWorldTransform().getOrigin();
			btVector3 end = start + btVector3(0, -2.2, 0);

			btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
			physicsHandler.dynamicsWorld->rayTest(start, end, RayCallback);
			if (RayCallback.hasHit()) {
				toMove += glm::vec3(0, 1, 0) * 5.0f;
				lastJump = glfwGetTime();
			}

		}
		mpc.btModel->setLinearVelocity(btVector3(toMove[0], min(toMove[1] + mpc.btModel->getLinearVelocity()[1], 5.0f), toMove[2]));
		mpc.btModel->activate();
		}
	key_callbacks.clear();
	char_callbacks.clear();
	return didPlaceObject;


}

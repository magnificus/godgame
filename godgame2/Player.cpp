#include "Player.h"
#include "BaseLibrary.h"
#include "tinyexpr.h"
#include "CustomShape.h"


bool Player::processInput(GLFWwindow *window, std::vector<unsigned int> &char_callbacks, std::vector<KeyStruct> &key_callbacks, float time, ModelHandler &modelHandler, PhysicsHandler &physicsHandler, Shader *s)
{
	if (isWriting) {
		for (unsigned int i : char_callbacks) {
			written += (char)i;
		}
	}
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
					shape->transform[3] = glm::vec4(0, 6, 0, 1);
					shape->color = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
					modelHandler.addModel(shape);
					physicsHandler.addMPC(ModelPhysicsCoordinator(shape, CollisionType::custom, 1));
				}
				written = "";
			}
		}

		if (k.key == GLFW_KEY_ESCAPE && k.action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
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

			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && glfwGetTime() - lastJump > 1.0) {// && mpc.btModel->getLinearVelocity()[1] == 0.0f)
				toMove += glm::vec3(0, 1, 0) * 1000.0f * time;
				lastJump = glfwGetTime();
			}

			//mpc.btModel->applyForce(btVector3(toMove[0], toMove[1], toMove[2])*100, mpc.btModel->getWorldTransform().getOrigin());
			//mpc.btModel->translate(btVector3(toMove[0], toMove[1], toMove[2]));
			mpc.btModel->setLinearVelocity(btVector3(toMove[0], toMove[1] + mpc.btModel->getLinearVelocity()[1], toMove[2]));
			mpc.btModel->activate();
		}
	key_callbacks.clear();
	char_callbacks.clear();
	return false;


}

#include "Player.h"

void Player::processInput(GLFWwindow * window, float time)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	glm::vec3 toMove;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		toMove += cam.Front * movementSpeed * time;
		//camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		toMove -= cam.Front * movementSpeed * time;
		cam.ProcessKeyboard(BACKWARD, time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		toMove -= cam.Right * movementSpeed * time;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		toMove += cam.Right * movementSpeed * time;

	toMove.y = 0;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && mpc.btModel->getWorldTransform().getOrigin()[1] < 2) {// && mpc.btModel->getLinearVelocity()[1] == 0.0f)
		toMove += glm::vec3(0, 1, 0) * 100.0f * time;
		lastJump = glfwGetTime();
	}

	//mpc.btModel->applyForce(btVector3(toMove[0], toMove[1], toMove[2])*100, mpc.btModel->getWorldTransform().getOrigin());
	//mpc.btModel->translate(btVector3(toMove[0], toMove[1], toMove[2]));
	mpc.btModel->setLinearVelocity(btVector3(toMove[0], toMove[1] + mpc.btModel->getLinearVelocity()[1], toMove[2]));


}

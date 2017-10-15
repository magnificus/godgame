#include "Player.h"


std::string Player::written;
void addToString(GLFWwindow *window, unsigned int keyCode) {
	if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
		Player::written.pop_back();

	Player::written += (char) keyCode;
}

void Player::processInput(GLFWwindow * window, float time)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	


	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && glfwGetTime() - lastEnter > 0.5) {
		lastEnter = glfwGetTime();
		isWriting = !isWriting;

		if (isWriting)
			glfwSetCharCallback(window, addToString);
		else
			glfwSetCharCallback(window, NULL);
		return;
	}
	if (isWriting) {
		// just wait

	}
	else {

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

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && mpc.btModel->getWorldTransform().getOrigin()[1] < 1.5 && glfwGetTime() - lastJump > 1.0) {// && mpc.btModel->getLinearVelocity()[1] == 0.0f)
			toMove += glm::vec3(0, 1, 0) * 1000.0f * time;
			lastJump = glfwGetTime();
		}

		//mpc.btModel->applyForce(btVector3(toMove[0], toMove[1], toMove[2])*100, mpc.btModel->getWorldTransform().getOrigin());
		//mpc.btModel->translate(btVector3(toMove[0], toMove[1], toMove[2]));
		mpc.btModel->setLinearVelocity(btVector3(toMove[0], toMove[1] + mpc.btModel->getLinearVelocity()[1], toMove[2]));
	}



}

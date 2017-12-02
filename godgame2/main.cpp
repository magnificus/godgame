//#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <iostream>
#include <ctime>
#include "Cube.h"
#include "Plane.h"
#include "Sphere.h"
#include "EmptyModel.h"
#include "ModelHandler.h"
#include "PhysicsHandler.h"
#include "inputHandler.h"
#include "btBulletDynamicsCommon.h"
#include "Player.h"
#include "BaseLibrary.h"
#include "CustomShape.h"
#include <ft2build.h>
#include <map>
#include "textHandler.h"
#include "LevelBuilder.h"

#include FT_FREETYPE_H

#define PI 3.14159

const glm::vec3 textColor = glm::vec3(0.9, 0.8f, 0.2f);
void renderModels(ModelHandler &modelHandler, glm::mat4 proj, glm::mat4 view, Shader *outlineShader) {
	unsigned int prev = 0;

	// draw standard models
	//glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0x00);
	for (int i = 0; i < modelHandler.cutoffPositions.size(); i++) {
		Model *model = modelHandler.models[i];
		if (model->outline) {
			prev = modelHandler.cutoffPositions[i];
			continue;
		}
		Shader *currentShader = model->shader;
		currentShader->setVec3("color", model->color);
		currentShader->setMat4("mvp", proj * view * model->transform);
		currentShader->setMat3("normalizer", glm::transpose(glm::inverse(glm::mat3(model->transform))));
		currentShader->setMat4("model", model->transform);
		currentShader->setFloat("timeExisted", float((glfwGetTime() - model->timeCreated)));
		// send in model
		glDrawElements(GL_TRIANGLES, (modelHandler.cutoffPositions[i] - prev), GL_UNSIGNED_INT, (void*)(prev * sizeof(GLuint)));
		prev = modelHandler.cutoffPositions[i];
	}
	prev = 0;

	//	 draw outlined models
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	for (int i = 0; i < modelHandler.cutoffPositions.size(); i++) {
		Model *model = modelHandler.models[i];
		if (!model->outline) {
			prev = modelHandler.cutoffPositions[i];
			continue;
		}
		Shader *currentShader = model->shader;
		currentShader->setVec3("color", model->color);
		currentShader->setMat4("mvp", proj * view * model->transform);
		currentShader->setMat3("normalizer", glm::transpose(glm::inverse(glm::mat3(model->transform))));
		currentShader->setMat4("model", model->transform);
		currentShader->setFloat("timeExisted", float((glfwGetTime() - model->timeCreated)));

		// send in model
		glDrawElements(GL_TRIANGLES, (modelHandler.cutoffPositions[i] - prev), GL_UNSIGNED_INT, (void*)(prev * sizeof(GLuint)));
		prev = modelHandler.cutoffPositions[i];
	}
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	outlineShader->use();
	prev = 0;

	for (int i = 0; i < modelHandler.cutoffPositions.size(); i++) {
		Model *model = modelHandler.models[i];
		if (!model->outline) {
			prev = modelHandler.cutoffPositions[i];
			continue;
		}
		glm::mat4 tf = model->transform;
		tf = glm::scale(tf, glm::vec3(1.03, 1.03, 1.03));
		outlineShader->setVec3("color", model->outlineColor);
		outlineShader->setMat4("mvp", proj * view * tf);
		outlineShader->setMat3("normalizer", glm::transpose(glm::inverse(glm::mat3(tf))));
		outlineShader->setMat4("model", tf);
		outlineShader->setFloat("timeExisted", float((glfwGetTime() - model->timeCreated)));

		// send in model
		glDrawElements(GL_TRIANGLES, (modelHandler.cutoffPositions[i] - prev), GL_UNSIGNED_INT, (void*)(prev * sizeof(GLuint)));
		prev = modelHandler.cutoffPositions[i];
	}
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);


}

void renderModelsShadow(ModelHandler &modelHandler, glm::mat4 lightMatrix, Shader *shadowShader) {
	unsigned int prev = 0;
	for (int i = 0; i < modelHandler.cutoffPositions.size(); i++) {
		Model *model = modelHandler.models[i];
		shadowShader->setMat4("model", model->transform);
		// send in model
		glDrawElements(GL_TRIANGLES, (modelHandler.cutoffPositions[i] - prev), GL_UNSIGNED_INT, (void*)(prev * sizeof(GLuint)));

		prev = modelHandler.cutoffPositions[i];
	}
}




int main()
{
	srand((unsigned)time(NULL));
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Game",NULL /* glfwGetPrimaryMonitor() */, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSwapInterval(0);
	Player player;

	camera = &(player.cam);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCharCallback(window, charCallback);


	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------

	//glDepthFunc(GL_ALWAYS);


	// build and compile our shader zprogram
	// ------------------------------------
	Shader stencilShader("standard_shader.vs", "simple_shader.fs");
	Shader textShader("text_shader.vs", "text_shader.fs");
	Shader shader1("standard_shader.vs", "standard_shader.fs");
	Shader simpleDepthShader("point_shadows_shader.vs", "point_shadows_shader.fs", "point_shadows_shader.gs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------


	ModelHandler modelHandler;
	ModelHandler stencilModelHandler;
	PhysicsHandler physicsHandler;

	// set up all of our game objects

	Sphere *light = new Sphere(&shader1);
	light->color = glm::vec3(10, 10, 10);
	light->transform *= 0.5;
	light->transform[3] = glm::vec4(0, 3, 0, 1);
	light->scale(glm::vec3(0.5, 0.5, 0.5));

	modelHandler.addModel(light);
	physicsHandler.addMPC(ModelPhysicsCoordinator(light, CollisionType::sphere, 0.3));

	LevelBuilder::getLevel1(physicsHandler, modelHandler, shader1);



	btRigidBody *playerBt = player.mpc.btModel;

	physicsHandler.addMPC(player.mpc);
	RenderInfo info = modelHandler.getRenderInfo();

	std::vector<glm::vec3> finalPositions;
	for (int i = 0; i < info.vertices.size(); i++) {
		finalPositions.push_back(info.vertices[i]);
		finalPositions.push_back(info.normals[i]);
	}

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, finalPositions.size() * sizeof(glm::vec3), &finalPositions[0], GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.indices.size() * sizeof(unsigned int), &info.indices[0], GL_STATIC_DRAW);


	// shadows
	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth cubemap texture
	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	shader1.use();
	shader1.setInt("depthMap", 1);


	clock_t start = 0;
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// FREETYPE STUFF
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	setUpCharacters();
	glm::mat4 textProjection = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f);

	GLuint textVAO, textVBO;
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// stencil stuff
	glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_EQUAL, 1, 0xFF);


	unsigned int count = 0;
	bool drawShadows = true;
	float prev = glfwGetTime();
	std::string fpsString = "";
	while (!glfwWindowShouldClose(window))
	{

		glBindVertexArray(VAO);

		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// physics
		physicsHandler.simulationTick(deltaTime);
		btVector3 trans = player.mpc.btModel->getWorldTransform().getOrigin();
		camera->Position = glm::vec3(trans[0], trans[1], trans[2]);
		// input
		// -----
		//light->transform[3] = glm::vec4(cos(currentFrame) * 2, sin(currentFrame/2) +7, sin(currentFrame) * 3, 1);

		// update models
		glm::vec3 lightLocation = glm::vec3(light->transform[3][0], light->transform[3][1], light->transform[3][2]);
		float near_plane = 1.0f;
		float far_plane = 100.0f;

		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera->GetViewMatrix();

		// render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightLocation, lightLocation + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightLocation, lightLocation + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightLocation, lightLocation + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightLocation, lightLocation + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightLocation, lightLocation + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightLocation, lightLocation + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));


		glCullFace(GL_FRONT);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			simpleDepthShader.use();
			for (unsigned int i = 0; i < shadowTransforms.size(); ++i)
				simpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
			simpleDepthShader.setFloat("far_plane", far_plane);
			simpleDepthShader.setVec3("lightPos", lightLocation);
			renderModelsShadow(modelHandler, projection, &simpleDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);

		// render everything
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapFBO);
		glBindVertexArray(VAO);

		float time = (std::clock() - start);
		shader1.use();
		//shader1.setMat4("projection", projection);
		//shader1.setMat4("view", view);


		shader1.setFloat("time", time / 100);
		shader1.setVec3("viewPos", camera->Position);
		shader1.setVec3("lightPos", lightLocation);
		shader1.setFloat("far_plane", far_plane);
		shader1.setInt("depthMap", 0);
		shader1.setBool("shadows", drawShadows);


		if (player.processInput(window, char_callbacks, key_callbacks, drawShadows, texts, deltaTime, modelHandler, physicsHandler, &shader1)) {
			finalPositions.clear();
			info = modelHandler.getRenderInfo();
			for (int i = 0; i < info.vertices.size(); i++) {
				finalPositions.push_back(info.vertices[i]);
				finalPositions.push_back(info.normals[i]);
			}
			//glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, finalPositions.size() * sizeof(glm::vec3), &finalPositions[0], GL_STATIC_DRAW);


			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			// normals
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.indices.size() * sizeof(unsigned int), &info.indices[0], GL_STATIC_DRAW);
		}
		glEnable(GL_DEPTH_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		renderModels(modelHandler, projection, view, &stencilShader);
		//shader1.use();
		// text
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		if (player.isWriting) {
			//std::cout << Player::written << std::endl;
			RenderText(textShader, player.written, 25.0f, 25.0f, 1.0f, textColor, textVAO, textVBO, textProjection);
			RenderText(textShader, "Write your equation using the variables x,y,z for example x-y, finish with enter", 25.0f, 850.0f, 1.0f, textColor, textVAO, textVBO, textProjection);
			RenderText(textShader, "Available methods:", 25.0f, 750.0f, 1.0f, textColor, textVAO, textVBO, textProjection);
			RenderText(textShader, "< > + - / * ^ %", 25.0f, 700.0f, 1.0f, textColor, textVAO, textVBO, textProjection);
			RenderText(textShader, "min, max, abs, acos, asin, atan, atan2, ceil, cos, cosh,", 25.0f, 650.0f, 1.0f, textColor, textVAO, textVBO, textProjection);
			RenderText(textShader, "exp, floor, ln, log, log10, pow, sin, sinh, sqrt, tan, tanh", 25.0f, 600.0f, 1.0f, textColor, textVAO, textVBO, textProjection);
			RenderText(textShader, "constans: pi, e", 25.0f, 550.0f, 1.0f, textColor, textVAO, textVBO, textProjection);


		}
		else {
			RenderText(textShader, "ENTER to write", 25.0f, 25.0f, 1.0f, textColor, textVAO, textVBO, textProjection);
		}

		
		auto it = texts.begin();
		float testAgainst = glfwGetTime();
		while (it != texts.end() && it->destroyTime < testAgainst) {
			++it;
			texts.pop_front();
		}
		int yPos = 400;

		it = texts.begin();
		while (it != texts.end()) {
			RenderText(textShader, (*it).text, 25.0f, yPos, 1.0f, textColor, textVAO, textVBO, textProjection);
			yPos -= 50;
			++it;

		}

		if (++count % 100 == 0) {
			float frameTime = glfwGetTime() - prev;
			prev = glfwGetTime();
			fpsString = "fps: " + std::to_string((100 / frameTime));
		}
		RenderText(textShader, fpsString, 25.0f, 1000.0f, 1.0f, textColor, textVAO, textVBO, textProjection);
		

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	//for (ModelPhysicsCoordinator b : physicsHandler.models)
	//	free (b.btModel);

	//for (Model *m : modelHandler.models)
	//	delete m;


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}
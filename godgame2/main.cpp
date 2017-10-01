//#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>

#include <iostream>

#include <ctime>

#include "Cube.h"
#include "Plane.h"
#include "Sphere.h"
#include "ModelHandler.h"
#include "inputHandler.h"
#include "btBulletDynamicsCommon.h"



void renderModels(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos,  glm::vec3 lightPos, float far_plane, float start, ModelHandler &modelHandler,  Shader *overrideShader = nullptr) {
	unsigned int prev = 0;
	float time = (std::clock() - start);
	for (int i = 0; i < modelHandler.cutoffPositions.size(); i++) {
		Model *model = modelHandler.models[i];

		Shader &currentShader = overrideShader ? *overrideShader : *model->shader;
		if (!overrideShader) {
			// set up shader
			currentShader.use();
			currentShader.setMat4("projection", projection);
			currentShader.setMat4("view", view);
			currentShader.setFloat("time", time / 100);
			currentShader.setVec3("viewPos", viewPos);
			currentShader.setVec3("lightPos", lightPos);
			currentShader.setFloat("far_plane", far_plane);
			currentShader.setInt("depthMap", 0);

			currentShader.setVec3("color", model->color);
		}
		// send in model
		glm::mat4 spaceModel;
		spaceModel = glm::translate(spaceModel, model->worldPos);
		currentShader.setMat4("model", spaceModel);
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Game", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

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
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);


	// build and compile our shader zprogram
	// ------------------------------------
	Shader shader1("standard_shader.vs", "standard_shader.fs");
	Shader simpleShader("standard_shader.vs", "standard_shader.fs");
	//Shader shadowShader("shadow_shader.vs", "shadow_shader.fs");
	//Shader debugDepthQuad("depth_quad_shader.vs", "depth_quad_shader.fs");
	Shader simpleDepthShader("point_shadows_shader.vs", "point_shadows_shader.fs", "point_shadows_shader.gs");


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	std::vector<glm::vec3> finalPositions;

	ModelHandler modelHandler;
	Cube c(&shader1);
	c.color = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	Cube light(&shader1);
	light.color = glm::vec3(10, 10, 10);
	light.worldPos = glm::vec3(0, 3, 0);
	//light.castShadows = false;
	for (glm::vec3 &vec: light.vertices) {
		vec *= 0.5;

	}
	Sphere s(&shader1);
	s.worldPos = glm::vec3(-4, 0.5, 2);
	s.color = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	Cube c2(&shader1);
	c2.color = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	c2.worldPos.y += 1;
	c.worldPos.x -= 2;
	c.worldPos.z += 1;
	Plane p(&shader1);
	p.color = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	p.worldPos.y -= 0.5;
	modelHandler.addModel(&s);
	modelHandler.addModel(&light);
	modelHandler.addModel(&c);
	modelHandler.addModel(&p);
	modelHandler.addModel(&c2);

	RenderInfo info = modelHandler.getRenderInfo();

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
	glBufferData(GL_ARRAY_BUFFER, finalPositions.size() * sizeof(glm::vec3), &finalPositions[0], GL_DYNAMIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.indices.size() * sizeof(unsigned int), &info.indices[0], GL_DYNAMIC_DRAW);


	// shadows
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
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
	//glEnable(GL_BLEND);

	// render loop
	// -----------


	unsigned int count = 0;
	float prev = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		if (++count % 100 == 0) {
			float frameTime = glfwGetTime() - prev;
			prev = glfwGetTime();
			std::cout << "fps: " << 100 / frameTime;
		}

		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		processInput(window);

		light.worldPos.z = sin(currentFrame) * 3;
		light.worldPos.x = cos(currentFrame) * 2;

		// update models
		RenderInfo info = modelHandler.getRenderInfo();

		finalPositions.clear();
		for (int i = 0; i < info.vertices.size(); i++) {
			finalPositions.push_back(info.vertices[i]);
			finalPositions.push_back(info.normals[i]);
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, finalPositions.size() * sizeof(glm::vec3), &finalPositions[0], GL_DYNAMIC_DRAW);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float near_plane = 1.0f;
		float far_plane = 25.0f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(light.worldPos, light.worldPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(light.worldPos, light.worldPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(light.worldPos, light.worldPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(light.worldPos, light.worldPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(light.worldPos, light.worldPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(light.worldPos, light.worldPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));


		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			simpleDepthShader.use();
			for (unsigned int i = 0; i < 6; ++i)
				simpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
			simpleDepthShader.setFloat("far_plane", far_plane);
			simpleDepthShader.setVec3("lightPos", light.worldPos);
			renderModels(projection, view, camera.Position, light.worldPos, far_plane, start, modelHandler, &simpleDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// render everything
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapFBO);
		glBindVertexArray(VAO);

		renderModels(projection, view, camera.Position, light.worldPos, far_plane, start, modelHandler);

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

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}



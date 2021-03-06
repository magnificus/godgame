#pragma once
#include <glm/glm.hpp>
#include <learnopengl/shader_m.h>
#include <vector>
#include "BaseLibrary.h"
#include <GLFW/glfw3.h>

class Model {

public:
	Model(Shader *s) {
		shader = s;
		timeCreated = glfwGetTime() - 2;
	}
	~Model() {}

	void scale(glm::vec3 modifier);
	double getMaxDistanceAcross();

	glm::mat4 transform;
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indicies;
	std::vector<glm::vec3> normals;
	Shader *shader;
	glm::vec3 color = glm::vec3(1.0,1.0,1.0);
	bool outline = false;
	glm::vec3 outlineColor = glm::vec3(1.0, 1.0, 1.0);
	double timeCreated = -100.0;
	float transparency = 1.0;

};
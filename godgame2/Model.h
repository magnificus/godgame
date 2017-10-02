#pragma once
#include <glm/glm.hpp>
#include <learnopengl/shader_m.h>
#include <vector>

class Model {

public:
	Model(Shader *s) {
		shader = s;
	}
	glm::mat4 transform;
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indicies;
	std::vector<glm::vec3> normals;
	Shader *shader;
	glm::vec3 color = glm::vec3(1.0,1.0,1.0);
	bool castShadows = true;

};
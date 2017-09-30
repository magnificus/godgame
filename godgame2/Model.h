#pragma once
#include <glm/glm.hpp>
#include <learnopengl/shader_m.h>
#include <vector>

class Model {

public:
	glm::vec3 worldPos = glm::vec3(0,0,0);
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indicies;
	std::vector<glm::vec3> normals;
	Shader *shader;
	glm::vec3 color = glm::vec3(1.0,1.0,1.0);
	bool castShadows = true;


private:
	//virtual std::vector<glm::vec3> getVertices() = 0;
	//virtual std::vector<unsigned int> getIndicies() = 0;
	//virtual std::vector<glm::vec3> getNormals() = 0;

};
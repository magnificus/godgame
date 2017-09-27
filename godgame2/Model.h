#pragma once
#include <glm/glm.hpp>
#include <vector>

class Model {

public:
	glm::vec3 worldPos = glm::vec3(0,0,0);
	virtual std::vector<glm::vec3> getVertices() = 0;
	virtual std::vector<unsigned int> getIndicies() = 0;
	virtual std::vector<glm::vec3> getNormals() = 0;
	unsigned int VAO;
private:
	std::vector<glm::vec3> vertices;
};
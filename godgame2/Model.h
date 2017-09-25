#pragma once
#include <glm/glm.hpp>
#include <vector>

class Model {

public:
	glm::vec3 worldPos;
	virtual std::vector<glm::vec3> getVertices() = 0;
	virtual std::vector<unsigned int> getIndicies() = 0;

private:
	std::vector<glm::vec3> vertices;
};
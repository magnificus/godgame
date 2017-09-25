#pragma once
#include <glm/glm.hpp>
#include <vector>

class Model {

public:
	std::vector<glm::vec3> getVertices;

private:
	std::vector<glm::vec3> vertices;
};
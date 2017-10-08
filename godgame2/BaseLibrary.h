#pragma once
#include <glm/glm.hpp>

struct RenderInfo {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
};

typedef float(*shapeFunction)(float x, float y, float z);

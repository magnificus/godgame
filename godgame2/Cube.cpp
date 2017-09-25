#include "Cube.h"

std::vector<glm::vec3> Cube::getVertices() {

	return std::vector<glm::vec3>{glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{ -0.5f, -0.5f, 0.5f },
		glm::vec3{ -0.5f, 0.5f, -0.5f }, glm::vec3{ -0.5f, 0.5f, 0.5f }, glm::vec3{ 0.5f, -0.5f, -0.5f },
		glm::vec3{ 0.5f, -0.5f, 0.5f }, glm::vec3{ 0.5f, 0.5f, -0.5f }, glm::vec3{ 0.5f, 0.5f, 0.5f }
	};
}
std::vector<unsigned int> Cube::getIndicies() {
	
	return std::vector<unsigned int>{ 2, 0, 1, 2, 1, 3, 1,5, 7, 7, 3, 1,1,0,4,5,1,4,7,6,3,6,2,3, 4,6,7,7,5,4, 0,2,6, 6,4,0};
}
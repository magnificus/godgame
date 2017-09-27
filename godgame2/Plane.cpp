#include "Plane.h"



std::vector<glm::vec3> Plane::getVertices() {

	return std::vector<glm::vec3>{glm::vec3{ -50.5f, 0.0f, -50.5f }, glm::vec3{ -50.5f, 0.0f, 50.5f }, glm::vec3{ 50.5f, 0.0f, 50.5f }, glm::vec3{ 50.5f, 0.f, -50.5f }
	};
}
std::vector<unsigned int> Plane::getIndicies() {

	return std::vector<unsigned int>{ 0,1,2,0,2,3};
}

std::vector<glm::vec3> Plane::getNormals() {

	std::vector<glm::vec3> toReturn;
	for (int i = 0; i < 4; i++)
		toReturn.push_back(glm::vec3(0, 1, 0));
	return toReturn;
}
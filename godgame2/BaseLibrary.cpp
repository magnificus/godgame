#pragma once
#include "BaseLibrary.h"

double getHullVolume(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices) {
	double totVol = 0;
	glm::vec3 origin = glm::vec3(-1, -1, -1);
	for (int i = 0; i < indices.size() - 2; i += 3) {
		glm::vec3 v1 = vertices[indices[i]];
		glm::vec3 v2 = vertices[indices[i + 1]];
		glm::vec3 v3 = vertices[indices[i + 2]];
		glm::vec3 norm = -glm::cross((v1 - v2), (v1 - v3));
		norm = glm::normalize(norm);

		double triangleArea = 0.5*glm::length(glm::cross((v1 - v2), (v3 - v2)));
		//std::cout << std::endl << "volume of new triangle: " << triangleArea << std::endl;
		double toAdd = triangleArea * glm::dot(v1, norm) / 3;
		if (std::abs(toAdd) < 1000.0f)
			totVol += toAdd;

	}
	std::cout << std::endl << "volume of new shape: " << totVol << std::endl;

	return std::abs(totVol);
}

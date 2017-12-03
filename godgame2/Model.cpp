#include "Model.h"

void Model::scale(glm::vec3 modifier)
{
	for (glm::vec3 &vec : vertices)
		vec *= modifier;
}

double Model::getMaxDistanceAcross() {
	double longest = 0;
	for (int i = 0; i < vertices.size(); i++) {
		for (int j = i + 1; j < vertices.size(); j++) {
			longest = max(longest, glm::length(vertices[i] - vertices[j]));
		}
	}
	return longest;
}
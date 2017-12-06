#include "Model.h"

void Model::scale(glm::vec3 modifier)
{
	for (glm::vec3 &vec : vertices)
		vec *= modifier;
}

double Model::getMaxDistanceAcross() {
	double longest = 0;
	for (glm::vec3 &vert : vertices) {
		longest = max(vert.length(), longest);
	}
	return longest;
}
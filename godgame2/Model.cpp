#include "Model.h"

void Model::scale(glm::vec3 modifier)
{
	for (glm::vec3 &vec : vertices)
		vec *= modifier;
}

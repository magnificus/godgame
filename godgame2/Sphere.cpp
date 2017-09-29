#include "Sphere.h"

Sphere::Sphere()
{
	indicies = getIndicies();

}

std::vector<glm::vec3> Sphere::getVertices() {

	std::vector<glm::vec3> points;

	return points;

}
std::vector<unsigned int> Sphere::getIndicies() {
	std::vector<unsigned int> indices;
	return indices;
}


std::vector<glm::vec3> Sphere::getNormals() {

	std::vector<glm::vec3> normals;
	return normals;
}
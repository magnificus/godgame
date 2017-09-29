#pragma once
#include "Model.h"

class Sphere : public Model {

public:
	Sphere();
	virtual std::vector<glm::vec3> getVertices();
	virtual std::vector<unsigned int> getIndicies();
	virtual std::vector<glm::vec3> getNormals();

};
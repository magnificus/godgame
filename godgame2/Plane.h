#pragma once
#include "Model.h"
class Plane : public Model {

public:
	virtual std::vector<glm::vec3> getVertices();
	virtual std::vector<unsigned int> getIndicies();
	virtual std::vector<glm::vec3> getNormals();

};
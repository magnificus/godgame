#pragma once
#include "Model.h"

class Cube : public Model {

public:
	virtual std::vector<glm::vec3> getVertices();
	virtual std::vector<unsigned int> getIndicies();

};
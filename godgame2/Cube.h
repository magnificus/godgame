#pragma once
#include "Model.h"

class Cube : public Model {
public:
	Cube();
protected:
	virtual std::vector<glm::vec3> getVertices();
	virtual std::vector<unsigned int> getIndicies();
	virtual std::vector<glm::vec3> getNormals();

};
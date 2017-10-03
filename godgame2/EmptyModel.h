#pragma once
#include "Model.h";

class EmptyModel : public Model {
public:
	EmptyModel() : Model(nullptr) { transform = glm::mat4(); transform[3][1] = 1; };

};
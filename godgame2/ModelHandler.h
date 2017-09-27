#pragma once
#include "Model.h"

struct RenderInfo{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
};

class ModelHandler {

public:
	RenderInfo getRenderInfo();
	std::vector<unsigned int> cutoffPositions;
	void addModel(Model *m);
	std::vector<Model*> models;
	unsigned int VAO;

private:


};
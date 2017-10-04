#pragma once
#include "Model.h"
#include "BaseLibrary.h"

class ModelHandler {

public:
	RenderInfo getRenderInfo();
	std::vector<unsigned int> cutoffPositions;
	void addModel(Model *m);
	std::vector<Model*> models;
	unsigned int VAO;

private:


};
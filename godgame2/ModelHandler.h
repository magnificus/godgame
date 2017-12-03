#pragma once
#include "Model.h"
#include "BaseLibrary.h"

class ModelHandler {

public:
	~ModelHandler();
	void clearModels();
	void renderModels(bool outline, glm::mat4 proj, glm::mat4 view, Shader *overrideShader = nullptr, glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0), bool outlineColor = false);

	RenderInfo getRenderInfo();
	std::vector<unsigned int> cutoffPositions;
	void addModel(Model *m);
	std::vector<Model*> models;
	unsigned int VAO;

private:


};
#include "ModelHandler.h"
#include <glm/gtc/matrix_transform.hpp>

void ModelHandler::clearModels() {


	//for (auto it = models.begin(); it != models.end(); ++it)
	//{
	//	//std::cout << "deleting: " << (*it)- << std::endl;
	//	delete *it; 
	//}
	models.clear();
	cutoffPositions.clear();
	//for (Model *m : models) {
	//	delete m;
	//}
}

ModelHandler::~ModelHandler() {
	clearModels();
}

void ModelHandler::renderModels(bool outline, glm::mat4 proj, glm::mat4 view, Shader *overrideShader, glm::vec3 scale, bool outlineColor) {
	int prev = 0;
	for (int i = 0; i < cutoffPositions.size(); i++) {
		Model *model = models[i];
		if (model->outline ^ outline) {
			prev = cutoffPositions[i];
			continue;
		}
		Shader *currentShader = overrideShader ? overrideShader : model->shader;
		glm::mat4 tf = model->transform;
		tf = glm::scale(tf, scale);
		currentShader->setVec3("color", outlineColor ? model->outlineColor : model->color);
		currentShader->setMat4("mvp", proj * view * tf);
		currentShader->setMat3("normalizer", glm::transpose(glm::inverse(glm::mat3(tf))));
		currentShader->setMat4("model", tf);
		currentShader->setFloat("timeExisted", float((glfwGetTime() - model->timeCreated)));
		// send in model
		glDrawElements(GL_TRIANGLES, (cutoffPositions[i] - prev), GL_UNSIGNED_INT, (void*)(prev * sizeof(GLuint)));
		prev = cutoffPositions[i];
	}
}

RenderInfo ModelHandler::getRenderInfo()
{
	RenderInfo info;
	cutoffPositions.clear();
	for (Model *m : models) {
		auto currVerts = m->vertices;
		auto currIndices = m->indicies;
		auto currNormals = m->normals;

		for (unsigned int &i : currIndices)
			i += info.vertices.size();

		info.vertices.insert(info.vertices.end(), currVerts.begin(), currVerts.end());
		info.normals.insert(info.normals.end(), currNormals.begin(), currNormals.end());
		info.indices.insert(info.indices.end(), currIndices.begin(), currIndices.end());

		cutoffPositions.push_back(info.indices.size());
	}

	return info;
}

void ModelHandler::addModel(Model *m)
{
	models.push_back(m);
}

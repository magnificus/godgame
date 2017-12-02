#include "ModelHandler.h"

void ModelHandler::clearModels() {
	models.clear();
	cutoffPositions.clear();

	for (auto it = models.begin(); it != models.end(); ++it)
	{
		//std::cout << "deleting: " << (*it)- << std::endl;
		delete *it; 
	}
	//for (Model *m : models) {
	//	delete m;
	//}
}

ModelHandler::~ModelHandler() {
	clearModels();
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

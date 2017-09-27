#include "ModelHandler.h"

RenderInfo ModelHandler::getRenderInfo()
{
	RenderInfo info;
	cutoffPositions.clear();
	for (Model *m : models) {
		auto currVerts = m->getVertices();
		auto currIndices = m->getIndicies();
		auto currNormals = m->getNormals();

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

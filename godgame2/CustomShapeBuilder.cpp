#include "CustomShapeBuilder.h"
#include <algorithm>
#include "QuickHull.hpp"
#include <map>
#include <set>
//#include "Structs\Vector3.hpp"

using namespace quickhull;
RenderInfo CustomShapeBuilder::buildShape(CustomFunction &f)
{
	std::vector<glm::vec3> samples = getSamplePositions(f);
	std::vector<quickhull::Vector3<float>> pointCloud;
	QuickHull<float> qh;
	for (glm::vec3 sample : samples) {
		pointCloud.push_back(Vector3<float>( sample.x, sample.y, sample.z ));
	}
	auto hull = qh.getConvexHull(pointCloud, false, false);
	auto indexBuffer = hull.getIndexBuffer();
	auto vertexBuffer = hull.getVertexBuffer();

	RenderInfo toReturn;
	glm::vec3 center;

	std::map<unsigned int, glm::vec3>  found;
	//sstd::set<unsigned int> found;
	for (auto vertex : vertexBuffer) {
		glm::vec3 curr = glm::vec3(vertex.x, vertex.y, vertex.z);
		center += curr;
		toReturn.vertices.push_back(curr);
		//toReturn.normals.push_back(glm::vec3(0, 1, 0));

	}
	center /= vertexBuffer.size();
	for (int i = 0; i < indexBuffer.size(); i++) {
			toReturn.indices.push_back(indexBuffer[i]);

	}
	for (int i = 0; i < toReturn.vertices.size(); i++) {
		glm::vec3 curr = toReturn.vertices[i] - center;
		curr = glm::normalize(curr);
		toReturn.normals.push_back(curr);
	}

	for (int i = 0; i < toReturn.indices.size() - 2; i+=3) {
		glm::vec3 v1 = toReturn.vertices[toReturn.indices[i]];
		glm::vec3 v2 = toReturn.vertices[toReturn.indices[i+1]];
		glm::vec3 v3 = toReturn.vertices[toReturn.indices[i+2]];

		glm::vec3 norm = glm::cross(v2 - v1, v3 - v1);

		norm = glm::normalize(norm);

		float tolerance = 0.5f;
		if (found.count(toReturn.indices[i]) && glm::dot(norm, found[toReturn.indices[i]]) < tolerance) {
			toReturn.vertices.push_back(toReturn.vertices[toReturn.indices[i]]);
			toReturn.indices[i] = toReturn.vertices.size() - 1;
			toReturn.normals.push_back(glm::vec3());

		}
		toReturn.normals[toReturn.indices[i]] = norm;
		found[toReturn.indices[i]] = norm;

		if (found.count(toReturn.indices[i+1]) && glm::dot(norm, found[toReturn.indices[i+1]]) < tolerance) {
			toReturn.vertices.push_back(toReturn.vertices[toReturn.indices[i+1]]);
			toReturn.indices[i+1] = toReturn.vertices.size() - 1;
			toReturn.normals.push_back(glm::vec3());

		}
		toReturn.normals[toReturn.indices[i+1]] = norm;
		found[toReturn.indices[i + 1]] = norm;

		if (found.count(toReturn.indices[i+2]) && glm::dot(norm, found[toReturn.indices[i+2]]) < tolerance) {
			toReturn.vertices.push_back(toReturn.vertices[toReturn.indices[i+2]]);
			toReturn.indices[i+2] = toReturn.vertices.size() - 1;
			toReturn.normals.push_back(glm::vec3());
		}
		toReturn.normals[toReturn.indices[i+2]] = norm;
		found[toReturn.indices[i + 2]] = norm;

	}

	return toReturn;
}

struct VecVal {
	glm::vec3 vec;
	float val;


};


std::vector<glm::vec3> CustomShapeBuilder::getSamplePositions(CustomFunction &f)
{
	std::vector<glm::vec3> acceptablePoints;
	float minFound = 100;
	float maxFound = -100;
	for (float i = -2; i <= 2; i+=0.01f) {
		if (f.eval(i, 0, 0) > 0 || f.eval(0, i, 0) > 0 || f.eval(0, 0, i) > 0){
			minFound = std::min(minFound, i);
			maxFound = std::max(maxFound, i);

		}
	}
	minFound = std::min(-1.0f, minFound);
	maxFound = std::max(1.0f, maxFound);

	float scale =  maxFound - minFound;
	float incr = 0.02*scale;
	for (float x = minFound; x <= minFound+scale; x += incr) {
		for (float y = minFound; y <= minFound + scale; y += incr) {
			float encounteredZ = minFound + scale;
			for (float z = minFound; z <= minFound + scale; z += incr) {
				if (f.eval(x, y, z) > 0) {
					acceptablePoints.push_back(glm::vec3(x, y, z));
					encounteredZ = z;
					break;
				}
			}
			for (float z = minFound + scale; z >= encounteredZ; z -= incr) {
				if (f.eval(x, y, z) > 0) {
					acceptablePoints.push_back(glm::vec3(x, y, z));
					break;
				}
			}
		}
	}
	return acceptablePoints;
}

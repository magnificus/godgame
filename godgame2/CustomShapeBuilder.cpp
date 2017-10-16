#include "CustomShapeBuilder.h"
#include <algorithm>
#include "QuickHull.hpp"
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
	for (int i = 0; i < vertexBuffer.size(); i++) {
		glm::vec3 curr = toReturn.vertices[i] - center;
		curr = glm::normalize(curr);
		toReturn.normals.push_back(curr);
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

	float scale = std::max(1.0f, maxFound - minFound);
	float incr = 0.02*scale;
	for (float x = minFound; x <= minFound+scale; x += incr) {
		for (float y = minFound; y <= minFound + scale; y += incr) {
			for (float z = minFound; z <= minFound + scale; z += incr) {
				if (f.eval(x, y, z) > 0)
					acceptablePoints.push_back(glm::vec3(x, y, z));
			}
		}
	}
	return acceptablePoints;
}




//std::vector<glm::vec3> CustomShapeBuilder::getSamplePositions(shapeFunction * f)
//{
//	std::vector<glm::vec3> acceptablePoints;
//	float x = 0;
//	float y = 0;
//	float z = 0;
//	// attempt to find corners
//
//	float initialRes = (*f)(0.0f, 0.0f, 0.0f);
//	float epsilon = 0.01;
//	while (initialRes > 0) {
//		x *= 2;
//		initialRes = (*f)(0.0f, 0.0f, 0.0f);
//	}
//	x /= 2;
//	float toApply = x / 2;
//	float res = (*f)(x, 0, 0);
//	while (std::abs(res) > epsilon) {
//		if (res > 0)
//			toApply = -std::abs(toApply) / 2;
//		else
//			toApply = std::abs(toApply) / 2;
//		x += toApply;
//		res = (*f)(x, 0, 0);
//	}
//	//acceptablePoints.push_back
//
//	return acceptablePoints;
//}


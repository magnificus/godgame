#include "CustomShapeBuilder.h"
#include <algorithm>
#include "QuickHull.hpp"
//#include "Structs\Vector3.hpp"

using namespace quickhull;
RenderInfo CustomShapeBuilder::buildShape(shapeFunction * f)
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

	}
	center /= vertexBuffer.size();
	for (int i = 0; i < indexBuffer.size(); i++) {
		toReturn.indices.push_back(indexBuffer[i]);
	}
	for (int i = 0; i < vertexBuffer.size(); i++) {
		toReturn.normals.push_back(toReturn.vertices[i] - center);
	}


	return toReturn;
}

struct VecVal {
	glm::vec3 vec;
	float val;


};


std::vector<glm::vec3> CustomShapeBuilder::getSamplePositions(shapeFunction * f)
{
	std::vector<VecVal> points;
	std::vector<glm::vec3> acceptablePoints;
	float incr = 0.05;
	for (float x = -1; x <= 1; x += incr) {
		for (float y = -1; y <= 1; y += incr) {
			for (float z = -1; z <= 1; z += incr) {
				VecVal curr;
				curr.vec = glm::vec3(x, y, z);
				curr.val = (*f)(x, y, z);
				points.push_back(curr);
				if (curr.val > 0)
					acceptablePoints.push_back(glm::vec3(x, y, z));
			}
		}
	}
	std::sort(points.begin(), points.end(), [](const VecVal & a,const  VecVal & b) {return a.val < b.val;});
	std::vector<glm::vec3> toReturn;
	for (int i = 0; i < points.size(); i++) {
		toReturn.push_back(points[i].vec);
	}
	return acceptablePoints;
}

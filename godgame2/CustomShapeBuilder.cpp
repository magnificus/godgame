#include "CustomShapeBuilder.h"
#include <algorithm>
#include "QuickHull.hpp"
#include <map>
#include <set>
#include <bitset>
#include "BaseLibrary.h"
#include <glm\gtc\type_ptr.hpp>

using namespace quickhull;

const float defScale = 0.02;
const unsigned int sampleLength = 60;

typedef std::bitset<sampleLength*sampleLength*sampleLength> ShapeBits;

struct ShapeInfo {
	ShapeBits bits;
	glm::vec3 offset;
	float scale;
};

glm::vec3 getBitSetPositionFromReal(glm::vec3 position, float scale) {
	return position / scale;
}

unsigned int getBitsetPosition(unsigned int x, unsigned int y, unsigned int z) {
	return x + y * sampleLength + z * sampleLength * sampleLength;
}

glm::vec3 getBitAddressVector(unsigned int num, float scale, const glm::vec3 &offset) {
	return glm::vec3(num%sampleLength, (num / sampleLength) % sampleLength, num / (sampleLength*sampleLength))*scale + offset;
}
ShapeInfo getValidPositions(CustomFunction &f);


CustomMeshInfo CustomShapeBuilder::buildShape(CustomFunction &f)
{
	RenderInfo toReturn;

	ShapeInfo info = getValidPositions(f);


	ShapeBits newBits = info.bits;
	ShapeBits originalBits = info.bits;
	double mass = 0;

	std::vector<quickhull::Vector3<float>> pointClouds[8];

	glm::vec3 center = glm::vec3(0, 0, 0);
	for (unsigned int x = 0; x < sampleLength; x++) {
		for (unsigned int y = 0; y < sampleLength; y++) {
			for (unsigned int z = 0; z < sampleLength; z++) {
				unsigned int currPos = getBitsetPosition(x, y, z);
				if (info.bits[currPos]) {
					mass++;
					glm::vec3 pos = getBitAddressVector(currPos, info.scale, info.offset);
					center += pos;
				}
			}
		}
	}

	center /= mass;

	mass /= pow(sampleLength/info.scale, 3);
	mass *= 40000;

	// if every point around this point is part of the object, then obviously this point is not on any edge and is therefore not interesting
	for (unsigned int x = 1; x < sampleLength - 1; x++) {
		for (unsigned int y = 1; y < sampleLength - 1; y++) {
			for (unsigned int z = 1; z < sampleLength - 1; z++) {
				unsigned int currPos = getBitsetPosition(x, y, z);
				if (info.bits[currPos]) {
					unsigned int nextX = getBitsetPosition(x + 1, y, z);
					unsigned int nextY = getBitsetPosition(x, y + 1, z);
					unsigned int nextZ = getBitsetPosition(x, y, z + 1);
					unsigned int prevX = getBitsetPosition(x - 1, y, z);
					unsigned int prevY = getBitsetPosition(x, y - 1, z);
					unsigned int prevZ = getBitsetPosition(x, y, z - 1);
					if (info.bits[nextX] && info.bits[nextY] && info.bits[nextZ] && info.bits[prevX] && info.bits[prevY] && info.bits[prevZ])
						newBits[currPos] = false;
				}
			}
		}
	}
	info.bits = newBits;


	float marginSize = info.scale + 0.001f;
	for (unsigned int x = 0; x < sampleLength; x++) {
		for (unsigned int y = 0; y < sampleLength; y++) {
			for (unsigned int z = 0; z < sampleLength; z++) {
				unsigned int currPos = getBitsetPosition(x, y, z);
				if (info.bits[currPos]) {
					glm::vec3 pos = getBitAddressVector(currPos, info.scale, info.offset) - center;
					// for middle cases we have to add to multiple pools
					std::vector<unsigned int> xToAdd;
					std::vector<unsigned int> yToAdd;
					std::vector<unsigned int> zToAdd;
					bool addXMin = pos.x < marginSize;
					bool addXMax = pos.x > 0;
					bool addYMin = pos.y < marginSize;
					bool addYMax = pos.y > 0;
					bool addZMin = pos.z < marginSize;
					bool addZMax = pos.z > 0;

					if (addXMin)
						xToAdd.push_back(0);
					if (addXMax)
						xToAdd.push_back(4);
					if (addYMin)
						yToAdd.push_back(0);
					if (addYMax)
						yToAdd.push_back(2);
					if (addZMin)
						zToAdd.push_back(0);
					if (addZMax)
						zToAdd.push_back(1);

					for (unsigned int x2 : xToAdd) {
						for (unsigned int y2 : yToAdd) {
							for (unsigned int z2 : zToAdd) {
								pointClouds[x2 + y2 + z2].push_back(quickhull::Vector3<float>(pos.x, pos.y, pos.z));
							}
						}
					}
				}
			}
		}
	}




	quickhull::ConvexHull<float> hulls[8];
	std::vector<unsigned int> indexBuffers[8];
	quickhull::VertexDataSource<float> vertexBuffers[8];
	unsigned int currentVertexOffset = 0;
	for (int i = 0; i < 8; i++) {
		if (pointClouds[i].size() < 3)
			continue;
		QuickHull<float> qh;

		hulls[i] = qh.getConvexHull(pointClouds[i], false, false);
		indexBuffers[i] = hulls[i].getIndexBuffer();
		vertexBuffers[i] = hulls[i].getVertexBuffer();

		// add all the vertices together

		for (auto vec : vertexBuffers[i]) {
			toReturn.vertices.push_back(glm::vec3(vec.x, vec.y, vec.z));
			toReturn.normals.push_back(glm::vec3(0.0f,0.0f,0.0f));
		}

		for (unsigned int j = 0; j < indexBuffers[i].size() - 2; j+=3) {
			// if the face is deemed to not be visible from outside the shape ignore it
			glm::vec3 v1 = toReturn.vertices[indexBuffers[i][j] + currentVertexOffset];
			glm::vec3 v2 = toReturn.vertices[indexBuffers[i][j+1] + currentVertexOffset];
			glm::vec3 v3 = toReturn.vertices[indexBuffers[i][j+2] + currentVertexOffset];
			glm::vec3 norm = glm::cross(v2 - v1, v3 - v1);
			norm = glm::normalize(norm);
			glm::vec3 middle = (v1 + v2 + v3) / 3.0f;
			glm::vec3 offsetPoint = middle - info.offset + center;
			glm::vec3 measurePoint1 = middle + center;
			glm::vec3 bitPos = getBitSetPositionFromReal(measurePoint1, info.scale);


			bool foundPointOutside = false;
			float longest = std::max(std::max(abs(offsetPoint.x), abs(offsetPoint.y)), abs(offsetPoint.z));
			float shortest = std::min(std::min(abs(offsetPoint.x), abs(offsetPoint.y)), abs(offsetPoint.z));
			if (longest + 0.01 > (sampleLength - 1)*info.scale || shortest - 0.01 < 0) {
				foundPointOutside = true;
			}
			else {
				for (int x = -1; x <= 1; x++) {
					for (int y = -1; y <= 1; y++) {
						for (int z = -1; z <= 1; z++) {
							unsigned int bitNum = getBitsetPosition(bitPos.x + x, bitPos.y + y, bitPos.z + z);
							if (!originalBits[bitNum]) {
								foundPointOutside = true;
								goto outOfLoop;
							}
						}
					}
				}
			}
			outOfLoop:

			if (foundPointOutside) {
				toReturn.indices.push_back(indexBuffers[i][j] + currentVertexOffset);
				toReturn.indices.push_back(indexBuffers[i][j+1] + currentVertexOffset);
				toReturn.indices.push_back(indexBuffers[i][j+2] + currentVertexOffset);

			}
		}

		currentVertexOffset += vertexBuffers[i].size();
	}


	// fix normals by giving each vertex its own, copy vertexes where several indicies refer to the same one
	if (toReturn.indices.size() >= 3) {
		for (unsigned int i = 0; i < toReturn.indices.size() - 2; i += 3) {
			unsigned int &toUse1 = toReturn.indices[i];
			unsigned int &toUse2 = toReturn.indices[i + 1];
			unsigned int &toUse3 = toReturn.indices[i + 2];

			glm::vec3 v1 = toReturn.vertices[toUse1];
			glm::vec3 v2 = toReturn.vertices[toUse2];
			glm::vec3 v3 = toReturn.vertices[toUse3];
			glm::vec3 norm = glm::cross(v2 - v1, v3 - v1);
			norm = glm::normalize(norm);


			if (toReturn.normals[toUse1].length() != 0.0f) {
				toUse1 = toReturn.vertices.size();
				toReturn.vertices.push_back(v1);
				toReturn.normals.push_back(glm::vec3());
			}
			if (toReturn.normals[toUse2].length() != 0.0f) {
				toUse2 = toReturn.vertices.size();
				toReturn.vertices.push_back(v2);
				toReturn.normals.push_back(glm::vec3());

			}
			if (toReturn.normals[toUse3].length() != 0.0f) {
				toUse3 = toReturn.vertices.size();
				toReturn.vertices.push_back(v3);
				toReturn.normals.push_back(glm::vec3());

			}

			toReturn.normals[toUse1] = norm;
			toReturn.normals[toUse2] = norm;
			toReturn.normals[toUse3] = norm;
		}
	}




	std::vector<glm::vec3> glmVertexBuffers[8];
	for (int i = 0; i < 8; i++) {
		for (auto vec : vertexBuffers[i])
			glmVertexBuffers[i].push_back(glm::vec3(vec.x, vec.y, vec.z));
	}



	btCompoundShape *shape2 = new btCompoundShape();
	for (int i = 0; i < 8; i++) {
		if (glmVertexBuffers[i].size() < 3)
			continue;
		btConvexHullShape *poolShape = new btConvexHullShape(glm::value_ptr(glmVertexBuffers[i][0]), int(glmVertexBuffers[i].size()), sizeof(glm::vec3));
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(btVector3(0, 0, 0));
		shape2->addChildShape(trans, poolShape);
	}

	return CustomMeshInfo{ toReturn, shape2, (float)mass };
}

struct VecVal {
	glm::vec3 vec;
	float val;
};

ShapeInfo getValidPositions(CustomFunction &f) {
	ShapeInfo info;
	float minFound = 100;
	float maxFound = -100;

	float limit = 5.0;
	float incr = 0.014f;
	for (float i = -limit; i <= limit; i += incr) {
		if (f.eval(i, 0, 0) > 0 || f.eval(0, i, 0) > 0 || f.eval(0, 0, i) > 0 || f.eval(i, -limit, -limit) > 0|| f.eval(i, limit, limit) > 0 || f.eval(-limit, i, -limit) > 0 || f.eval(limit, i, limit) > 0 || f.eval(-limit, -limit, i) > 0 || f.eval(limit, limit, i)> 0) {
			minFound = std::min(minFound, i);
			maxFound = std::max(maxFound, i);

		}
	}
	minFound = std::min(-0.5f, minFound);
	maxFound = std::max(0.5f, maxFound);

	glm::vec3 offset = glm::vec3(minFound, minFound, minFound);
	float dist = maxFound - minFound;
	info.scale = dist /sampleLength;
	for (unsigned int x = 0; x < sampleLength; ++x) {
		for (unsigned int  y = 0; y < sampleLength; ++y) {
			for (unsigned int  z = 0; z < sampleLength; ++z) {
				info.bits[getBitsetPosition(x, y, z)] = f.eval(x*info.scale + offset.x, y*info.scale + offset.y, z*info.scale + offset.z) > 0;
			}
		}
	}
	return info;
}


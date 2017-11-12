#include "CustomShapeBuilder.h"
#include <algorithm>
#include "QuickHull.hpp"
#include <map>
#include <set>
#include <bitset>
using namespace quickhull;

const float defScale = 0.02;
const unsigned int sampleLength = 26;

typedef std::bitset<sampleLength*sampleLength*sampleLength> ShapeBits;

struct ShapeInfo {
	ShapeBits bits;
	glm::vec3 offset;
	float scale;
};

unsigned int getBitsetPosition(unsigned int x, unsigned int y, unsigned int z) {
	return x + y * sampleLength + z * sampleLength * sampleLength;
}

glm::vec3 getBitAddressVector(unsigned int num, float scale, const glm::vec3 &offset) {
	return glm::vec3(num%sampleLength, (num / sampleLength) % sampleLength, num / (sampleLength*sampleLength))*scale + offset;
}
ShapeInfo getValidPositions(CustomFunction &f);


void decideLatestIndicesAndNormals(std::vector<glm::vec3> &vertices, std::vector<unsigned int> &indices, std::vector<glm::vec3> &normals, CustomFunction &f, unsigned int anchor, ShapeBits &originalSet) {
	glm::vec3 v1 = vertices[vertices.size()-3];
	glm::vec3 v2 = vertices[vertices.size() - 2];
	glm::vec3 v3 = vertices[vertices.size() - 1];

	glm::vec3 norm = glm::cross(v2 - v1, v3 - v1);
	norm = glm::normalize(norm);


	glm::vec3 anchorP = getBitAddressVector(anchor, 1.0, glm::vec3(0, 0, 0));

	float count = 1;
	bool bothOK = true;
	bool useThisNormal = true;
	//norm  = abs(norm.x) > std::max(abs(norm.y), abs(norm.z)) ? glm::vec3(norm.x, 0, 0) : abs(norm.y) > abs(norm.z) ? glm::vec3(0, norm.y, 0) : glm::vec3(0, 0, norm.z);

	while (true) {
		glm::vec3 closestNormal = norm * count++;
		glm::vec3 res1 = anchorP + closestNormal;
		glm::vec3 res2 = anchorP - closestNormal;

		if (std::max(res1.x, std::max(res1.y, res1.z)) >= sampleLength || std::min(res1.x, std::min(res1.y, res1.z)) < 0) {
			useThisNormal = false;
			break;
		}
		else if (std::max(res2.x, std::max(res2.y, res2.z)) >= sampleLength || std::min(res2.x, std::min(res2.y, res2.z)) < 0) {
			break;
		}
		else if (!originalSet[getBitsetPosition(res1.x, res1.y, res1.z)]) {
			useThisNormal = false;
			break;
		}
		else if (!originalSet[getBitsetPosition(res2.x, res2.y, res2.z)]) {
			break;
		}


	}



	//else if (originalSet[getBitsetPosition(round(res.x), round(res.y), round(res.z))])
	//	useThisNormal = false;
	//useThisNormal = !useThisNormal;

	if (useThisNormal)
		norm = -norm;

	normals.push_back(norm);
	normals.push_back(norm);
	normals.push_back(norm);
	if (true || !useThisNormal) {
		indices.push_back(vertices.size() - 3);
		indices.push_back(vertices.size() - 2);
		indices.push_back(vertices.size() - 1);

	}
	else {
		//for (int i = 0; i < 3; i++) {
		//	vertices.pop_back();
		//}
		indices.push_back(vertices.size() - 1);
		indices.push_back(vertices.size() - 2);
		indices.push_back(vertices.size() - 3);
	}


}

CustomMeshInfo CustomShapeBuilder::buildShape(CustomFunction &f)
{
	RenderInfo toReturn;

	ShapeInfo info = getValidPositions(f);


	// eliminate point not on any edge

	ShapeBits newBits = info.bits;
	ShapeBits originalBits = info.bits;
	double mass = 0;
	for (unsigned int x = 0; x < sampleLength; x++) {
		for (unsigned int y = 0; y < sampleLength; y++) {
			for (unsigned int z = 0; z < sampleLength; z++) {
				unsigned int currPos = getBitsetPosition(x, y, z);
				if (info.bits[currPos])
					mass++;
			}
		}
	}


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
	mass /= pow(sampleLength, 3);
	//mass /= pow(info.scale, 3);
	mass *= 10;
	info.bits = newBits;

	std::map<unsigned int, std::set<unsigned int>> connectionsForward;
	std::map<unsigned int, std::set<unsigned int>> connectionsBackward;

	//for (int x = 0; x < sampleLength; x++) {
	//	for (int y = 0; y < sampleLength; y++) {
	//		for (int z = 0; z < sampleLength; z++) {
	//			unsigned int currPos = getBitsetPosition(x, y, z);
	//			if (info.bits[currPos]) {
	//				connectionsForward[currPos] = std::set<unsigned int>();
	//			}
	//		}
	//	}
	//}

	// cant have the same three points in a row twice
	std::map<unsigned int, std::set<unsigned int>> establishedConnections;
	//std::map<unsigned int, std::set<unsigned int>> establishedFullConnections;


	// find all edges

	for (int x = 0; x < sampleLength; x++) {
		for (int y = 0; y < sampleLength; y++) {
			for (int z = 0; z < sampleLength; z++) {
				unsigned int currPos = getBitsetPosition(x, y, z);
				if (info.bits[currPos]) {

					std::set<unsigned int> &toPlace = connectionsForward[currPos];

					//for (int x2 = std::max(x - 1, 0); x2 <= std::min(x + 1, (int)sampleLength - 1); x2+=2) {
					//	unsigned int curr2 = getBitsetPosition(x2, y, z);
					//	if (curr2 != currPos && info.bits[curr2]) {
					//		toPlace.insert(curr2);
					//	}
					//}
					//for (int y2 = std::max(y - 1, 0); y2 <= std::min(y + 1, (int)sampleLength - 1); y2+=2) {
					//	unsigned int curr2 = getBitsetPosition(x, y2, z);
					//	if (curr2 != currPos && info.bits[curr2]) {
					//		toPlace.insert(curr2);
					//	}
					//}
					//		
					//for (int z2 = std::max(z - 1, 0); z2 <= std::min(z + 1, (int)sampleLength - 1); z2+=2) {
					//	unsigned int curr2 = getBitsetPosition(x, y, z2);
					//	if (curr2 != currPos && info.bits[curr2]) {
					//		toPlace.insert(curr2);
					//	}
					//}

					for (int x2 = std::max(x - 1, 0); x2 <= std::min(x + 1, (int) sampleLength - 1); x2++) {
						for (int y2 = std::max(y - 1, 0); y2 <= std::min(y + 1, (int) sampleLength - 1); y2++) {
							for (int z2 = std::max(z -1, 0); z2 <= std::min(z + 1, (int)sampleLength - 1); z2++) {
								unsigned int curr2 = getBitsetPosition(x2, y2, z2);
									if (curr2 != currPos && info.bits[curr2]) {
										toPlace.insert(curr2);
									}
							}
						}
					}

				}
			}
		}
	}

	// every time we can return to original in three jumps we've found a triangle
	for (auto pair : connectionsForward) {
		std::set<unsigned int> jmp1 = pair.second;
		for (unsigned int i : jmp1) {
			if (i < pair.first)
			 continue;
			std::set<unsigned int> &jmp2 = connectionsForward[i];
			for (unsigned int j : jmp2) {
				if (j < pair.first)
					continue;
				std::set<unsigned int> &jmp3 = connectionsForward[j];
				if (jmp3.find(pair.first) != jmp3.end() && establishedConnections[pair.first].find(i) == establishedConnections[pair.first].end()
					&& establishedConnections[j].find(pair.first) == establishedConnections[j].end()
					&& establishedConnections[i].find(j) == establishedConnections[i].end()){
					glm::vec3 v1 = getBitAddressVector(pair.first, info.scale, info.offset);
					glm::vec3 v2 = getBitAddressVector(i, info.scale, info.offset);
					glm::vec3 v3 = getBitAddressVector(j, info.scale, info.offset);

					toReturn.vertices.push_back(v1);
					toReturn.vertices.push_back(v2);
					toReturn.vertices.push_back(v3);
					decideLatestIndicesAndNormals(toReturn.vertices, toReturn.indices, toReturn.normals, f, pair.first, originalBits);


					//establishedConnections[pair.first].insert(i);
					//establishedConnections[i].insert(j);
					//establishedConnections[j].insert(pair.first);

					//establishedConnections[i].insert(pair.first);
					//establishedConnections[j].insert(i);
					//establishedConnections[pair.first].insert(j);


				}
			}
		}
	}
	return CustomMeshInfo{ toReturn, (float) mass };

	//std::vector<glm::vec3> samples = getSamplePositions(f);
	//if (samples.size() < 3)
	//	return toReturn;
	//std::vector<quickhull::Vector3<float>> pointCloud;
	//QuickHull<float> qh;
	//for (glm::vec3 sample : samples) {
	//	pointCloud.push_back(Vector3<float>(sample.x, sample.y, sample.z));
	//}
	//auto hull = qh.getConvexHull(pointCloud, false, false);
	//auto indexBuffer = hull.getIndexBuffer();


	//auto vertexBuffer = hull.getVertexBuffer();

	//glm::vec3 center;

	//std::map<unsigned int, glm::vec3>  found;
	////sstd::set<unsigned int> found;
	//for (auto vertex : vertexBuffer) {
	//	glm::vec3 curr = glm::vec3(vertex.x, vertex.y, vertex.z);
	//	center += curr;
	//	toReturn.vertices.push_back(curr);
	//	//toReturn.normals.push_back(glm::vec3(0, 1, 0));

	//}
	//center /= vertexBuffer.size();
	//for (int i = 0; i < indexBuffer.size(); i++) {
	//	toReturn.indices.push_back(indexBuffer[i]);

	//}

	//for (int i = 0; i < toReturn.vertices.size(); i++) {
	//	glm::vec3 curr = toReturn.vertices[i] - glm::vec3(0, 0, 0);//center;
	//	curr = glm::normalize(curr);
	//	toReturn.normals.push_back(curr);
	//}
	//return toReturn;

	/*for (int i = 0; i < toReturn.indices.size() - 2; i+=3) {
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

	return toReturn;*/
}

struct VecVal {
	glm::vec3 vec;
	float val;


};

ShapeInfo getValidPositions(CustomFunction &f) {
	ShapeInfo info;
	float minFound = 100;
	float maxFound = -100;
	for (float i = -2; i <= 2; i += 0.01f) {
		if (f.eval(i, 0, 0) > 0 || f.eval(0, i, 0) > 0 || f.eval(0, 0, i) > 0) {
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
				info.bits[getBitsetPosition(x, y, z)] = f.eval(x*info.scale + offset.x, y*info.scale + offset.y, z*info.scale + offset.z) >= 0;
			}
		}
	}
	return info;
}


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
	minFound = std::min(-0.5f, minFound);
	maxFound = std::max(0.5f, maxFound);

	float scale =  maxFound - minFound;
	float incr = defScale*scale;
	for (float x = minFound; x <= minFound+scale; x += incr) {
		for (float y = minFound; y <= minFound + scale; y += incr) {
			//float encounteredZ = minFound + scale;
			for (float z = minFound; z <= minFound + scale; z += incr) {
				if (f.eval(x, y, z) > 0) {
					acceptablePoints.push_back(glm::vec3(x, y, z));
				//	encounteredZ = z;
				//	break;
				}
			}
			//for (float z = minFound + scale; z >= encounteredZ; z -= incr) {
			//	if (f.eval(x, y, z) > 0) {
			//		acceptablePoints.push_back(glm::vec3(x, y, z));
			//		break;
			//	}
			//}
		}
	}
	return acceptablePoints;
}

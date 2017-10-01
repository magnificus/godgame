#include "Cube.h"

std::vector<glm::vec3> getPlane(int fstRotateIndex, unsigned int sndRotateIndex, float finalIndexMp, float size) {
	
	std::vector<glm::vec3> points;
	int muls1[] = { -1, 1, 1, -1 };
	int muls2[] = { -1, -1, 1, 1 };
	for (int i = 0; i < 4; i++) {
		glm::vec3 point;
		point[fstRotateIndex] = size*muls1[i];
		point[sndRotateIndex] = size*muls2[i];
		point[3 - fstRotateIndex - sndRotateIndex] = finalIndexMp * size;
		points.push_back(point);
	}
	return points;

}

Cube::Cube(Shader *s) : Model(s)
{
	vertices = getVertices();
	indicies = getIndicies();
	normals = getNormals();
}

std::vector<glm::vec3> Cube::getVertices() {

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> left = getPlane(2, 1, -1, 0.5);
	std::vector<glm::vec3> right = getPlane(1, 2, 1, 0.5);
	std::vector<glm::vec3> up = getPlane(2, 0, 1, 0.5);
	std::vector<glm::vec3> down = getPlane(0, 2, -1, 0.5);
	std::vector<glm::vec3> front = getPlane(1, 0, -1, 0.5);
	std::vector<glm::vec3> back = getPlane(0, 1, 1, 0.5);


	points.insert(points.end(), left.begin(), left.end());
	points.insert(points.end(), right.begin(), right.end());
	points.insert(points.end(), up.begin(), up.end());
	points.insert(points.end(), down.begin(), down.end());
	points.insert(points.end(), front.begin(), front.end());
	points.insert(points.end(), back.begin(), back.end());
	return points;

}
std::vector<unsigned int> Cube::getIndicies() {
	std::vector<unsigned int> indices;
	for (int i = 0; i < 24; i+=4) {
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
		indices.push_back(i);
		indices.push_back(i + 2);
		indices.push_back(i + 3);
	}
	return indices;
}

void addNum(std::vector<glm::vec3> &points, glm::vec3 toAdd, int num) {
	for (int i = 0; i < num; i++)
		points.push_back(toAdd);
}

std::vector<glm::vec3> Cube::getNormals() {
	
	std::vector<glm::vec3> normals;
	addNum(normals, glm::vec3(-1, 0, 0), 4);
	addNum(normals, glm::vec3(1, 0, 0), 4);
	addNum(normals, glm::vec3(0, 1, 0), 4);
	addNum(normals, glm::vec3(0, -1, 0), 4);
	addNum(normals, glm::vec3(0, 0, -1), 4);
	addNum(normals, glm::vec3(0, 0, 1), 4);
	return normals;
}
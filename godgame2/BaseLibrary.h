#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "tinyexpr.h"
#include <iostream>
struct RenderInfo {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
};

//typedef float(*shapeFunction)(float x, float y, float z);

struct CustomFunction {

	te_expr *expr;
	int err;
	double x, y, z;
	te_variable vars[3] = { { "x", &x },{ "y", &y },{ "z", &z } };
	CustomFunction(std::string inputString) {
		expr = te_compile(inputString.c_str(), vars, 3, &err);

		if (!expr) {
			printf("\nParse error at %d\n", err);
		}
	}

	double eval(double inX, double inY, double inZ) {
		x = inX;
		y = inY;
		z = inZ;
		return te_eval(expr);
	}

};

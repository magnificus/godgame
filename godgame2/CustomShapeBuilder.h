#pragma once
#include <vector>
#include "BaseLibrary.h"
class CustomShapeBuilder {
public:
	static RenderInfo buildShape(shapeFunction * f);
	static std::vector<glm::vec3> getSamplePositions(shapeFunction* f);
};
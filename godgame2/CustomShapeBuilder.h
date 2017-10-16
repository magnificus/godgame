#pragma once
#include <vector>
#include "BaseLibrary.h"
class CustomShapeBuilder {
public:
	static RenderInfo buildShape(CustomFunction  &f);
	static std::vector<glm::vec3> getSamplePositions(CustomFunction  &f);
};
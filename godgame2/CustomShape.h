#pragma once
#include "Model.h"
#include "BaseLibrary.h"
class CustomShape : public Model {
public:
	CustomShape(Shader *s, CustomFunction &c, double time);
	float mass;
	btCollisionShape *collisionShape;
};
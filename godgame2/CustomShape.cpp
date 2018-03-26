#include "CustomShape.h"
#include "CustomShapeBuilder.h"
CustomShape::CustomShape(Shader *s, CustomFunction &f, double time) : Model(s) {
	CustomMeshInfo info = CustomShapeBuilder::buildShape(f);

	normals = info.renderInfo.normals;
	vertices = info.renderInfo.vertices;
	indicies = info.renderInfo.indices;
	mass = info.mass;
	collisionShape = info.collisionShape;
	timeCreated = time;

	color = glm::vec3(min(1.0f, mass/100.0f), min(indicies.size() / mass, 1.0f), sin(time)*0.5 + 0.5);
}
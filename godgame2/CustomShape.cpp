#include "CustomShape.h"
#include "CustomShapeBuilder.h"
CustomShape::CustomShape(Shader *s, CustomFunction &f) : Model(s) {
	RenderInfo info = CustomShapeBuilder::buildShape(f);

	normals = info.normals;
	vertices = info.vertices;
	indicies = info.indices;
}
#pragma once
#include "PhysicsHandler.h"
#include "ModelHandler.h"
#include "Sphere.h"


namespace LevelBuilder {
	static int currentLevel = 1;

	void getNextLevel(PhysicsHandler &physHandler, ModelHandler &modelHandler, Shader &s);
	Sphere* getCurrentLevelLight(PhysicsHandler &physHandler, ModelHandler &modelHandler, Shader &s);

	void getLevel1(PhysicsHandler &physHandler, ModelHandler &modelHandler, Shader &s);
	Sphere* getLevel1Light(Shader &shader1);

	void getLevel2(PhysicsHandler &physHandler, ModelHandler &modelHandler, Shader &s);
	Sphere* getLevel2Light(Shader &shader1);

	void getLevel3(PhysicsHandler &physHandler, ModelHandler &modelHandler, Shader &s);
	Sphere* getLevel3Light();
}

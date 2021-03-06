#pragma once
#include "PhysicsHandler.h"
#include "ModelHandler.h"
#include "Sphere.h"


namespace LevelBuilder {
	static int currentLevel = 1;

	void getNextLevel(PhysicsHandler &physHandler, ModelHandler &modelHandler, Shader &s);
	void getCurrentLevel(PhysicsHandler &physHandler, ModelHandler &modelHandler, Shader &s);

	Sphere* getCurrentLevelLight(Shader &s);

	void getLevel1(PhysicsHandler &physHandler, ModelHandler &modelHandler, Shader &s);
	Sphere* getLevel1Light(Shader &shader1);

	void getLevel2(PhysicsHandler &physHandler, ModelHandler &modelHandler, Shader &s);
	Sphere* getLevel2Light(Shader &shader1);

	void getLevel3(PhysicsHandler &physHandler, ModelHandler &modelHandler, Shader &s);
	Sphere* getLevel3Light(Shader &shader1);

	void getLevel4(PhysicsHandler &physHandler, ModelHandler &modelHandler, Shader &s);
	Sphere* getLevel4Light(Shader &shader1);

	void getLevel5(PhysicsHandler &physHandler, ModelHandler &modelHandler, Shader &s);
	Sphere* getLevel5Light(Shader &shader1);
}

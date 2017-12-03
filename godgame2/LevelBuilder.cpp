#include "LevelBuilder.h"
#include "Cube.h"
#include "Sphere.h"
#include "Plane.h"
#include "glm/gtc/matrix_transform.hpp"
#include "BaseLibrary.h"
#include "btBulletDynamicsCommon.h"

#define PI 3.14159

void LevelBuilder::getCurrentLevel(PhysicsHandler &physHandler, ModelHandler &modelHandler, Shader &s) {

	switch (currentLevel) {
	case 1: return getLevel1(physHandler, modelHandler, s); break;
	case 2: return getLevel2(physHandler, modelHandler, s); break;
	case 3: return getLevel3(physHandler, modelHandler, s); break;
	default: return getLevel3(physHandler, modelHandler, s); break;
		//case 3: return getLevel3(physHandler, modelHandler, s); break;
	}
}

void LevelBuilder::getNextLevel(PhysicsHandler &physHandler, ModelHandler &modelHandler, Shader &s) {
	currentLevel++;
	getCurrentLevel(physHandler, modelHandler, s);
}

Sphere* LevelBuilder::getCurrentLevelLight(Shader &s) {
	switch (currentLevel) {
	case 1: return getLevel1Light(s); break;
	case 2: return getLevel2Light(s); break;
	case 3: return getLevel3Light(s); break;

	default: return getLevel2Light(s); break;

	}
}

Sphere* LevelBuilder::getLevel2Light(Shader &shader1) {
	Sphere *light = new Sphere(&shader1);
	light->color = glm::vec3(10, 10, 10);
	light->transform *= 0.5;
	light->transform[3] = glm::vec4(0, 11, -5, 1);
	light->scale(glm::vec3(0.5, 0.5, 0.5));
	return light;
}


Sphere* LevelBuilder::getLevel3Light(Shader &shader1) {
	Sphere *light = new Sphere(&shader1);
	light->color = glm::vec3(10, 10, 10);
	light->transform *= 0.5;
	light->transform[3] = glm::vec4(-15, 15, -25, 1);
	light->scale(glm::vec3(0.5, 0.5, 0.5));
	return light;
}


void LevelBuilder::getLevel3(PhysicsHandler &physicsHandler, ModelHandler &modelHandler, Shader &shader1) {
	Cube *c2 = new Cube(&shader1);
	c2->transform[3] = glm::vec4(0, -500, 0, 1);
	c2->scale(glm::vec3(6.0f, 1000.0f, 6.0f));

	Cube *c3 = new Cube(&shader1);
	c3->transform[3] = glm::vec4(-15, -505, -25, 1);
	c3->scale(glm::vec3(1.0f, 1000.0f,1.0f));

	Cube *c4 = new Cube(&shader1);
	c4->transform[3] = glm::vec4(-10, -500, 0, 1);
	c4->scale(glm::vec3(4.0f, 1000.0f, 4.0f));

	modelHandler.addModel(c2);
	physicsHandler.addMPC(ModelPhysicsCoordinator(c2, CollisionType::custom, 0, glm::vec3(0.0f, 0.0f, 0.0)));
	modelHandler.addModel(c3);
	physicsHandler.addMPC(ModelPhysicsCoordinator(c3, CollisionType::custom, 0, glm::vec3(0.0f, 0.0f, 0.0)));
	modelHandler.addModel(c4);
	physicsHandler.addMPC(ModelPhysicsCoordinator(c4, CollisionType::custom, 0, glm::vec3(0.0f, 0.0f, 0.0)));


}


void LevelBuilder::getLevel2(PhysicsHandler &physicsHandler, ModelHandler &modelHandler, Shader &shader1) {

	glm::vec3 geometryColor = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);

	glm::vec3 planeColor = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	planeColor = glm::normalize(planeColor)*2.0f;
	planeColor = glm::vec3(1, 1, 1);
	Plane *p1 = new Plane(&shader1);
	p1->color = planeColor;
	p1->transform[3] = glm::vec4(0, -0.5, 0, 1);
	p1->scale(glm::vec3(1000, 1, 1000));



	Cube *c2 = new Cube(&shader1);
	c2->transform[3] = glm::vec4(0, 10, -15, 1);
	c2->scale(glm::vec3(5.0f, 20.0f, 5.0f));
	//c2->color = geometryColor;

	Cube *c3 = new Cube(&shader1);
	c3->transform[3] = glm::vec4(0, 14.0, -10, 1);
	c3->scale(glm::vec3(2, 1.0f, 10.0f));

	Cube *c4 = new Cube(&shader1);
	c4->transform[3] = glm::vec4(0, 10, -5, 1);
	c4->scale(glm::vec3(2.0f, 1.0f, 5.0f));

	Cube *c5 = new Cube(&shader1);
	c5->transform[3] = glm::vec4(0, 4, 2, 1);
	c5->scale(glm::vec3(2, 1.0f, 15.0f));

	Cube *c6 = new Cube(&shader1);
	c6->transform[3] = glm::vec4(-1, 10, -5, 1);
	c6->scale(glm::vec3(0.2, 12.0f, 12.0f));

	Cube *c7 = new Cube(&shader1);
	c7->transform[3] = glm::vec4(1, 10, -5, 1);
	c7->scale(glm::vec3(0.2, 12.0f, 12.0f));

	Cube *c8 = new Cube(&shader1);
	c8->transform[3] = glm::vec4(12.9, 9.2, -15, 1);
	c8->scale(glm::vec3(0.5f, 30.0f, 5.0f));

	modelHandler.addModel(p1);
	modelHandler.addModel(c2);
	modelHandler.addModel(c3);
	modelHandler.addModel(c4);
	modelHandler.addModel(c5);
	modelHandler.addModel(c6);
	modelHandler.addModel(c7);
	modelHandler.addModel(c8);



	auto mpc3 = ModelPhysicsCoordinator(c3, CollisionType::custom, 0, glm::vec3(0.0f, PI / 4, 0.0));
	auto mpc4 = ModelPhysicsCoordinator(c4, CollisionType::custom, 0, glm::vec3(0.0f, 0.0f, 0.0));
	auto mpc5 = ModelPhysicsCoordinator(c5, CollisionType::custom, 0, glm::vec3(0.0f, PI / 4, 0.0));
	auto mpc6 = ModelPhysicsCoordinator(c6, CollisionType::custom, 0, glm::vec3(0.0f,0.0 , 0.0));
	auto mpc7 = ModelPhysicsCoordinator(c7, CollisionType::custom, 0, glm::vec3(0.0f,0.0, 0.0));
	auto mpc8 = ModelPhysicsCoordinator(c8, CollisionType::custom, 0, glm::vec3(0.0, 0.0, PI / 4));

	physicsHandler.addMPC(ModelPhysicsCoordinator(p1, CollisionType::plane, 0, glm::vec3(0, 0, 0)));
	physicsHandler.addMPC(ModelPhysicsCoordinator(c2, CollisionType::custom, 0, glm::vec3(0.0f, 0.0f, 0.0)));
	physicsHandler.addMPC(mpc3);
	physicsHandler.addMPC(mpc4);
	physicsHandler.addMPC(mpc5);
	physicsHandler.addMPC(mpc6);
	physicsHandler.addMPC(mpc7);
	physicsHandler.addMPC(mpc8);

}




Sphere* LevelBuilder::getLevel1Light(Shader &shader1) {
	Sphere *light = new Sphere(&shader1);
	light->color = glm::vec3(10, 10, 10);
	light->transform *= 0.5;
	light->transform[3] = glm::vec4(-25, 15, 0, 1);
	light->scale(glm::vec3(0.5, 0.5, 0.5));
	return light;
}

void LevelBuilder::getLevel1(PhysicsHandler &physicsHandler, ModelHandler &modelHandler, Shader &shader1) {
	glm::vec3 geometryColor = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	Cube *c = new Cube(&shader1);

	c->scale(glm::vec3(10.0f, 10.0f, 70.0f));
	c->transform[3] = glm::vec4(30, 0, 0, 1);

	Cube *c2 = new Cube(&shader1);
	c2->transform[3] = glm::vec4(-25, 8, 0, 1);
	c2->scale(glm::vec3(5.0f, 1.0f, 5.0f));

	Sphere *s = new Sphere(&shader1);
	s->transform[3] = glm::vec4(-4, 5, 2, 1);
	s->color = geometryColor;
	Cube *c3 = new Cube(&shader1);
	c3->color = geometryColor;
	c3->transform[3] = glm::vec4(-2, 0, 0, 1);

	Cube *c4 = new Cube(&shader1);
	c4->color = geometryColor;
	c4->transform[3] = glm::vec4(-25, 10, 0, 1);
	c4->scale(glm::vec3(0.2, 4, 0.2));

	// surrounding planes
	float planeSize = 30;

	glm::vec3 planeColor = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	planeColor = glm::normalize(planeColor)*2.0f;
	planeColor = glm::vec3(1, 1, 1);
	Plane *p1 = new Plane(&shader1);
	p1->color = planeColor;
	p1->transform[3] = glm::vec4(0, -0.5, 0, 1);
	Plane *p2 = new Plane(&shader1);
	p2->color = planeColor;
	p2->transform[3] = glm::vec4(0, -0.5, planeSize, 1);
	Plane *p3 = new Plane(&shader1);
	p3->color = planeColor;
	p3->transform[3] = glm::vec4(0, -0.5, -planeSize, 1);
	Plane *p4 = new Plane(&shader1);
	p4->color = planeColor;
	p4->transform[3] = glm::vec4(planeSize, -0.5, 0, 1);
	Plane *p5 = new Plane(&shader1);
	p5->color = planeColor;
	p5->transform[3] = glm::vec4(-planeSize, -0.5, 0, 1);

	// add them to the handlers

	modelHandler.addModel(p1);
	modelHandler.addModel(p2);
	modelHandler.addModel(p3);
	modelHandler.addModel(p4);
	modelHandler.addModel(p5);

	modelHandler.addModel(s);

	modelHandler.addModel(c);
	modelHandler.addModel(c2);
	modelHandler.addModel(c3);
	modelHandler.addModel(c4);


	physicsHandler.addMPC(ModelPhysicsCoordinator(c, CollisionType::custom, 0, glm::vec3(0.0f,0.0f,0.0)));
	physicsHandler.addMPC(ModelPhysicsCoordinator(c2, CollisionType::custom, 0, glm::vec3(0.0f, 0.0f, 0.0)));

	physicsHandler.addMPC(ModelPhysicsCoordinator(c3, CollisionType::custom, 1));
	physicsHandler.addMPC(ModelPhysicsCoordinator(c4, CollisionType::custom, 1));
	physicsHandler.addMPC(ModelPhysicsCoordinator(s, CollisionType::sphere, 1));

	physicsHandler.addMPC(ModelPhysicsCoordinator(p1, CollisionType::plane, 0, glm::vec3(0, 0, 0)));
	physicsHandler.addMPC(ModelPhysicsCoordinator(p2, CollisionType::plane, 0, glm::vec3(0, -PI / 2, 0)));
	physicsHandler.addMPC(ModelPhysicsCoordinator(p3, CollisionType::plane, 0, glm::vec3(0, PI / 2, 0)));
	physicsHandler.addMPC(ModelPhysicsCoordinator(p4, CollisionType::plane, 0, glm::vec3(0, 0, PI / 2)));
	physicsHandler.addMPC(ModelPhysicsCoordinator(p5, CollisionType::plane, 0, glm::vec3(0, 0, -PI / 2)));
}
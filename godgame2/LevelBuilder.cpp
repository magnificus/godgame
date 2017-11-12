#include "LevelBuilder.h"
#include "Cube.h"
#include "Sphere.h"
#include "Plane.h"
#include "glm/gtc/matrix_transform.hpp"

#define PI 3.14159

void LevelBuilder::getLevel1(PhysicsHandler &physicsHandler, ModelHandler &modelHandler, Shader &shader1) {
	// large geometry

	glm::vec3 geometryColor = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	Cube *c = new Cube(&shader1);
	c->color = geometryColor;
	c->scale(glm::vec3(10.0f, 10.0f, 70.0f));
	c->transform[3] = glm::vec4(30, 0, 0, 1);


	Cube *c2 = new Cube(&shader1);
	c2->color = geometryColor;
	c2->transform[3] = glm::vec4(0, 8, 0, 1);
	c2->scale(glm::vec3(70.0f, 1.0f, 5.0f));



	Sphere *s = new Sphere(&shader1);
	s->transform[3] = glm::vec4(-4, 5, 2, 1);
	s->color = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	Cube *c3 = new Cube(&shader1);
	c3->color = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	c3->transform[3] = glm::vec4(-2, 0, 0, 1);




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


	physicsHandler.addMPC(ModelPhysicsCoordinator(c, CollisionType::custom, 0, glm::vec3(0.0f,0.0f,0.0)));
	physicsHandler.addMPC(ModelPhysicsCoordinator(c2, CollisionType::custom, 0, glm::vec3(0.0f, 0.0f, 0.0)));

	physicsHandler.addMPC(ModelPhysicsCoordinator(c3, CollisionType::custom, 1));
	physicsHandler.addMPC(ModelPhysicsCoordinator(s, CollisionType::sphere, 1));

	physicsHandler.addMPC(ModelPhysicsCoordinator(p1, CollisionType::plane, 0));
	physicsHandler.addMPC(ModelPhysicsCoordinator(p2, CollisionType::plane, 0, glm::vec3(0, -PI / 2, 0)));
	physicsHandler.addMPC(ModelPhysicsCoordinator(p3, CollisionType::plane, 0, glm::vec3(0, PI / 2, 0)));
	physicsHandler.addMPC(ModelPhysicsCoordinator(p4, CollisionType::plane, 0, glm::vec3(0, 0, PI / 2)));
	physicsHandler.addMPC(ModelPhysicsCoordinator(p5, CollisionType::plane, 0, glm::vec3(0, 0, -PI / 2)));
}
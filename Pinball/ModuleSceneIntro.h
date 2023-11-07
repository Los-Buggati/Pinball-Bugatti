#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	void CreateFlippers();
	void CreateLanzador();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;

	SDL_Texture* flipper;
	SDL_Texture* flipper2;

	PhysBody* sensor;
	bool sensed;
	int ballDiametro;

	//Spring
	PhysBody* lanzador;
	PhysBody* StaticLanzador;
	int springForce;

	// Flippers
	PhysBody* flipperLeft;
	PhysBody* flipperRight;
	PhysBody* flipperLeftPoint;
	PhysBody* flipperRightPoint;
	int flipperforce;

	// Sensors
	PhysBody* leftSideKicker;
	PhysBody* rightSideKicker;

	SDL_Texture* ball;
	SDL_Texture* background;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
	bool dir;

};

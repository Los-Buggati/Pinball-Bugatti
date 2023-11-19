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
	ModuleSceneIntro(Application* app, bool start_enabled = false);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	void CreateFlippers();
	void CreateLanzador();
	void CreateBumpers();
	void CreateSensors();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;

	SDL_Texture* flipper;
	SDL_Texture* flipper2;
	SDL_Texture* piston;

	SDL_Texture* circle;
	SDL_Texture* box;


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
	PhysBody* loseSensor;

	PhysBody* leftPad;
	PhysBody* rightPad;

	PhysBody* leftPlat;
	PhysBody* rightPlat;
	PhysBody* topPlat;
	SDL_Texture* sideKicker;

	PhysBody* bola;
	SDL_Texture* ball;
	SDL_Texture* disco;
	SDL_Texture* background;

	float rotation;
	uint bonus_fx;
	uint music;
	p2Point<int> ray;
	bool ray_on;
	bool dir;
	//bumpers
	PhysBody* bumperTop;
	PhysBody* bumperBig;
	PhysBody* bumperMid1;
	PhysBody* bumperMid2;
	PhysBody* bumperMid3;
	PhysBody* mapa;

	SDL_Texture* plat;

	iPoint moveIzq;
	iPoint moveDerch;

	int count;
	int vidas;
	int score;

	float bumperVel = 0.4f;

};

#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"

struct SDL_Texture;
class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = false);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	p2List<PhysBody*> circles;
	PhysBody* ball;
	iPoint position;
	SDL_Texture* bola;
	uint wheels;
	uint flush;
	uint platform;

	bool death;
	bool kicker;
	bool leftPlat;
	bool rightPlat;
	bool leftWall;
	bool rightWall;
	bool rueda;

	int score;
	int highScore;
	int lifes = 3;

};
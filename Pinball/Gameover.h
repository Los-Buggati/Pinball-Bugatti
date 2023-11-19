#ifndef __GAMEOVER_H__
#define __GAMEOVER_H__

#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"
#include "ModuleTextures.h"


class Gameover : public Module
{
public:
	Gameover(Application* app, bool start_enabled = false);
	~Gameover();

	bool Start();
	update_status Update();
	bool CleanUp();


public:
	SDL_Texture* gameover;
	Module* moduleToEnable = nullptr;

};

#endif


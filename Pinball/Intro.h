#ifndef __INTRO_H__
#define __INTRO_H__

#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"
#include "ModuleTextures.h"


class Intro : public Module
{
public:
	Intro(Application* app, bool start_enabled = true);
	~Intro();

	bool Start();
	update_status Update();
	bool CleanUp();
	

public:
	SDL_Texture* backgroundintro;
	Module* moduleToEnable = nullptr;

};

#endif

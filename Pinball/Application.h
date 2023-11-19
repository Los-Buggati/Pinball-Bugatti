#pragma once

#include "p2List.h"
#include "Globals.h"

class Module;

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModulePlayer;
class ModuleFadeToBlack;
class ModuleSceneIntro;
class ModulePhysics;
class Intro;
class Gameover;

class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModulePlayer* player;
	ModuleFadeToBlack* fade;
	ModulePhysics* physics;

	Intro* intro;
	ModuleSceneIntro* scene_intro;
	Gameover* game_over;
	

private:

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
};
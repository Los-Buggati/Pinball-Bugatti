#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Module.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "SString.h"

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
	
	bool vsync;
	bool unlimitFrames = false;

	uint frames;
	float dt;

	// required variables are provided:
	Timer startupTime;
	PerfTimer frameTime;
	PerfTimer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	uint32 secondsSinceStartup = 0;

	int argc;
	char** args;
	SString title;
	SString organization;

	uint32 maxFrameDuration = 16;

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
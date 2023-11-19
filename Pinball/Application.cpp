
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModulePhysics.h"
#include "Intro.h"
#include "ModuleSceneIntro.h"
#include "Gameover.h"
#include "ModuleFonts.h"


#include "Application.h"

Application::Application()
{
	Timer timer = Timer();
	startupTime = Timer();
	frameTime = PerfTimer();
	lastSecFrameTime = PerfTimer();

	frames = 0;

	renderer = new ModuleRender(this);
	window = new ModuleWindow(this);
	textures = new ModuleTextures(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	player = new ModulePlayer(this);
	intro = new Intro(this);
	scene_intro = new ModuleSceneIntro(this);
	physics = new ModulePhysics(this);
	fade = new ModuleFadeToBlack(this);
	game_over = new Gameover(this);
	fonts = new ModuleFonts(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	
	AddModule(physics);
	AddModule(fade);
	AddModule(renderer);
	AddModule(textures);
	AddModule(input);
	AddModule(audio);
	AddModule(fonts);
	

	// Scenes
	AddModule(intro);
	AddModule(scene_intro);
	AddModule(game_over);

	
	// Player
	AddModule(player);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
}


bool Application::Init()
{
	bool ret = true;
	Timer timer = Timer();
	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	
	("Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		if(item->data->IsEnabled())
			ret = item->data->Start();
		item = item->next;
	}
	
	return ret;
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if(item->data->IsEnabled())
			ret = item->data->PreUpdate();
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if(item->data->IsEnabled())
  			ret = item->data->Update();
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if(item->data->IsEnabled())
			ret = item->data->PostUpdate();
		item = item->next;
	}
	//AQUIIIIIIIIIIIIIIIII	

	if (input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		unlimitFrames = !unlimitFrames;
	}
	if (unlimitFrames)
	{
		maxFrameDuration = 32; //now is variable
	}
	else
	{
		maxFrameDuration = 16;
	}
	frameTime.Start();
	double currentDt = frameTime.ReadMs();
	if (maxFrameDuration > 0 && currentDt < maxFrameDuration) {
		uint32 delay = (uint32)(maxFrameDuration - currentDt);

		PerfTimer delayTimer = PerfTimer();
		SDL_Delay(delay);
		//LOG("We waited for %I32u ms and got back in %f ms",delay,delayTimer.ReadMs());
	}


	// L1: DONE 4: Calculate:
	// Amount of frames since startup
	frameCount++;

	// Amount of time since game start (use a low resolution timer)
	secondsSinceStartup = startupTime.ReadSec();

	// Amount of ms took the last update (dt)
	dt = (float)frameTime.ReadMs();

	// Amount of frames during the last second
	lastSecFrameCount++;

	// Average FPS for the whole game life
	if (lastSecFrameTime.ReadMs() > 1000) {
		lastSecFrameTime.Start();
		averageFps = (averageFps + lastSecFrameCount) / 2;
		framesPerSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
	}


	// Shows the time measurements in the window title
	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last FPS: %i Last dt: %.0f Highscore: %i ",
		averageFps, framesPerSecond, dt, player->highScore);

	window->SetTitle(title);

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}
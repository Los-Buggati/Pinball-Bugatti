#include "Gameover.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"

#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"

#include <sstream>
#include <string.h>

Gameover::Gameover(Application* app, bool start_enabled) : Module(app, start_enabled)
{


}

Gameover::~Gameover()
{}

// Load assets
bool Gameover::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;	
	
	App->player->Disable();
	

	gameover = App->textures->Load("Assets/gameover.png");

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Texturas

	//Audios
	/*bonus_fx = App->audio->LoadFx("Assets/bonus.wav");*/

	return ret;
}

// Load assets
bool Gameover::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status Gameover::Update()
{

	//Background Draw

	App->renderer->Blit(gameover, 0, 0);

	// FadeTo --> SceneIntro ------------------------------------------------------

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT)
	{
		App->fade->FadeToBlack(this, (Module*)App->intro, 60);
		App->player->lifes = 3;
	}


	return UPDATE_CONTINUE;
}
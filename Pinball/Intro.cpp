#include "Intro.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"

#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

#include <sstream>
#include <string.h>

Intro::Intro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	

}

Intro::~Intro()
{}

// Load assets
bool Intro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;


	background = App->textures->Load("Assets/pinball.png");

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Texturas

	//Audios
	/*bonus_fx = App->audio->LoadFx("Assets/bonus.wav");*/

	return ret;
}

// Load assets
bool Intro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status Intro::Update()
{
	
	//Background Draw

	App->renderer->Blit(background, 0, 0);

	// FadeTo --> SceneIntro ------------------------------------------------------
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN) {
		App->fade->FadeToBlack(this, (Module*)App->scene_intro, 60);
		/*App->fade->FadeToBlack(this, (Module*)App->sceneOutro, 60); */          //OUTRO --> descomentar para probar, comentar arriba.

	}

	return UPDATE_CONTINUE;
}









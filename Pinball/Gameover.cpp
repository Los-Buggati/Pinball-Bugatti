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

using namespace std;
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

	for (int i = 0; i < 10; i++)
	{
		scoreRect[i] = { (16 + 6) * i, 0, 16, 28 };
	}
	scoreFont = App->textures->Load("Assets/nums.png");


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


	string scoreString = to_string(App->player->score);
	int xPos = 350 - (scoreString.size() * 16);

	for (unsigned int i = 0; i < scoreString.size(); i++)
	{
		int digit = scoreString[i] - '0';

		App->renderer->Blit(scoreFont, xPos + (i * 18), 425, &scoreRect[digit], 0.0f);
	}

	// FadeTo --> SceneIntro ------------------------------------------------------

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		App->fade->FadeToBlack(this, (Module*)App->intro, 60);
		App->player->lifes = 3;
	}

	


	return UPDATE_CONTINUE;
}
#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleSceneIntro.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleInput.h"
#include "Intro.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	score = 0;
	combo = App->audio->LoadFx("Assets/combo.wav");
	flush = App->audio->LoadFx("Assets/flush.wav");
	platform = App->audio->LoadFx("Assets/rueda.wav");
	bola = App->textures->Load("Assets/bola.png");
	ball = App->physics->CreateCircle(390, 477, 12);
	ball->listener = this;
	circles.add(ball);
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	App->textures->Unload(bola);
	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		changeForce = !changeForce;
	}
	if (changeForce==true)
	{
		force = 300;
	}
	else
	{
		force = 100;
	}
	if (death == true)
	{
		if (lifes==0)
		{

		}
		else {
			ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(390), PIXEL_TO_METERS(477)), 0);
			lifes = lifes - 1;
			death = false;
		}
		//App->physics->world->DestroyBody(Door->body);
	}

	if (rightPlat==true)
	{
		score += 100;
		ball->body->ApplyForceToCenter(b2Vec2(-150, -force), 1);
		App->audio->PlayFx(platform);
		rightPlat = false;
		
	}
	if (leftPlat==true)
	{
		score += 100;
		ball->body->ApplyForceToCenter(b2Vec2(150, -force), 1);
		App->audio->PlayFx(platform);
		leftPlat = false;
	}
	if (kicker==true)
	{
		score += 300;
		ball->body->ApplyForceToCenter(b2Vec2(0, -200), 1);
		App->audio->PlayFx(flush);
		kicker = false;
	}
	if (rightWall==true)
	{
		score += 100;
		ball->body->ApplyForceToCenter(b2Vec2(150, 100), 1);
		App->audio->PlayFx(platform);
		rightWall = false;
	}
	if (leftWall==true)
	{
		score += 100;
		ball->body->ApplyForceToCenter(b2Vec2(-150, 100), 1);
		App->audio->PlayFx(platform);
		leftWall = false;
	}

	if (score == 2000)
	{
		App->audio->PlayFx(combo);
	}

	int x, y;
	ball->GetPosition(x, y);

	App->renderer->Blit(bola, x, y, NULL, 1.0f, ball->GetRotation());
	return UPDATE_CONTINUE;
}




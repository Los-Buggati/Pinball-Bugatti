#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleSceneIntro.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
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

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	if (death == true)
	{
		ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(390), PIXEL_TO_METERS(477)), 0);
		lifes = lifes - 1;
		death = false;
		
		//App->physics->world->DestroyBody(Door->body);
	}

	if (rightPlat==true)
	{
		score += 100;
		ball->body->ApplyForceToCenter(b2Vec2(-150, -100), 1);
		rightPlat = false;
	}
	if (leftPlat==true)
	{
		score += 100;
		ball->body->ApplyForceToCenter(b2Vec2(150, -100), 1);
		leftPlat = false;
	}
	if (kicker==true)
	{
		score += 300;
		ball->body->ApplyForceToCenter(b2Vec2(0, -200), 1);
		kicker = false;
	}
	/*if (rightWall==true)
	{
		App->scene_intro->score += 100;
		ball->body->ApplyForceToCenter(b2Vec2(150, 100), 1);
		rightWall = false;
	}
	if (leftWall==true)
	{
		App->scene_intro->score += 100;
		ball->body->ApplyForceToCenter(b2Vec2(-150, 100), 1);
		leftWall = false;
	}*/



	int x, y;
	ball->GetPosition(x, y);

	App->renderer->Blit(bola, x, y, NULL, 1.0f, ball->GetRotation());
	return UPDATE_CONTINUE;
}




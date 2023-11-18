#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleSceneIntro.h"
#include "ModuleAudio.h"


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
		death = false;
		
		//App->physics->world->DestroyBody(Door->body);
	}

	if (rightPlat==true)
	{
		App->scene_intro->score += 100;
		ball->body->ApplyForceToCenter(b2Vec2(-150, -200), 1);
		rightPlat = false;
	}
	if (leftPlat==true)
	{
		App->scene_intro->score += 100;
		ball->body->ApplyForceToCenter(b2Vec2(150, -200), 1);
		leftPlat = false;
	}
	if (kicker==true)
	{
		App->scene_intro->score += 50;
		ball->body->ApplyForceToCenter(b2Vec2(0, -300), 1);
		kicker = false;
	}


	int x, y;
	ball->GetPosition(x, y);

	App->renderer->Blit(bola, x, y, NULL, 1.0f, ball->GetRotation());
	return UPDATE_CONTINUE;
}




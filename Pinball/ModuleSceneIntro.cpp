#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include <sstream>
#include <string.h>
using namespace std;

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball = NULL;
	//circle = box = NULL;
	ray_on = false;
	sensed = false;
	dir = true;
	flipperforce = -150;
	springForce = 0;
	ballDiametro = 36;
	count = 0;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	App->player->Enable();
	LOG("Loading Intro assets");
	bool ret = true;

	
	
	for (int i = 0; i < 10; i++)
	{
		scoreRect[i] = { (16 + 6) * i, 0, 16, 28 };
	}

	App->physics->Enable();
	disco = App->textures->Load("Assets/discos.png");
	background = App->textures->Load("Assets/pinballgd.png");
	hud = App->textures->Load("Assets/hud.png");
	gasofa = App->textures->Load("Assets/gasofa.png");
	CreateLanzador();
	CreateFlippers();
	CreateBumpers();
	CreateSensors();
	
	
	scoreFont = App->textures->Load("Assets/nums.png");

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Texturas
	
	/*punto.x = 280;
	punto.y = 377;*/
	
	
	//Audios
	App->audio->PlayMusic("Assets/music.wav");
	palas = App->audio->LoadFx("Assets/metal.wav");
	
	sensor = App->physics->CreateRectangleSensor(220, 770, 100, 50);

	//Fisicas mapa
	int pinball2[114] = {
	420, 695,
	423, 210,
	410, 183,
	400, 165,
	386, 145,
	366, 123,
	349, 110,
	329, 96,
	304, 85,
	279, 80,
	247, 80,
	227, 81,
	201, 85,
	180, 94,
	161, 104,
	145, 118,
	129, 133,
	119, 146,
	111, 156,
	101, 173,
	92, 203,
	89, 230,
	88, 246,
	89, 269,
	92, 365,
	88, 385,
	76, 400,
	60, 412,
	52, 418,
	40, 440,
	38, 460,
	38, 478,
	42, 492,
	53, 505,
	69, 512,
	83, 515,
	101, 517,
	100, 682,
	142, 680,
	143, 648,
	199, 706,
	201, 768,
	200, 791,
	266, 788,
	263, 708,
	334, 635,
	332, 705,
	373, 704,
	375, 518,
	333, 462,
	369, 401,
	372, 387,
	371, 359,
	374, 326,
	388, 326,
	385, 692,
	406, 693
	};
	mapa = App->physics->CreateChain(0, 0, pinball2, 114);
	mapa->body->SetType(b2_staticBody);
	mapa->body->GetFixtureList()->SetRestitution(0.5f);


	return ret;
	
}

// Unload assets
bool ModuleSceneIntro::CleanUp()
{
	
	LOG("Unloading Intro scene");
	
	App->textures->Unload(disco);
	App->textures->Unload(background);
	App->textures->Unload(sideKicker);
	App->textures->Unload(plat);
	App->textures->Unload(piston);
	App->textures->Unload(flipper);
	App->textures->Unload(flipper2);
	App->textures->Unload(hud);
	App->textures->Unload(gasofa);

	// Unload audio
	// Destroy scene elements
	if (mapa != nullptr) {
		App->physics->world->DestroyBody(mapa->body);
		mapa = nullptr;
	}

	// Destroy sensors
	App->physics->world->DestroyBody(sensor->body);
	App->physics->world->DestroyBody(rightSideKicker->body);
	App->physics->world->DestroyBody(leftSideKicker->body);
	App->physics->world->DestroyBody(leftPlat->body);
	App->physics->world->DestroyBody(rightPlat->body);
	App->physics->world->DestroyBody(topPlat->body);
	App->physics->world->DestroyBody(loseSensor->body);
	App->physics->world->DestroyBody(leftWall->body);
	App->physics->world->DestroyBody(rightWall->body);
	App->physics->world->DestroyBody(topWall->body);

	// Destroy bumpers
	App->physics->world->DestroyBody(bumperTop->body);
	App->physics->world->DestroyBody(bumperBig->body);
	App->physics->world->DestroyBody(bumperMid2->body);
	App->physics->world->DestroyBody(bumperMid3->body);

	// Destroy launchers
	App->physics->world->DestroyBody(lanzador->body);
	App->physics->world->DestroyBody(StaticLanzador->body);

	// Destroy flippers
	App->physics->world->DestroyBody(flipperLeft->body);
	App->physics->world->DestroyBody(flipperLeftPoint->body);
	App->physics->world->DestroyBody(flipperRight->body);
	App->physics->world->DestroyBody(flipperRightPoint->body);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	LOG("%i",App->player->score);
	rotation += 0.2f; // Ajusta la velocidad de rotaci�n seg�n sea necesario
	if (rotation >= 360.0f) {
		rotation -= 360.0f;
	}
	App->renderer->Blit(disco, 31, 35,NULL, 1.0f, rotation);
	App->renderer->Blit(background, 0, 0);
	App->renderer->Blit(hud, 0, 0);
	if (App->player->lifes == 3) {
		App->renderer->Blit(gasofa, -10, 680);
		App->renderer->Blit(gasofa, 43, 680);
		App->renderer->Blit(gasofa, 90, 680);
	}
	if (App->player->lifes == 2) {
		App->renderer->Blit(gasofa, 43, 680);
		App->renderer->Blit(gasofa, 90, 680);
	}
    if (App->player->lifes == 1) {
		App->renderer->Blit(gasofa, 90, 680);
	}


	//impression
	
	App->renderer->Blit(flipper, moveIzq.x-30, moveIzq.y-50, NULL, 0, flipperLeft->GetRotation(),30,50);
	App->renderer->Blit(flipper2, moveDerch.x-75,moveDerch.y-50, NULL, 0, flipperRight->GetRotation(),75,50);

	int lanzaX, lanzaY;
	lanzador->GetPosition(lanzaX, lanzaY);
	App->renderer->Blit(piston, lanzaX, lanzaY, NULL, 0, lanzador->GetRotation());
	

	App->renderer->Blit(plat, 120,340, NULL,0,leftPlat->GetRotation());
	App->renderer->Blit(plat, 285, 340, NULL, 0, rightPlat->GetRotation());
	App->renderer->Blit(plat, 325, 275, NULL, 0, topPlat->GetRotation());

	//score
	

	string scoreString = to_string(App->player->score);
	int xPos = 200 - (scoreString.size() * 16);

	for (unsigned int i = 0; i < scoreString.size(); i++)
	{
		int digit = scoreString[i] - '0';

		App->renderer->Blit(scoreFont, xPos + (i * 18), 20, &scoreRect[digit], 0.0f);
	}

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (springForce < 450) {
			springForce += 15;
		}
		lanzador->body->ApplyForceToCenter(b2Vec2(0, springForce), 1);
	}
	
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(400, 450, 12));

		// If Box2D detects a collision with this last generated circle, it will automatically callback the function ModulePhysics::BeginContact()
		circles.getLast()->data->listener = this;
		//bola=(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 12));
		
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {

		flipperLeft->body->ApplyForceToCenter(b2Vec2(0, flipperforce), 1);

	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		flipperRight->body->ApplyForceToCenter(b2Vec2(0, flipperforce), 1);

	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
		App->audio->PlayFx(palas);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
		App->audio->PlayFx(palas);
	}
	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------

		p2List_item<PhysBody*>* c = circles.getFirst();
		while (c != NULL)
		{
			int x, y;
			c->data->GetPosition(x, y);

			App->renderer->Blit(ball, x, y, NULL, 1.0f, c->data->GetRotation());

			c = c->next;
		}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}
	
	//VIDAS
	if (App->player->lifes == 0) {
		App->fade->FadeToBlack(this, (Module*)App->game_over, 60);

	}
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::CreateSensors() {
	// --- Sensors that move the ball or something like that: ---

	// Side Kickers
	rightSideKicker = App->physics->CreateRectangleSensor(120, 680, 40, 10);
	leftSideKicker = App->physics->CreateRectangleSensor(350, 708, 40, 10);

	sideKicker = App->textures->Load("Assets/sideKicker.png");

	//sideKickerFx = App->audio->LoadFx("pinball/audio/fx/sideKick.wav");
	
	int LplatX = 150;
	int LplatY = 350;
	int RplatX = 312;
	int RplatY = 350;

	leftPlat = App->physics->CreateRectangleSensor(LplatX, LplatY, 60, 15);
	leftWall = App->physics->CreateRectangle(LplatX-2, LplatY+15, 60, 2,NULL);
	leftWall->body->SetType(b2_staticBody);

	rightPlat = App->physics->CreateRectangleSensor(RplatX, RplatY, 60, 15);
	rightWall = App->physics->CreateRectangle(RplatX, RplatY+15, 60, 2,NULL);
	rightWall->body->SetType(b2_staticBody);

	topPlat = App->physics->CreateRectangleSensor(355, 280, 60, 15);
	topWall = App->physics->CreateRectangle(355, 295, 60, 2,NULL);
	topWall->body->SetType(b2_staticBody);

	b2Vec2 posLPlat(PIXEL_TO_METERS(LplatX), PIXEL_TO_METERS(LplatY));
	b2Vec2 posTPlat(PIXEL_TO_METERS(355), PIXEL_TO_METERS(280));
	b2Vec2 posRPlat(PIXEL_TO_METERS(RplatX), PIXEL_TO_METERS(RplatY));

	b2Vec2 posLWall(PIXEL_TO_METERS(147), PIXEL_TO_METERS(360));
	b2Vec2 posTWall(PIXEL_TO_METERS(353), PIXEL_TO_METERS(293));
	b2Vec2 posRWall(PIXEL_TO_METERS(316), PIXEL_TO_METERS(360));


	leftPlat->body->SetTransform(posLPlat, 0.6f); 
	topPlat->body->SetTransform(posTPlat, 0.9f);
	rightPlat->body->SetTransform(posRPlat, -0.6f);

	leftWall->body->SetTransform(posLWall, 0.6f);
	topWall->body->SetTransform(posTWall, 0.9f);
	rightWall->body->SetTransform(posRWall, -0.6f);

	plat = App->textures->Load("Assets/barrera.png");
	


	// --- Sensors that just do what a sensor do ---

	// Losing a ball sensor
	loseSensor = App->physics->CreateRectangleSensor(223, 900, 100, 100);

}

void ModuleSceneIntro::CreateBumpers() {
	// Top bumper
	
	bumperTop= App->physics->CreateCircleSensor(250, 250, 36);
	bumperBig = App->physics->CreateCircleSensor(95, 467, 23);
	bumperMid2 = App->physics->CreateCircleSensor(287, 440, 16);
	bumperMid3 = App->physics->CreateCircleSensor(183, 440, 16);
}
void ModuleSceneIntro::CreateLanzador() 
{
	lanzador = App->physics->CreateRectangle(404, 564, 30, 20,false);
	StaticLanzador = App->physics->CreateRectangle(404, 640, 30, 20,false);
	StaticLanzador->body->SetType(b2_staticBody);
	
	

	//Joint del muelle
	b2DistanceJointDef MuelleJointDef;
	MuelleJointDef.bodyA = lanzador->body;
	MuelleJointDef.bodyB = StaticLanzador->body;
	MuelleJointDef.localAnchorA.Set(0, 0);
	MuelleJointDef.localAnchorB.Set(0, 0);
	MuelleJointDef.length = 2;
	MuelleJointDef.collideConnected = true;
	MuelleJointDef.frequencyHz = 7.0f;
	MuelleJointDef.dampingRatio = 0.5f;
	b2PrismaticJoint* MuelleJoint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&MuelleJointDef);

	piston = App->textures->Load("Assets/lanzador.png");

}

void ModuleSceneIntro::CreateFlippers() 
{
	int xL = 170;
	int yL = 600;

	int xR = 305;
	int yR = 600;

	int w = 60;
	int h = 10;

	moveDerch=iPoint(xR,yR);
	moveIzq=iPoint(xL,yL);

	// --- Left flipper ---
	flipperLeft = App->physics->CreateRectangle(xL, yL, w, h,true);
	flipperLeftPoint = App->physics->CreateCircleSensor(xL, yL, 2);

	// Flipper Joint (flipper rectangle x flipper circle to give it some movement)
	b2RevoluteJointDef flipperLeftJoint;

	flipperLeftJoint.bodyA = flipperLeft->body;
	flipperLeftJoint.bodyB = flipperLeftPoint->body;
	flipperLeftJoint.localAnchorA.Set(PIXEL_TO_METERS(-33), 0);
	flipperLeftJoint.localAnchorB.Set(0, 0);
	flipperLeftJoint.enableLimit = true;
	flipperLeftJoint.referenceAngle = 0 * DEGTORAD;
	flipperLeftJoint.lowerAngle = -30 * DEGTORAD;
	flipperLeftJoint.upperAngle = 30 * DEGTORAD;
	
	b2RevoluteJoint* joint_leftFlipper = (b2RevoluteJoint*)App->physics->world->CreateJoint(&flipperLeftJoint);

	// --- Right flipper ---
	flipperRight = App->physics->CreateRectangle(xR, yR, w, h,true);
	flipperRightPoint = App->physics->CreateCircleSensor(xR, yR, 2);
	

	// Flipper Joint
	b2RevoluteJointDef flipperRightJoint;

	flipperRightJoint.bodyA = flipperRight->body;
	flipperRightJoint.bodyB = flipperRightPoint->body;
	flipperRightJoint.localAnchorA.Set(PIXEL_TO_METERS(33), 0);
	flipperRightJoint.localAnchorB.Set(0, 0);
	flipperRightJoint.enableLimit = true;
	flipperRightJoint.referenceAngle = 0 * DEGTORAD;
	flipperRightJoint.lowerAngle = -30 * DEGTORAD;
	flipperRightJoint.upperAngle = 30 * DEGTORAD;
	
	b2RevoluteJoint* joint_rightFlipper = (b2RevoluteJoint*)App->physics->world->CreateJoint(&flipperRightJoint);

	flipper = App->textures->Load("Assets/palancaIzq.png");
	flipper2 = App->textures->Load("Assets/palancaDerch.png");

}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

}

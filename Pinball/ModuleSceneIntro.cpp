#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include <sstream>
#include <string.h>

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
	LOG("Loading Intro assets");
	bool ret = true;
	int vidas = 3;

	App->physics->Enable();

	background = App->textures->Load("Assets/pinball2.png");
	CreateLanzador();
	CreateFlippers();
	CreateBumpers();
	CreateSensors();
	

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Texturas
	
	/*punto.x = 280;
	punto.y = 377;*/
	
	
	//Audios
	App->audio->PlayMusic("Assets/music.wav");
	bonus_fx = App->audio->LoadFx("Assets/bonus.wav");
	
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

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->textures->Unload(ball);
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	LOG("%i",score);
	App->renderer->Blit(background, 0, 0);

	App->renderer->Blit(sideKicker, 26, 500);
	App->renderer->Blit(sideKicker, 388, 500);

	//flippers impression
	float32 flipperLeftAngle = flipperLeft->body->GetAngle();
	float32 flipperRightAngle = flipperRight->body->GetAngle();

	App->renderer->Blit(flipper, 140, 560, NULL, 0, flipperLeft->GetRotation());
	App->renderer->Blit(flipper2, 237, 560, NULL, 0, RADTODEG * (flipperRightAngle));
	
	//bumpers
	SDL_Rect bumpRect = bumperAnim.GetCurrentFrame();
	App->renderer->Blit(bumperTexture, bumperTopX, bumperTopY, &bumpRect);

	App->renderer->Blit(bumperTexture, bumperMidX, bumperMidY, &bumpRect);

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

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::CreateSensors() {
	// --- Sensors that move the ball or something like that: ---

	// Side Kickers
	rightSideKicker = App->physics->CreateRectangleSensor(120, 680, 40, 10);
	leftSideKicker = App->physics->CreateRectangleSensor(350, 708, 40, 10);

	sideKicker = App->textures->Load("Assets/sideKicker.png");

	sideKickerFx = App->audio->LoadFx("pinball/audio/fx/sideKick.wav");

	
	int LplatX = 170;
	int LplatY = 480;
	int RplatX = 312;
	int RplatY = 480;

	leftPlat = App->physics->CreateRectangleSensor(LplatX, LplatY, 60, 15);
	rightPlat = App->physics->CreateRectangleSensor(RplatX, RplatY, 60, 15);
	topPlat = App->physics->CreateRectangleSensor(355, 310, 50, 10);
	b2Vec2 posLPlat(PIXEL_TO_METERS(LplatX), PIXEL_TO_METERS(LplatY));
	b2Vec2 posTPlat(PIXEL_TO_METERS(355), PIXEL_TO_METERS(310));
	b2Vec2 posRPlat(PIXEL_TO_METERS(RplatX), PIXEL_TO_METERS(RplatY));


	leftPlat->body->SetTransform(posLPlat, 0.6f); 
	topPlat->body->SetTransform(posTPlat, 0.7f);
	rightPlat->body->SetTransform(posRPlat, -0.6f);

	// Assign Textures and anim
	bouncePad = App->textures->Load("Assets/bouncePad.png");
	bouncePadB = App->textures->Load("Assets/bouncePadB.png");

	int y = 0;
	for (int i = 0; i < 5; i++) {
		bounceAnim.PushBack({ 0,y,50,10 });
		bounceAnim2.PushBack({ 0,y,50,10 });
		y += 10;
	}
	bounceAnim.loop = bounceAnim2.loop = false;
	bounceAnim.speed = bounceAnim2.speed = 0.5f;

	y = 0;
	for (int i = 0; i < 4; i++) {
		bounceAnimB.PushBack({ 0,y,35,10 });
		bounceAnimB2.PushBack({ 0,y,35,10 });
		y += 10;
	}
	bounceAnimB.loop = bounceAnimB2.loop = false;
	bounceAnimB.speed = bounceAnimB2.speed = 0.5f;

	// --- Sensors that just do what a sensor do ---

	// Losing a ball sensor
	loseSensor = App->physics->CreateRectangleSensor(223, 900, 100, 100);

}

void ModuleSceneIntro::CreateBumpers() {
	// Top bumper
	bumperTopX = 97;
	bumperTopY = 470;
	bumperTop = App->physics->CreateCircleRebote(bumperTopX, bumperTopY, 20);

	// Animation and texture
	
	// Mid bumper
	bumperMidX = 235;
	bumperMidY = 470;
	bumperMid = App->physics->CreateCircleRebote(bumperMidX, bumperMidY, 20);
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

}

void ModuleSceneIntro::CreateFlippers() 
{
	int xL = 170;
	int yL = 600;

	int xR = 305;
	int yR = 600;

	int w = 60;
	int h = 10;

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
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}

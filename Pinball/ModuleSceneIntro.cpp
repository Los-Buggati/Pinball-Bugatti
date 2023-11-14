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
	//ball = NULL;
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

	App->physics->Enable();

	background = App->textures->Load("Assets/pinball.png");
	CreateLanzador();
	CreateFlippers();
	CreateBumpers();
	CreateSensors();
	flipper = App->textures->Load("Assets/negro.png");
	flipper2 = App->textures->Load("Assets/negro.png");
	
	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Texturas
	
	punto.x = 280;
	punto.y = 377;
	bola = App->physics->CreateCircle(punto.x, punto.y, 13);
	bola->listener=this;

	ball = App->textures->Load("Assets/wheel.png"); 
	
	

	//Audios
	App->audio->PlayMusic("Assets/music.wav");
	bonus_fx = App->audio->LoadFx("Assets/bonus.wav");
	
	


	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(sideKicker, 26, 500);
	App->renderer->Blit(sideKicker, 388, 500);

	float32 flipperLeftAngle = flipperLeft->body->GetAngle();
	float32 flipperRightAngle = flipperRight->body->GetAngle();

	App->renderer->Blit(flipper, 155, 695, NULL, 0, RADTODEG * (flipperLeftAngle), 0, 5);
	App->renderer->Blit(flipper2, 240, 695, NULL, 0, RADTODEG * (flipperRightAngle), 52, 5);

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
		if (springForce < 300) {
			springForce += 10;
		}
		lanzador->body->ApplyForceToCenter(b2Vec2(0, springForce), 1);
	}
	
	/*if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		circles.getLast()->data->listener = this;
	}*/

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		flipperLeft->body->ApplyForceToCenter(b2Vec2(0, flipperforce), 1);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		flipperRight->body->ApplyForceToCenter(b2Vec2(0, flipperforce), 1);
	}
	
	//Background Draw
	
	App->renderer->Blit(background, 0, 0);

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	punto = iPoint(METERS_TO_PIXELS(bola->body->GetPosition().x), METERS_TO_PIXELS(bola->body->GetPosition().y));
		App->renderer->Blit(ball, punto.x-20, punto.y-20, NULL, 1.0f,bola->GetRotation());

		p2List_item<PhysBody*>* c = circles.getFirst();
		LOG("%i", (METERS_TO_PIXELS(bola->body->GetPosition().x)));
		LOG("%i", punto.x);

		while (c != NULL)
		{
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
	rightSideKicker = App->physics->CreateRectangleSensor(120, 708, 40, 10);
	leftSideKicker = App->physics->CreateRectangleSensor(350, 708, 40, 10);

	sideKicker = App->textures->Load("pinball/sprites/sideKicker.png");

	sideKickerFx = App->audio->LoadFx("pinball/audio/fx/sideKick.wav");

	// Pads
	//leftPad = App->physics->CreateRectangleSensor(0, 0, 34, 5);
	////rightPad = App->physics->CreateRectangleSensor(0, 0, 34, 5);

	//b2Vec2 posLeftPad(300, 5.85f);	// X, Y, and Angle to aply a rotation.... Idk what i'm doing but it works  
	////rightPad->body->SetTransform(posLeftPad, -0.64f);

	//b2Vec2 posRightPad(2.65f, 5.8f);
	//leftPad->body->SetTransform(posRightPad, 0.64f);

	// Platforms just above the flippers
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
	bouncePad = App->textures->Load("pinball/sprites/bouncePad.png");
	bouncePadB = App->textures->Load("pinball/sprites/bouncePadB.png");

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
	bumperTexture = App->textures->Load("pinball/sprites/bumper.png");

	// Mid bumper
	bumperMidX = 235;
	bumperMidY = 470;
	bumperMid = App->physics->CreateCircleRebote(bumperMidX, bumperMidY, 20);
}
void ModuleSceneIntro::CreateLanzador() 
{
	lanzador = App->physics->CreateRectangle(400, 564, 30, 20);
	StaticLanzador = App->physics->CreateRectangle(400, 640, 30, 20);
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
	int yL = 620;

	int xR = 305;
	int yR = 620;

	int w = 60;
	int h = 10;

	// --- Left flipper ---
	flipperLeft = App->physics->CreateRectangle(xL, yL, w, h);
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
	flipperRight = App->physics->CreateRectangle(xR, yR, w, h);
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

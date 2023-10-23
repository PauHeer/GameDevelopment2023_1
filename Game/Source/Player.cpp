#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {
	
	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateRectangle(position.x -288, position.y + 320, 32, 60, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	//sensor = app->physics->CreateRectangleSensor(position.x - 288, position.y + 320, 32, 60, bodyType::KINEMATIC, ColliderType::PLAYER);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/coinSound.ogg");

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 vel = pbody->body->GetLinearVelocity();
	vel.x = 0;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		//
	}
	
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			vel.y = 10.0f;
	}
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vel.x = (-speed*dt);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel.x = (speed*dt);
	}
	if (Jumping != true)
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			vel.y = -9.0f;
			jumpCounter++;
			if (jumpCounter > 1)
			{
				Jumping = true;
			}
		}
	}

	//Set the velocity of the pbody of the player
	//if (Jumping != true)
	//{
		pbody->body->SetLinearVelocity(vel);
	//}
	
	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 30;

	//Update camera position
	int center = app->win->screenSurface->w/2;
	if ((center - position.x < 0) && (center - position.x > -1030)) app->render->camera.x = center - position.x;


	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		Jumping = false;
		jumpCounter = 0;
		LOG("Collision PLATFORM");
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}
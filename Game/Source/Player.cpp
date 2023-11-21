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
#include "Animation.h"


Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
	
	idleAnim.PushBack({ 0, 0, 32, 60 });
	idleAnim.PushBack({ 194, 0, 32, 60 });
	idleAnim.PushBack({ 389, 0, 32, 60 });
	idleAnim.PushBack({ 584, 0, 32, 60 });
	idleAnim.PushBack({ 778, 0, 32, 60 });
	idleAnim.loop = true;
	idleAnim.speed = 0.2f;

	rightAnim.PushBack({ 0, 61, 43, 56 });
	rightAnim.PushBack({ 187, 62, 43, 56 });
	rightAnim.PushBack({ 375, 62, 43, 56 });
	rightAnim.PushBack({ 560, 61, 43, 56 });
	rightAnim.PushBack({ 748, 60, 43, 56 });
	rightAnim.PushBack({ 933, 60, 43, 56 });
	rightAnim.loop = true;
	rightAnim.speed = 0.3f;

	leftAnim.PushBack({ 962, 246, 43, 56 });
	leftAnim.PushBack({ 772, 247, 43, 56 });
	leftAnim.PushBack({ 581, 247, 43, 56 });
	leftAnim.PushBack({ 393, 246, 43, 56 });
	leftAnim.PushBack({ 204, 244, 43, 56 });
	leftAnim.PushBack({ 16, 246, 43, 56 });
	leftAnim.loop = true;
	leftAnim.speed = 0.3f;

	upAnim.PushBack({ 0, 178, 37, 60 });
	upAnim.PushBack({ 194, 178, 37, 60 });
	upAnim.PushBack({ 389, 178, 37, 60 });
	upAnim.loop = true;
	upAnim.speed = 0.3f;

	dieAnim.PushBack({ 0, 121, 39, 54 });
	dieAnim.PushBack({ 173, 120, 39, 54 });
	dieAnim.PushBack({ 347, 132, 44, 33 });
	dieAnim.PushBack({ 516, 132, 49, 33 });
	dieAnim.PushBack({ 684, 142, 52, 30 });
	dieAnim.PushBack({ 857, 146, 52, 30 });
	dieAnim.loop = false;
	dieAnim.speed = 0.3f;
}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	start.x = parameters.attribute("x").as_int();
	start.y = parameters.attribute("y").as_int();

	return true;
}

bool Player::Start() {
	
	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateRectangle(position.x, position.y, 32, 60, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/coinSound.ogg");

	gravityScale = pbody->body->GetGravityScale();

	currentAnimation = &idleAnim;

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 vel = pbody->body->GetLinearVelocity();
	vel.x = 0;
	
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		godMode = !godMode;
	}

	if (godMode == false)
	{
		pbody->body->SetGravityScale(gravityScale);
		pbody->body->GetFixtureList()->SetSensor(false);

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			vel.y = 10.0f;
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			vel.x = (-speed * dt);

			if (currentAnimation != &leftAnim) {
				leftAnim.Reset();
				currentAnimation = &leftAnim;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			vel.x = (speed * dt);

			if (currentAnimation != &rightAnim) {
				rightAnim.Reset();
				currentAnimation = &rightAnim;
			}
		}

		if (Jumping != true)
		{
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
				vel.y = -12.0f;
				jumpCounter++;
				if (jumpCounter > 1)
				{
					Jumping = true;
				}

				if (currentAnimation != &upAnim) {
					upAnim.Reset();
					currentAnimation = &upAnim;
				}
			}
		}
	}
	
	else if (godMode == true)
	{
		pbody->body->SetGravityScale(0);
		pbody->body->GetFixtureList()->SetSensor(true);
		vel.x = 0;
		vel.y = 0;

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			vel.y = (-speed * dt);
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			vel.y = (speed * dt);
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			vel.x = (-speed * dt);
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			vel.x = (speed * dt);
		}
	}

	if ((vel.x == 0) && (vel.y == 0)) {
		if (currentAnimation != &idleAnim) {
			idleAnim.Reset();
			currentAnimation = &idleAnim;
		}
	}

	//Set the velocity of the pbody of the player
	//if (Jumping != true)
	//{
		pbody->body->SetLinearVelocity(vel);
	//}
	
	currentAnimation->Update();

	//Moves the player to the start
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		position.x = start.x;
		position.y = start.y;

		app->physics->DestroyBody(pbody);
		pbody = app->physics->CreateRectangle(position.x, position.y, 32, 60, bodyType::DYNAMIC);
		pbody->body->SetFixedRotation(true);
		pbody->listener = this;
		pbody->ctype = ColliderType::PLAYER;
		app->render->camera.x = 0;
		app->render->camera.y = 0;
	}
	//Update player position in pixels
	else {
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 30;

		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);
	}

	//Update camera position
	//Lerp-smoothing camera
	int center = app->win->screenSurface->w/2;
	float t = 0.1f; // 0 -> +smooth, 1 -> -smooth

	int targetX = center - position.x;
	int targetY = center - position.y;

	float xLerp = app->render->camera.x + t * (targetX - app->render->camera.x);
	float yLerp = app->render->camera.y + t * (targetY - app->render->camera.y);

	if ((center - position.x < 0) && (center - position.x > -2030)) app->render->camera.x = xLerp;
	if ((center - position.y < 0) && (center - position.y > -2400)) app->render->camera.y = yLerp;

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
	case ColliderType::DAMAGE:
		if (godMode == false) {
			currentAnimation = &dieAnim;
			currentAnimation->Update();
			playerDead = true;
		}
		LOG("Collision DAMAGE");
		break;
	}
}
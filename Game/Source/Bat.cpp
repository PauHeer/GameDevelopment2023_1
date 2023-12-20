#include "Bat.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Map.h"

Bat::Bat() : Entity(EntityType::BAT)
{
	name.Create("bat");

	/* Animacion para 32x32-bat-sprite-4
	idleAnim.PushBack({ 35, 5, 26, 21 });
	idleAnim.PushBack({ 66, 6, 28, 14 });
	idleAnim.PushBack({ 97, 1, 30, 20 });
	idleAnim.loop = true;
	idleAnim.speed = 0.2f;
	*/
}

Bat::~Bat() {}

bool Bat::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Bat::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	bat = app->physics->CreateRectangle(position.x, position.y, 27, 21, bodyType::DYNAMIC);
	bat->body->SetFixedRotation(true);
	bat->body->SetGravityScale(0.0f);
	bat->ctype = ColliderType::BAT;
	bat->listener = this;

	// Texture to highligh pathfinding
	pathfindingTex = app->tex->Load("Assets/Maps/tileSelection.png");

	//currentAnimation = &idleAnim;

	return true;
}

bool Bat::Update(float dt)
{
	// Do only while enemy is alive
	if (bat != nullptr) {
		// Enemy and Player position for pathfinding
		iPoint enemyPos = app->map->WorldToMap(position.x, position.y);
		Player* player = app->scene->GetPlayer();
		iPoint playerPos = app->map->WorldToMap(player->position.x, player->position.y);

		if (IsInRange(playerPos, enemyPos, 10)) {

			app->map->pathfinding->CreatePath(enemyPos, playerPos);

			// Get the latest calculated path
			const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
			iPoint nextPoint = app->map->MapToWorld(path->At(0)->x, path->At(0)->y);
			MoveTowards(nextPoint, dt);

			// Draws the pathfinding
			if (app->physics->debug) {
				for (uint i = 0; i < path->Count(); ++i)
				{
					iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
					app->render->DrawTexture(pathfindingTex, pos.x, pos.y);
				}
			}
		}
	}

	// Destroy enemy collider
	if (enemyKilled == true) {
		app->physics->DestroyBody(bat);
		bat = nullptr;
	}

	if (bat != nullptr) {
		// Add a physics to an item - update the position of the object from the physics.  
		position.x = METERS_TO_PIXELS(bat->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(bat->body->GetTransform().p.y) - 16;

		app->render->DrawTexture(texture, position.x, position.y);

	}

	return true;
}

bool Bat::CleanUp()
{
	return true;
}

void Bat::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::ATTACK:
		enemyKilled = true;

		LOG("Collision ATTACK");
		break;
	}
}

bool Bat::IsInRange(iPoint playerPos, iPoint enemyPos, int range) {
	iPoint distance;

	distance.x = abs(enemyPos.x - playerPos.x);
	distance.y = abs(playerPos.y - enemyPos.y);
	if (distance.x < range && distance.y < range) return true;

	else return false;
}

void Bat::MoveTowards(const iPoint& destination, float dt) {/*
	iPoint direction;
	direction.x = destination.x - position.x;
	direction.y = destination.y - position.y;

	// Normalize direction vector
	float length = sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length > 0) {
		direction.x /= length;
		direction.y /= length;
	}

	// Adjust speed
	float speed = 0.05f;
	position.x += direction.x * speed * dt;
	position.y += direction.y * speed * dt;

	bat->body->SetTransform({ PIXEL_TO_METERS(position.x + 16), PIXEL_TO_METERS(position.y + 16) }, 0.0f);*/
}
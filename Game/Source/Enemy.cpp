#include "Enemy.h"
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

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("enemy");
}

Enemy::~Enemy() {}

bool Enemy::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Enemy::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	ebody = app->physics->CreateRectangle(position.x, position.y, 32, 60, bodyType::DYNAMIC);
	ebody->body->SetFixedRotation(true);
	ebody->ctype = ColliderType::ENEMY;
	ebody->listener = this;

	// Texture to highligh pathfinding
	pathfindingTex = app->tex->Load("Assets/Maps/tileSelection.png");

	return true;
}

bool Enemy::Update(float dt)
{
	if (ebody != nullptr) {
		// Add a physics to an item - update the position of the object from the physics.  
		position.x = METERS_TO_PIXELS(ebody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(ebody->body->GetTransform().p.y) - 16;

		app->render->DrawTexture(texture, position.x, position.y);

	}

	// Do only while enemy is alive
	if (ebody != nullptr) {
		// Enemy and Player position for pathfinding
		iPoint enemyPos = app->map->WorldToMap(position.x, position.y);
		Player* player = app->scene->GetPlayer();
		iPoint playerPos = app->map->WorldToMap(player->position.x, player->position.y);
		
		app->map->pathfinding->CreatePath(enemyPos, playerPos);

		// Get the latest calculated path and draw
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(pathfindingTex, pos.x, pos.y);
		}
	}

	// Destroy enemy collider
	if (enemyKilled == true) {
		app->physics->DestroyBody(ebody);
		ebody = nullptr;
	}

	return true;
}

bool Enemy::CleanUp()
{
	return true;
}
void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::ATTACK:
		enemyKilled = true;

		LOG("Collision ATTACK");
		break;
	}
}
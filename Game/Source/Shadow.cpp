#include "Shadow.h"
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

Shadow::Shadow() : Entity(EntityType::SHADOW)
{
	name.Create("shadow");
}

Shadow::~Shadow() {}

bool Shadow::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Shadow::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	shadow = app->physics->CreateRectangle(position.x, position.y, 32, 60, bodyType::DYNAMIC);
	shadow->body->SetFixedRotation(true);
	shadow->ctype = ColliderType::SHADOW;
	shadow->listener = this;

	// Texture to highligh pathfinding
	pathfindingTex = app->tex->Load("Assets/Maps/tileSelection.png");

	return true;
}

bool Shadow::Update(float dt)
{
	if (shadow != nullptr) {
		// Add a physics to an item - update the position of the object from the physics.  
		position.x = METERS_TO_PIXELS(shadow->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(shadow->body->GetTransform().p.y) - 16;

		app->render->DrawTexture(texture, position.x, position.y);

	}

	// Do only while enemy is alive
	if (shadow != nullptr) {
		// Enemy and Player position for pathfinding
		iPoint enemyPos = app->map->WorldToMap(position.x, position.y);
		Player* player = app->scene->GetPlayer();
		iPoint playerPos = app->map->WorldToMap(player->position.x, player->position.y);
		
		if (IsInRange(playerPos, enemyPos)) {

			app->map->pathfinding->CreatePath(enemyPos, playerPos);

			// Get the latest calculated path and draw
			const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				app->render->DrawTexture(pathfindingTex, pos.x, pos.y);
				//vel.x = pos.x;
				//vel.y = pos.y;
				//bat->body->SetLinearVelocity(vel);
			}
		}
	}

	// Destroy enemy collider
	if (enemyKilled == true) {
		app->physics->DestroyBody(shadow);
		shadow = nullptr;
	}

	return true;
}

bool Shadow::CleanUp()
{
	return true;
}
void Shadow::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::ATTACK:
		enemyKilled = true;

		LOG("Collision ATTACK");
		break;
	}
}

bool Shadow::IsInRange(iPoint playerPos, iPoint enemyPos) {
	iPoint range;

	range.x = enemyPos.x - playerPos.x;
	range.y = playerPos.y - enemyPos.y;
	if (range.x < 10 && range.y < 10) return true;

	else return false;
}
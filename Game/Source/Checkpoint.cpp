#include "Checkpoint.h"
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

Checkpoint::Checkpoint() : Entity(EntityType::CHECKPOINT)
{
	name.Create("checkpoint");
}

Checkpoint::~Checkpoint() {}

bool Checkpoint::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	return true;
}

bool Checkpoint::Start() {

	// Create collider
	checkp = app->physics->CreateRectangleSensor(position.x, position.y, 64, 32, bodyType::STATIC);
	checkp->ctype = ColliderType::CHECKPOINT;
	checkp->listener = this;

	return true;
}

bool Checkpoint::Update(float dt)
{
	return true;
}

bool Checkpoint::CleanUp()
{
	return true;
}
void Checkpoint::OnCollision(PhysBody* physA, PhysBody* physB) {

}
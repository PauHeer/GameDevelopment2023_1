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
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Checkpoint::Start() {

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
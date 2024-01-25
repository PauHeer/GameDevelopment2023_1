#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	for (pugi::xml_node itemNode = config.child("shadow"); itemNode; itemNode = itemNode.next_sibling("shadow"))
	{
		shadow = (Shadow*)app->entityManager->CreateEntity(EntityType::SHADOW);
		shadow->parameters = itemNode;
	}

	for (pugi::xml_node itemNode = config.child("bat"); itemNode; itemNode = itemNode.next_sibling("bat"))
	{
		bat = (Bat*)app->entityManager->CreateEntity(EntityType::BAT);
		bat->parameters = itemNode;
	}
	
	for (pugi::xml_node itemNode = config.child("checkpoint"); itemNode; itemNode = itemNode.next_sibling("checkpoint"))
	{
		checkpoint = (Checkpoint*)app->entityManager->CreateEntity(EntityType::CHECKPOINT);
		checkpoint->parameters = itemNode;
	}

	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	//img = app->tex->Load("Assets/Textures/test.png");
	
	//Music is commented so that you can add your own music
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	app->map->Load();

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	// L15: DONE 2: Instantiate a new GuiControlButton in the Scene

	SDL_Rect btPos = { windowW / 2 - 60, windowH / 2 - 10, 120,20 };
	gcButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this);

	app->audio->PlayMusic("Assets/Audio/Music/Goonies.ogg", 1.0f);

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	float camSpeed = 1;

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += (int)ceil(camSpeed * dt);


	// Renders the image in the center of the screen 
	//app->render->DrawTexture(img, (int)textPosX, (int)textPosY);

	// Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN /*|| player->playerDead == true*/)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::LoadState(pugi::xml_node node) 
{
	player->position.x = node.child("player").attribute("x").as_int();
	player->position.y = node.child("player").attribute("y").as_int();
	player->texturePath = node.child("player").attribute("texturepath").as_string();
	iPoint camera;
	camera.x = app->render->camera.x;
	camera.y = app->render->camera.y;
	player->PlayerToSavePoint(player->position, camera);

	bat->position.x = node.child("bat").attribute("x").as_int();
	bat->position.y = node.child("bat").attribute("y").as_int();
	bat->texturePath = node.child("player").attribute("texturepath").as_string();

	shadow->position.x = node.child("shadow").attribute("x").as_int();
	shadow->position.y = node.child("shadow").attribute("y").as_int();
	shadow->texturePath = node.child("player").attribute("texturepath").as_string();

	return true;
}

bool Scene::SaveState(pugi::xml_node node) 
{
	pugi::xml_node playerNode = node.append_child("player");
	playerNode.append_attribute("x").set_value(player->position.x);
	playerNode.append_attribute("y").set_value(player->position.y);
	playerNode.append_attribute("texturepath").set_value(player->texturePath);

	pugi::xml_node batNode = node.append_child("bat");
	batNode.append_attribute("x").set_value(bat->position.x);
	batNode.append_attribute("y").set_value(bat->position.y);
	batNode.append_attribute("texturepath").set_value(bat->texturePath);

	pugi::xml_node shadowNode = node.append_child("shadow");
	shadowNode.append_attribute("x").set_value(shadow->position.x);
	shadowNode.append_attribute("y").set_value(shadow->position.y);
	shadowNode.append_attribute("texturepath").set_value(shadow->texturePath);

	return true;
}

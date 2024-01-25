#include "GuiPause.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"

GuiPause::GuiPause(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
}

GuiPause::~GuiPause()
{

}

bool GuiPause::Update(float dt)
{

	return false;
}
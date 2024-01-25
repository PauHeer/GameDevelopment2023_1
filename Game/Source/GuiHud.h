#ifndef __GUIHUD_H__
#define __GUIHUD_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiHud : public GuiControl
{

public:

	GuiHud(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiHud();

	// Called each loop iteration
	bool Update(float dt);

private:

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUIHUD_H__
#ifndef __GUIPAUSE_H__
#define __GUIPAUSE_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiPause : public GuiControl
{

public:

	GuiPause(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiPause();

	// Called each loop iteration
	bool Update(float dt);

private:

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUIPAUSE_H__
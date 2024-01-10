#ifndef __BAT_H__
#define __BAT_H__

#include "Animation.h"
#include "Entity.h"
#include "Point.h"
#include "Timer.h"
#include "SDL/include/SDL.h"

//#define MOVE_DELAY_SECONDS = 1.0f;	// Dealay duration for movement

struct SDL_Texture;

class Bat : public Entity
{
public:

	Bat();
	virtual ~Bat();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool IsInRange(iPoint playerPos, iPoint enemyPos, int range);

	void MoveTowards(const iPoint& destination, float dt);

public:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* bat;
	SDL_Texture* pathfindingTex = nullptr;
	bool enemyKilled = false;

	Animation* currentAnimation = NULL;

	Animation idleAnim;

private:

	Timer moveTimer;							// Timer for controlling movement delay
	float MOVE_DELAY_SECONDS = 1000.0f;
};

#endif // __BAT_H__
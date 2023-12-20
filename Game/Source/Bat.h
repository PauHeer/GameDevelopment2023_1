#ifndef __BAT_H__
#define __BAT_H__

#include "Animation.h"
#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

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

	bool IsInRange(iPoint playerPos, iPoint enemyPos);

public:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* bat;
	SDL_Texture* pathfindingTex = nullptr;
	bool enemyKilled = false;

	Animation* currentAnimation = NULL;

	Animation idleAnim;
};

#endif // __BAT_H__
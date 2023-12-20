#ifndef __SHADOW_H__
#define __SHADOW_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Shadow : public Entity
{
public:

	Shadow();
	virtual ~Shadow();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool IsInRange(iPoint playerPos, iPoint enemyPos);

public:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* shadow;
	SDL_Texture* pathfindingTex = nullptr;
	bool enemyKilled = false;
};

#endif // __SHADOW_H__
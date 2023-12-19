#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Enemy : public Entity
{
public:

	Enemy();
	virtual ~Enemy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

private:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* ebody;
	SDL_Texture* pathfindingTex = nullptr;
	bool enemyKilled = false;
};

#endif // __ENEMY_H__
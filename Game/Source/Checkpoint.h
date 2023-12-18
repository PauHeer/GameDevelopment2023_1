#ifndef __CHECKPOINT_H__
#define __CHECKPOINT_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Checkpoint : public Entity
{
public:

	Checkpoint();
	virtual ~Checkpoint();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* checkp;
	SDL_Texture* pathfindingTex = nullptr;
	bool enemyKilled = false;
};

#endif // __CHECKPOINT_H__
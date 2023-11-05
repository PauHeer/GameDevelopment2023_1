#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Animation.h"
#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	bool Jumping = false;

	int jumpCounter = 0;

	bool playerDead = false;

	bool godMode = false;

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	float speed = 0.35f;
	const char* texturePath;
	SDL_Texture* texture = NULL;

	PhysBody* pbody;
	PhysBody* sensor;

	int pickCoinFxId;

	int gravityScale;

	Animation* currentAnimation = NULL;

	Animation idleAnim;
	Animation upAnim;
	Animation downAnim;
	Animation leftAnim;
	Animation rightAnim;
	Animation dieAnim;
};

#endif // __PLAYER_H__
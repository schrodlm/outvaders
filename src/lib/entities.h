#pragma once

#include <string>
#include "leetlib.h"
#include <map>

const std::string Path = "../gfx/";

class SpriteManager
{
public:
	SpriteManager() {}

	void LoadSpriteImpl(const char* path)
	{
		if (sprites.count(path) == 0)
		{
			void* sprite = LoadSprite(path);
			sprites[path] = sprite;
		}
	}

	void* GetSprite(const std::string& path)
	{
		return sprites[path];
	}

private:
	std::map<std::string, void*> sprites;
};



class Entity
{
public:

	virtual ~Entity() {};

	float BX = 0, BY = 0, BA = 0;

	void* getSprite() { return this->sprite; }
	RECT getBoundingBox() { return this->boundingBox; }
	int getState() { return state; }
	uint64_t getYSize() { return this->ySize; }
	uint64_t getXSize() { return this->xSize; }

	void setBoundingBox(long x, long y) { boundingBox = { x,y, x + this->xSize, y - this->ySize }; }
	void setState(int _state) { state = _state; }

	void updateBoundingBox() { boundingBox = { (int)BX,(int)BY, (int)BX + xSize, (int)BY - ySize }; }


protected:

	Entity(int xSize, int ySize, const char* _spritePath);


	RECT boundingBox;
	long xSize;
	long ySize;
	int state = 1;
	const char* spritePath;
	void* sprite;
};

class Enemy : public Entity
{
public:
	virtual  ~Enemy() {};

	Enemy() : Entity(30, 30, "gfx/Little Invader.png") {};

};


class Bullet : public Entity
{
public:
	virtual ~Bullet() {};

	Bullet() : Entity(30, 30, "gfx/Bullet.png") {};
	Bullet(const char* spritePath) : Entity(30, 30, spritePath) {};
};

class EnemyBullet : public Bullet
{
public:
	virtual ~EnemyBullet() {};

	EnemyBullet() : Bullet("gfx/zlet.png")
	{};

};

class Player : public Entity
{
public:
	virtual ~Player() {};

	Player() : Entity(60, 60, "gfx/Big Invader.png") {}


	int lives = 3;
};




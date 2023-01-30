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

	//void operator=(const Entity&) = delete;
	virtual ~Entity() {};

	float BX = 0, BY = 0, BA = 0;

	//	void* getSprite() { return this->sprite; }
	void setBoundingBox(RECT _boundingBox) { boundingBox = _boundingBox; }
	RECT getBoundingBox() { return this->boundingBox; }
	void setBoundingBox(long x, long y) { boundingBox = { x,y, x + this->xSize, y - this->ySize }; }
	void updateBoundingBox() { boundingBox = { (int)BX,(int)BY, (int)BX + xSize, (int)BY - ySize }; }
	void setState(int _state) { state = _state; }
	int getState() { return state; }
	uint64_t getXSize() { return this->xSize; }
	uint64_t getYSize() { return this->ySize; }

	const char* spritePath;
	void* sprite;


protected:
	Entity(int xSize, int ySize, const char* _spritePath, SpriteManager& manager)
	{
		this->xSize = xSize;
		this->ySize = ySize;


		this->spritePath = _spritePath;
		//saving and loading sprite of a specific entity
		manager.LoadSpriteImpl(spritePath);
		sprite = manager.GetSprite(spritePath);
	}

	RECT boundingBox;
	long xSize;
	long ySize;
	int state = 1;
};

class Enemy : public Entity
{
public:
	virtual  ~Enemy() {};

	Enemy(SpriteManager& manager) : Entity(30, 30, "gfx/Little Invader.png", manager) {};

};


class Bullet : public Entity
{
public:
	virtual ~Bullet() {};

	Bullet(SpriteManager& manager) : Entity(30, 30, "gfx/Bullet.png", manager) {};
	Bullet(SpriteManager& manager, const char* spritePath) : Entity(30, 30, spritePath, manager) {};
};

class EnemyBullet : public Bullet
{
public:
	virtual ~EnemyBullet() {};

	EnemyBullet(SpriteManager& manager) : Bullet(manager, "gfx/zlet.png")
	{};

};

class Player : public Entity
{
public:
	virtual ~Player() {};

	Player(SpriteManager& manager) : Entity(60, 60, "gfx/Big Invader.png", manager) {}


	int lives = 3;
};




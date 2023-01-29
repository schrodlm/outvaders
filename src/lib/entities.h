#pragma once

#include <string>
#include "leetlib.h"

const std::string Path = "../gfx/";

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


protected:
	Entity(int xSize, int ySize) { boundingBox = { 0,0,0,0 }; this->xSize = xSize; this->ySize = ySize; }

	RECT boundingBox;
	long xSize;
	long ySize;
	int state = 1;
};

class Enemy : public Entity
{
public:
	virtual  ~Enemy() {};

	Enemy() : Entity(30, 30) {};
	void* sprite = LoadSprite("../gfx/Little Invader.png");

};


class Bullet : public Entity
{
public:
	virtual ~Bullet() {};

	Bullet() : Entity(20, 10) {};
	void* sprite = LoadSprite("../gfx/Bullet.png");
};


class Player : public Entity
{
public:
	virtual ~Player() {};

	Player() : Entity(60, 60) {};
	void* sprite = LoadSprite("../gfx/Big Invader.png");
	int lives = 3;
};

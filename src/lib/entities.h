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
	void setBoundingBox(long x, long y) { boundingBox = { x,y, x + this->xSize, y + this->ySize }; }
	void setState(int _state) { state = _state; }
	int getState() { return state; }
	uint64_t getXSize() { return this->xSize; }
	uint64_t getYSize() { return this->ySize; }


protected:
	Entity(int xSize, int ySize) { boundingBox = { 0,0,0,0 }; this->xSize = xSize; this->ySize = ySize; }

	RECT boundingBox;
	long xSize = 20;
	long ySize = 20;
	int state = 1;
};

class Enemy : public Entity
{
public:
	virtual  ~Enemy() {};

	Enemy() : Entity(20, 20) {};
	void* sprite = LoadSprite("../gfx/Little Invader.png");

};


class Bullet : public Entity
{
public:
	virtual ~Bullet() {};

	Bullet() : Entity(10, 10) {};
	void* sprite = LoadSprite("../gfx/Bullet.png");
};



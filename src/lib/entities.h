#pragma once

#include <string>
#include "leetlib.h"

const std::string Path = "../gfx/";

class Entity
{
public:

	void operator=(const Entity&) = delete;
	virtual ~Entity() {};

	float BX = 0, BY = 0, BA = 0;

	//	void* getSprite() { return this->sprite; }
	void setBoundingBox(RECT _boundingBox) { boundingBox = _boundingBox; }
	RECT getBoundingBox() { return this->boundingBox; }


protected:
	Entity() { boundingBox = { 0,0,0,0 }; }

	RECT boundingBox;

};

class Enemy : public Entity
{
public:
	virtual  ~Enemy() {};

	Enemy() : Entity() {};
	void* sprite = LoadSprite("../gfx/Little Invader.png");
};



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

	void setBoundingBox(long x, long y) { boundingBox = { x - (xSize / 2) ,y + (ySize / 2), x + (xSize / 2), y - (ySize / 2) }; }
	void setState(int _state) { state = _state; }

	void updateBoundingBox() { boundingBox = { (int)BX - (xSize / 2) ,(int)BY + (ySize / 2), (int)BX + (xSize / 2), (int)BY - (ySize / 2) }; }


protected:

	Entity(int xSize, int ySize, const char* _spritePath);
	Entity(int xSize, int ySize) : xSize(xSize), ySize(ySize) {};

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
	int score = 0;
	void* sprite_1;
	void* sprite_2;
	void* sprite_death;

	//countdown is used after enemy is killed, to draw enemy vector for specified number of frames;
	int dead_countdown = 10;
	int dead = false;

	virtual  ~Enemy() {};

	Enemy(const char* spritePath1, const char* spritePath2, const char* spritePathDeath, int score);
	Enemy() : Entity(30, 30, "gfx/enemy1_1.png") {};

};

class EnemyFront : public Enemy
{
public:
	virtual ~EnemyFront() {};

	EnemyFront() : Enemy("gfx/enemy1_1.png", "gfx/enemy1_2.png", "gfx/enemy1_death.png", 50) {};

	//int score = 50;
};


class EnemyMiddle : public Enemy
{
public:
	virtual ~EnemyMiddle() {};

	EnemyMiddle() : Enemy("gfx/enemy2_1.png", "gfx/enemy2_2.png", "gfx/enemy1_death.png", 60) {};

	//int score = 60;
};

class EnemyBack : public Enemy
{
public:
	virtual ~EnemyBack() {};

	EnemyBack() : Enemy("gfx/enemy3_1.png", "gfx/enemy3_2.png", "gfx/enemy1_death.png", 70) {};
};


class Bullet : public Entity
{
public:
	virtual ~Bullet() {};

	Bullet() : Entity(5, 5, "gfx/Bullet.png") {};
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

	Player() : Entity(60, 60, "gfx/Big Invader.png") { this->BX = 400; this->BY = 550; }

	int getScore() { return score; }
	void updateScore(int toAdd) { score += toAdd; }
	int getLives() { return lives; }
	void setLives(int new_lives) { lives = new_lives; }

protected:
	int score = 0;
	int lives = 3;

};




#pragma once

#include <string>
#include "leetlib.h"
#include <map>

const std::string Path = "../gfx/";

void deleteManager();

class SpriteManager
{
public:
	SpriteManager() = default;
	~SpriteManager();
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

	virtual ~Entity() = default;

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

	Entity(int BX, int BY, int BA, int xSize, int ySize, const char* _spritePath);
	Entity(int xSize, int ySize) : xSize(xSize), ySize(ySize) { updateBoundingBox(); };

	RECT boundingBox;
	long xSize;
	long ySize;
	int state = 1;
	void* sprite = nullptr;
};

class Enemy : public Entity
{
public:
	int score = 0;
	void* sprite_1 = nullptr;
	void* sprite_2 = nullptr;
	void* sprite_death = nullptr;

	//countdown is used after enemy is killed, to draw enemy vector for specified number of frames;
	int dead_countdown = 10;
	int dead = false;

	virtual  ~Enemy() = default;

	Enemy(int xSize, int ySize, const char* spritePath1, const char* spritePath2, const char* spritePathDeath, int score);
	Enemy(int _BX, int _BY, int _BA) : Entity(_BX, _BY, _BA, 30, 30, "gfx/enemy1_1.png") {};

};

class EnemyFront : public Enemy
{
public:
	virtual ~EnemyFront() {};

	EnemyFront() : Enemy(25, 25, "gfx/enemy1_1.png", "gfx/enemy1_2.png", "gfx/enemy1_death.png", 50) {};
};


class EnemyMiddle : public Enemy
{
public:
	virtual ~EnemyMiddle() = default;

	EnemyMiddle() : Enemy(30, 30, "gfx/enemy2_1.png", "gfx/enemy2_2.png", "gfx/enemy2_death.png", 60) {};
};

class EnemyBack : public Enemy
{
public:
	virtual ~EnemyBack() = default;

	EnemyBack() : Enemy(30, 30, "gfx/enemy3_1.png", "gfx/enemy3_2.png", "gfx/enemy3_death.png", 70) {};
};

class EnemyRare : public Enemy
{
public:
	virtual ~EnemyRare() = default;

	EnemyRare() : Enemy(60, 60, "gfx/rare_enemy.png", "", "gfx/enemy3_death.png", 500) {};
};


class Bullet : public Entity
{
public:
	virtual ~Bullet() = default;

	Bullet(int _BX, int _BY, int _BA) : Entity(_BX, _BY, _BA, 10, 20, "gfx/Bullet.png") {};
	Bullet(int _BX, int _BY, int _BA, const char* spritePath) : Entity(_BX, _BY, _BA, 30, 30, spritePath) {};
};

class EnemyBullet : public Bullet
{
public:
	~EnemyBullet() = default;

	EnemyBullet(int _BX, int _BY, int _BA) : Bullet(_BX, _BY, _BA, "gfx/enemy_bullet1.png")
	{};

};

class Player : public Entity
{
public:
	virtual ~Player() = default;

	Player(int _BX, int _BY, int _BA) : Entity(_BX, _BY, _BA, 60, 60, "gfx/player.png") {}

	int getScore() { return score; }
	void updateScore(int toAdd) { score += toAdd; }
	int getLives() { return lives; }
	void setLives(int new_lives) { lives = new_lives; }
	int getShotsFired() { return shots_fired; }
	void updateShotsFired() { shots_fired++; }
	int getHit() { return hit; }
	void setHit(int cooldown = 20) { hit = cooldown; }
	void updateHit() { hit--; }



protected:
	int score = 0;
	int lives = 3;
	int shots_fired = 1;
	int hit = 0;

};




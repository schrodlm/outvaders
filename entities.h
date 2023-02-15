/*****************************************************************//**
 * \file   entities.h
 * \brief  Defines entities used in the game or by the game.
 *
 * \author schrodlm
 * \date   February 2023
 *********************************************************************/
#pragma once

#include <string>
#include "leetlib.h"
#include <map>

const std::string Path = "../gfx/";

void clearSpriteManager();

/**
 * Manages loading a retrieval of games sprites.
 */
class SpriteManager
{
public:
	SpriteManager() = default;
	~SpriteManager();

	/**
	 * Loads a sprite with the given path and adds it to the internal sprite map.
	 *
	 * \param path relative path to the sprite
	 */
	void LoadSpriteImpl(const char* path);

	/**
	 * Retrieves a sprite with the given path from the internal sprite map.
	 *
	 * \param path relative path to the sprite
	 * \return pointer to the sprite
	 */
	void* GetSprite(const std::string& path) { return sprites[path]; }

private:
	std::map<std::string, void*> sprites;
};


/**
 * Abstract class that is used by all entities in the game.
 */
class Entity
{
public:

	virtual ~Entity() = default;

	//Getters and setters

	void* getSprite() { return this->sprite; }

	uint64_t getYSize() { return this->ySize; }
	uint64_t getXSize() { return this->xSize; }

	RECT getBoundingBox() { return this->boundingBox; }
	void setBoundingBox(long x, long y) { boundingBox = { x - (xSize / 2) ,y + (ySize / 2), x + (xSize / 2), y - (ySize / 2) }; }
	void updateBoundingBox() { boundingBox = { (int)BX - (xSize / 2) ,(int)BY + (ySize / 2), (int)BX + (xSize / 2), (int)BY - (ySize / 2) }; }

	int getState() { return state; }
	void setState(int _state) { state = _state; }

	float getBX() { return BX; }
	float getBY() { return BY; }
	float getBA() { return BA; }
	float setBX(float _BX) { BX = _BX; return BX; }
	float setBY(float _BY) { BY = _BY; return BY; }
	float setBA(float _BA) { BA = _BA; return BA; }
	float updateBX(float toAdd) { BX += toAdd; return BX; }
	float updateBY(float toAdd) { BY += toAdd; return BY; }
	float updateBA(float toAdd) { BA += toAdd; return BA; }




protected:

	Entity(int BX, int BY, int BA, int xSize, int ySize, const char* _spritePath);
	Entity(int xSize, int ySize) : xSize(xSize), ySize(ySize) { updateBoundingBox(); };

	float BX = 0, BY = 0, BA = 0;
	RECT boundingBox; // <! hitbox
	long xSize; // !< width of the entity
	long ySize; // !< height of the entity
	int state = 1; // !< current state - 0 means it is no longer needed in the game and is ready to be deleted
	void* sprite = nullptr;
};

/**
 *  Abstract class for all enemies.
 */
class Enemy : public Entity
{
public:
	virtual  ~Enemy() = default;


	//getters and setters
	int getScore() { return score; }
	void* getStillSprite() { return still_sprite; }
	void* getMovingSprite() { return moving_sprite; }
	void* getDeathSprite() { return death_sprite; }
	int getDeathCountdown() { return dead_countdown; }
	int getDead() { return dead; }


	void updateCountdown(int _death_countdown = -1) { dead_countdown += _death_countdown; }
	void setDead(int _dead = true) { dead = _dead; }

protected:
	Enemy(int xSize, int ySize, const char* spritePath1, const char* spritePath2, const char* spritePathDeath, int score);
	int score = 0; // !< how many points is enemy worth
	void* still_sprite = nullptr;
	void* moving_sprite = nullptr;
	void* death_sprite = nullptr;


	int dead_countdown = 10; // !< countdown is used after enemy is killed, to draw enemy vector for specified number of frames
	int dead = false;

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

	EnemyRare() : Enemy(65, 50, "gfx/rare_enemy.png", "", "gfx/enemy3_death.png", 500) {};
};


class Bullet : public Entity
{
public:
	virtual ~Bullet() = default;

	Bullet(int _BX, int _BY, int _BA) : Entity(_BX, _BY, _BA, 10, 20, "gfx/bullet.png") {};
	Bullet(int _BX, int _BY, int _BA, const char* spritePath) : Entity(_BX, _BY, _BA, 10, 20, spritePath) {};
};

class EnemyBullet : public Bullet
{
public:
	~EnemyBullet() = default;

	EnemyBullet(int _BX, int _BY, int _BA) : Bullet(_BX, _BY, _BA, "gfx/enemy_bullet.png")
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




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

	Entity(float BX, float BY, float BA, int xSize, int ySize, const char* _spritePath);
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

/**
 * Enemy located at the front of the pack. Worth the least out of pack enemies.
 */
class EnemyFront : public Enemy
{
public:
	virtual ~EnemyFront() {};

	EnemyFront() : Enemy(25, 25, "gfx/enemy1_1.png", "gfx/enemy1_2.png", "gfx/enemy1_death.png", 50) {};
};

/**
 * Enemy located in the middle of the pack.
 */
class EnemyMiddle : public Enemy
{
public:
	virtual ~EnemyMiddle() = default;

	EnemyMiddle() : Enemy(30, 30, "gfx/enemy2_1.png", "gfx/enemy2_2.png", "gfx/enemy2_death.png", 60) {};
};

/**
 * Enemy located at the back of the pack. Worth the most out of pack enemies.
 */
class EnemyBack : public Enemy
{
public:
	virtual ~EnemyBack() = default;

	EnemyBack() : Enemy(30, 30, "gfx/enemy3_1.png", "gfx/enemy3_2.png", "gfx/enemy3_death.png", 70) {};
};

/**
 * Special enemy that spawns every 23rd shot of the the player. Cannot hurt the player. Worth the most points out of all enemies.
 */
class EnemyRare : public Enemy
{
public:
	virtual ~EnemyRare() = default;

	EnemyRare() : Enemy(65, 50, "gfx/rare_enemy.png", "", "gfx/enemy3_death.png", 500) {};
};

/**
 * Bullet class, this class is used as a player bullet and also a template for enemy bullets.
 */
class Bullet : public Entity
{
public:
	virtual ~Bullet() = default;

	Bullet(float _BX, float _BY, float _BA) : Entity(_BX, _BY, _BA, 10, 20, "gfx/bullet.png") {};
	Bullet(float _BX, float _BY, float _BA, const char* spritePath) : Entity(_BX, _BY, _BA, 10, 20, spritePath) {};
};

/**
 * Bullets of the pack enemies. Deals one damage to the player.
 */
class EnemyBullet : public Bullet
{
public:
	~EnemyBullet() = default;

	EnemyBullet(float _BX, float _BY, float _BA) : Bullet(_BX, _BY, _BA, "gfx/enemy_bullet.png") {};

};

/**
 * Class representing player and player information during the game.
 */
class Player : public Entity
{
public:
	virtual ~Player() = default;

	Player(float _BX, float _BY, float _BA) : Entity(_BX, _BY, _BA, 60, 60, "gfx/player.png") {}

	//getters and setters

	int getScore() { return score; }
	void updateScore(int toAdd) { score += toAdd; }
	int getLifes() { return lifes; }
	void updateLifes(int new_lives = -1) { lifes += new_lives; }
	int getShotsFired() { return shots_fired; }
	void updateShotsFired() { shots_fired++; }
	int getHit() { return hit_cooldown; }
	void setHitCooldown(int cooldown = 20) { hit_cooldown = cooldown; }
	void updateHitCooldown() { hit_cooldown--; }



protected:
	int score = 0;
	int lifes = 3;
	int shots_fired = 1;
	int hit_cooldown = 0; // !< after player gets hit, he cannot be hit for time specified by this variable

};




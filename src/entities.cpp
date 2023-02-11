#include "lib/entities.h"

SpriteManager manager;


Entity::Entity(int xSize, int ySize, const char* _spritePath)
{
	this->xSize = xSize;
	this->ySize = ySize;


	this->spritePath = _spritePath;
	//saving and loading sprite of a specific entity
	manager.LoadSpriteImpl(spritePath);
	sprite = manager.GetSprite(spritePath);
}

Enemy::Enemy(const char* spritePath1, const char* spritePath2, const char* spritePathDeath, int score) : Entity(30, 30)
{

	this->score = score;

	manager.LoadSpriteImpl(spritePath1);
	sprite_1 = manager.GetSprite(spritePath1);

	manager.LoadSpriteImpl(spritePath2);
	sprite_2 = manager.GetSprite(spritePath2);

	manager.LoadSpriteImpl(spritePathDeath);
	sprite_death = manager.GetSprite(spritePathDeath);



}
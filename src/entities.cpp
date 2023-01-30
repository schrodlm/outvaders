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
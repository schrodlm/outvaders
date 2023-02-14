#include "lib/entities.h"

SpriteManager* manager = new SpriteManager();

void deleteManager()
{
	delete manager;
	manager = new SpriteManager();
	return;
}

SpriteManager::~SpriteManager()
{
	for (auto& [path, sprite] : sprites)
	{
		if (sprite != nullptr)
			FreeSprite(sprite);
	}
	sprites.clear();
}

Entity::Entity(int BX, int BY, int BA, int xSize, int ySize, const char* _spritePath)
{
	this->xSize = xSize;
	this->ySize = ySize;

	this->BX = BX;
	this->BY = BY;
	this->BA = BA;

	updateBoundingBox();
	//saving and loading sprite of a specific entity
	manager->LoadSpriteImpl(_spritePath);
	sprite = manager->GetSprite(_spritePath);
}

Enemy::Enemy(int xSize, int ySize, const char* spritePath1, const char* spritePath2, const char* spritePathDeath, int score) : Entity(xSize, ySize)
{

	this->score = score;

	manager->LoadSpriteImpl(spritePath1);
	sprite_1 = manager->GetSprite(spritePath1);

	//it doesnt have to be exist for some enemies
	if (spritePath2 != "")
	{
		manager->LoadSpriteImpl(spritePath2);
		sprite_2 = manager->GetSprite(spritePath2);
	}
	manager->LoadSpriteImpl(spritePathDeath);
	sprite_death = manager->GetSprite(spritePathDeath);



}
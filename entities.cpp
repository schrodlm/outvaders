#include "entities.h"

// Manager
//============================================================


SpriteManager* manager = new SpriteManager();

/**
 * Free current internal sprite manager data.
 *
 */
void clearSpriteManager()
{
	delete manager;
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

void SpriteManager::LoadSpriteImpl(const char* path)
{
	if (sprites.count(path) == 0)
	{
		void* sprite = LoadSprite(path);
		sprites[path] = sprite;
	}
}

// Sprite path utilities
//============================================================


const std::string sprite_directory = "gfx/";

/**
 * Utility function to a path of a sprite.
 *
 * \param sprite_name sprite name in the sprites directory
 * \return a full path to the sprite
 */
std::string spritePath(std::string sprite_name) { return sprite_directory + sprite_name; }

// Entity
//============================================================


Entity::Entity(float BX, float BY, float BA, int xSize, int ySize, const char* _spritePath)
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

// Enemy
//============================================================

Enemy::Enemy(int xSize, int ySize, const char* spritePath1, const char* spritePath2, const char* spritePathDeath, int score) : Entity(xSize, ySize)
{

	this->score = score;

	manager->LoadSpriteImpl(spritePath1);
	still_sprite = manager->GetSprite(spritePath1);

	//it doesnt have to be exist for some enemies
	if (spritePath2 != "")
	{
		manager->LoadSpriteImpl(spritePath2);
		moving_sprite = manager->GetSprite(spritePath2);
	}
	manager->LoadSpriteImpl(spritePathDeath);
	death_sprite = manager->GetSprite(spritePathDeath);



}
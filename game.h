#pragma once

#include "leetlib.h"
#include "entities.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

class Game
{
public:

	/**
	* Initialize player and width a height of the screen
	*
	*/
	Game(int _width, int _height);

	/**
	 * Deletes heap memory usage of that class.
	 *
	 */
	~Game();

	/**
	* Responsible for running the game's main loop, updating the game state, and drawing the graphics, checking the collisions.
	*
	*/
	void gameLoop();

	/**

	This function handles the game over loop, displaying the game over screen,
	the player's score, and a menu to either play again, view high scores, or quit the game.

	\return Returns an integer value that represents the selected menu option.

	1 - play again, 2 - view high scores, 3 - quit the game.
	*/
	int gameOverLoop();

	/**
	* Show highscore on the screen and creates a menu to exit that screen
	*
	*/
	int highscoreLoop();


	void pauseLoop();

	/**
	* Clear entities used in a game loop.
	*
	*/
	void clearLevel();

	/**
	* Initialize entities used in a game loop.
	*
	*/
	void initializeLevel();

	/**
	* Screen with information about the level that pops up before said level.
	*
	*/
	void levelIntro();

	/**
	* Creates a new player.
	*
	*/
	void initializeNewPlayer();


	/**
	* Check collision of two entities.
	*
	* \return true if etities collide
	*/
	bool checkCollision(Entity& obj1, Entity& obj2);

private:
	Player* player; // !< represent player information

	int elapsed_time = 0;
	int width = 0; // !< screen width
	int height = 0;// !< screen height
	void* background = LoadSprite("background.bmp");
	std::vector<std::vector<Enemy>> enemies;
	std::vector<Bullet> player_bullets;
	std::vector<EnemyBullet> enemy_bullets;
	EnemyRare* rare_enemy = nullptr;

	int level = 0;

	// !< array of sprite images representing 26 letters of english alphabet
	void* Text[26] =
	{
		LoadSprite("gfx/alet.png"),
		LoadSprite("gfx/blet.png"),
		LoadSprite("gfx/clet.png"),
		LoadSprite("gfx/dlet.png"),
		LoadSprite("gfx/elet.png"),
		LoadSprite("gfx/flet.png"),
		LoadSprite("gfx/glet.png"),
		LoadSprite("gfx/hlet.png"),
		LoadSprite("gfx/ilet.png"),
		LoadSprite("gfx/jlet.png"),
		LoadSprite("gfx/klet.png"),
		LoadSprite("gfx/llet.png"),
		LoadSprite("gfx/mlet.png"),
		LoadSprite("gfx/nlet.png"),
		LoadSprite("gfx/olet.png"),
		LoadSprite("gfx/plet.png"),
		LoadSprite("gfx/qlet.png"),
		LoadSprite("gfx/rlet.png"),
		LoadSprite("gfx/slet.png"),
		LoadSprite("gfx/tlet.png"),
		LoadSprite("gfx/ulet.png"),
		LoadSprite("gfx/vlet.png"),
		LoadSprite("gfx/wlet.png"),
		LoadSprite("gfx/xlet.png"),
		LoadSprite("gfx/ylet.png"),
		LoadSprite("gfx/zlet.png"),
	};


};
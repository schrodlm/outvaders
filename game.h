#pragma once

#include "src/lib/leetlib.h"
#include "src/lib/entities.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

class Game
{
public:
	//play
	Game();
	~Game() {
		FreeSprite(background);
		deleteManager();
		delete player;
		clearLevel();

	};


	void gameLoop();
	int gameOverLoop();
	void highscoreLoop();
	void clearLevel();
	void initializeLevel();
	void levelIntro();
	void initializeNewPlayer();
	//	void readHighscore();
	//	void menu();

	bool checkCollision(Entity& obj1, Entity& obj2);

private:
	Player* player;

	int elapsed_time = 0;
	int width = 0;
	int height = 0;
	void* background = LoadSprite("../bitmap1.bmp");
	std::vector<std::vector<Enemy>> enemies;
	std::vector<Bullet> bullets;
	std::vector<EnemyBullet> enemyBullets;
	EnemyRare* rare_enemy = nullptr;

	bool direction = true;
	int level = 0;

};
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
	Game()
	{

		//enemy vector
		std::vector<Enemy> column;
		for (int i = 0; i < 2; i++)
			column.push_back(EnemyFront());
		for (int i = 0; i < 2; i++)
			column.push_back(EnemyMiddle());
		column.push_back(EnemyBack());
		enemies.resize(11, column);


		for (int i = 0; i < enemies.size(); i++)
		{
			for (int j = 0; j < enemies[0].size(); j++)
			{
				enemies[i][j].BX = (i % 11) * 50 + 120;
				enemies[i][j].BY = j * 60 + 70;
				enemies[i][j].updateBoundingBox();
			}
		}
		this->height = 600;
		this->width = 800;
	}

	void gameLoop();
	void gameOverLoop();
	//void menu();

	bool checkCollision(Entity& obj1, Entity& obj2);

private:
	int elapsed_time = 0;
	int width = 0;
	int height = 0;
	void* background = LoadSprite("../bitmap1.bmp");
	std::vector<std::vector<Enemy>> enemies;
	std::vector<Bullet> bullets;
	std::vector<EnemyBullet> enemyBullets;
	Player player;

	bool direction = true;

};
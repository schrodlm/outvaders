#include "game.h"
#include "menu.h"
#include <fstream>
#include <iostream>



//not necessery to be a game object -> it can be used also by other objects (menu etc.) 
// -> would make mroe sense if i later placed it in some utility_funcions.h or something
std::vector<std::string> readHighscore()
{
	//if file doesnt exist this will create it
	//could later create createFile() function for it to be more readable
	std::ofstream existCheck("highscores.txt", std::ios::out | std::ios::app);
	existCheck.close();


	std::ifstream file("highscores.txt");

	std::vector<std::string> highscores;
	if (file.good()) {
		//HighScore score;
		std::string score;
		while (file >> score)
			highscores.push_back(score);
	}

	file.close();

	return highscores;
}


void showHighscore()
{
	Menu menu;
	menu.AddItem("Back", [] {return 1; });

	//check if highscore was set
	std::vector<std::string> highscores = readHighscore();
	int back = -1;
	while (1)
	{
		startFlip();

		if (WantQuit()) return;
		if (IsKeyDown(VK_ESCAPE)) return;
		//DrawSprite(background, 400, 300, 800, 600, 0, 0xffffffff);
		for (int i = 0; i < highscores.size(); i++)
		{
			DrawText(800 / 2, 200 + i * 40, 45, 0xffffffff, true, highscores[i].c_str());

		}

		menu.Draw(800 / 2, 500, 40);
		back = menu.HandleInput();

		Flip();
		if (back == 1) return;

	}
}

int updateHighscore(int score)
{
	// -1 means it is not a highscore
	int new_highscore_place = -1;
	std::vector<std::string> highscores = readHighscore();

	for (int i = 0; i < highscores.size(); i++)
	{
		if (std::stoi(highscores[i]) < score)
		{
			new_highscore_place = i;
			break;
		}
	}
	//new highscore was set so we need to set the new one and push all lower highscores one to the right in the vector
	if (new_highscore_place != -1)
	{
		std::string prev = std::to_string(score);
		for (int i = new_highscore_place; i < highscores.size(); i++)
		{
			std::swap(prev, highscores[i]);
		}
	}

	std::ofstream file("highscores.txt");

	for (std::string highscore : highscores) {
		file << highscore << std::endl;
	}

	file.close();

	return new_highscore_place;
}


Game::Game()
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

	player = new Player(400, 550, 0);
}

int Game::gameLoop()
{

	void* Text[] =
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
	//randomness
	std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> distribution(0, 100);

	bool spriteAnim = false;

	EnemyRare* rare_enemy = nullptr;

end:
	startFlip();

	++elapsed_time;
	if (WantQuit()) return -1;
	if (IsKeyDown(VK_ESCAPE)) return -1;

	DrawSprite(background, 400, 300, 800, 600, 0, 0xffffffff);


	int most_right = INT_MIN;
	int most_left = INT_MAX;
	//drawing enemies on the screen
	for (auto& col : enemies)
	{
		for (auto& enemy : col)
		{
			if (enemy.dead) enemy.dead_countdown--;
			DrawSprite(enemy.dead ? enemy.sprite_death : spriteAnim ? enemy.sprite_1 : enemy.sprite_2, enemy.BX, enemy.BY, enemy.getXSize(), enemy.getYSize(), 0, 0xffffffff);
			if (elapsed_time % 20 == 0)
			{
				(direction) ? enemy.BX += 10 : enemy.BX -= 10;
			}

			enemy.updateBoundingBox();

			//creating enemy bullets -> max 3 on the screen at any time
			if (enemyBullets.size() < 3)
			{
				//shot in one movement with probability (not yet calculated)
				int random = distribution(rng);
				if (&enemy == &col.back() && random == 1)
				{
					enemyBullets.emplace_back(enemy.BX, enemy.BY, 0);
				}
			}
		}
	}
	//changing moving animation on enemies
	if (elapsed_time % 20 == 0)
	{
		spriteAnim = !spriteAnim;
	}

	most_right = enemies.back()[0].BX;
	most_left = enemies.front()[0].BX;

	//setting direction for enemies based on position of most left and most right enemy
	if (most_right + 10 >= 800 && direction)
	{
		direction = false;
		for (auto& col : enemies) for (auto& enemy : col) enemy.BY += 20, enemy.updateBoundingBox();
	}
	if (most_left - 10 <= 0 && !direction)
	{
		direction = true;
		for (auto& col : enemies)for (auto& enemy : col) enemy.BY += 20, enemy.updateBoundingBox();

	}

	//player_angle makes the player stride to the side that he is moving in
	int player_angle = 0;
	if (IsKeyDown(VK_RIGHT)) player_angle = -1;
	else if (IsKeyDown(VK_LEFT)) player_angle = 1;
	DrawSprite(player->getSprite(), player->BX += IsKeyDown(VK_LEFT) ? -5 : IsKeyDown(VK_RIGHT) ? 5 : 0, player->BY, player->getXSize(), player->getYSize(), player_angle * 0.1, 0xffffffff);
	player->updateBoundingBox();


	// FIRE
	static int playerFireCooldown = 0;
	if (playerFireCooldown) --playerFireCooldown;
	//if (!IsKeyDown(VK_SPACE)) count = 0;
	if (IsKeyDown(VK_SPACE) && playerFireCooldown == 0) { playerFireCooldown = 40; player->updateShotsFired(), bullets.emplace_back(player->BX, player->BY - player->getYSize() / 2, 0); }


	//spawn rare enemy after player shot 23 shots
	if (player->getShotsFired() % 5 == 0 && rare_enemy == nullptr)
	{
		rare_enemy = new EnemyRare();
		rare_enemy->BX = -100;
		rare_enemy->BY = 100;
		rare_enemy->updateBoundingBox();
	}
	//if rare enemy exists, move it and check if it is out of bounds
	if (rare_enemy)
	{
		//enemy is dead, we draw the death animation
		if (rare_enemy->dead) rare_enemy->dead_countdown--, DrawSprite(rare_enemy->sprite_death, rare_enemy->BX, rare_enemy->BY, rare_enemy->getXSize(), rare_enemy->getYSize(), 0, 0xffffffff);
		//enemy is alive, we draw it
		else DrawSprite(rare_enemy->sprite_1, rare_enemy->BX += 4, rare_enemy->BY, rare_enemy->getXSize(), rare_enemy->getYSize(), 0, 0xffffffff);
		rare_enemy->updateBoundingBox();
		if (rare_enemy->dead_countdown <= 0 || rare_enemy->BX - (rare_enemy->getXSize() / 2) > 800) delete rare_enemy, rare_enemy = nullptr;
	}


	//drawing bullet sprites -> we also add angle to them so they rotate?
	for (int n = 0; n < bullets.size(); ++n)
	{
		DrawSprite(bullets[n].getSprite(), bullets[n].BX, bullets[n].BY -= 4, bullets[n].getXSize(), bullets[n].getYSize(), 0, 0xffffffff);

		bullets[n].updateBoundingBox();
	}

	//drawing enemy bullet sprites
	for (int n = 0; n < enemyBullets.size(); ++n)
	{
		DrawSprite(enemyBullets[n].getSprite(), enemyBullets[n].BX, enemyBullets[n].BY += 4, enemyBullets[n].getXSize(), enemyBullets[n].getYSize(), 0, 0xffffffff);

		enemyBullets[n].updateBoundingBox();
		//if bullet is out of map -> delete it
		if (enemyBullets[n].getBoundingBox().bottom > 640) enemyBullets[n].setState(0);
	}


	//Collision checking -> enemy bullets : player
	for (auto& enemyBullet : enemyBullets)
	{
		if (checkCollision(enemyBullet, *player))
		{
			enemyBullet.setState(0);

			if (player->getLives() > 0) player->setLives(player->getLives() - 1);



		}
	}
	if (player->getLives() == 0)
	{
		Flip();
		delete rare_enemy;

		return gameOverLoop();

	}


	//Collision checking -> player bullets : enemies
	for (auto& bullet : bullets)
	{
		//bullet is out of bounds
		if (bullet.getBoundingBox().top < 0)
		{
			bullet.setState(0);
			continue;
		}
		//colision check for normal enemies and players bullets
		for (auto& col : enemies)
		{
			for (auto& enemy : col)
			{
				if (checkCollision(bullet, enemy))
				{
					enemy.dead = true;
					bullet.setState(0);
					//adding score to the player
					player->updateScore(enemy.score);
					//DrawSprite(enemy.sprite_death, enemy.BX, enemy.BY, enemy.getXSize(), enemy.getYSize(), 0, 0xffffffff);
				}
			}
		}
		//Collision check for rare_enemy and players bullets
		if (rare_enemy && checkCollision(bullet, *rare_enemy))
		{
			rare_enemy->dead = true;
			bullet.setState(0);
			player->updateScore(rare_enemy->score);
		}
	}

	//check if enemies are below their lowest y -> if they are lower the game is over
	for (auto& col : enemies)
	{
		if (col[0].getBoundingBox().bottom > 600)
		{
			Flip();
			delete player;
			delete rare_enemy;

			return gameOverLoop();
		}
	}



	//removing bullets that hit enemies or are out of bounds
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return (b.getState() == 0); }), bullets.end());
	enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(), [](EnemyBullet& e) { return (e.getState() == 0); }), enemyBullets.end());

	for (auto& col : enemies) col.erase(std::remove_if(col.begin(), col.end(), [](Enemy& e) { return (e.dead_countdown <= 0); }), col.end());
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](std::vector<Enemy>& e) { return e.empty(); }), enemies.end());


	//check if player won
	if (enemies.empty())
	{
		//player advances
		return 2;
	}


	//draws space invaders letters sprite on the screen 
	// - hardcoded loop 
	// - also for some reason angle change is dependent on the index of the sprite, so first letter will not be rotating and last will be rotating much more than others
	//for (int n = 0; n < strlen("space invaders"); ++n) if (n != 5)DrawSprite(Text[n], n * 40 + 150, 30, 20, 20, sin(elapsed_time * 0.1) * n * 0.01);

	//DrawTextFromSprites("hello world", 40, 40, Text);

	//Color format: 0xQQRRGGBB 
	//  Q: opacity 
	//  R: red
	//  G: green
	//  B: blue

	//Draw player score

	DrawText(width - 100, 30, 40, 0xffffffff, true, ("SCORE:" + std::to_string(player->getScore())).c_str());
	DrawText(width - 100, 55, 40, 0xffffffff, true, ("LIFES:" + std::to_string(player->getLives())).c_str());



	//rendering provided by the lib
	Flip();

	goto end;
}

//-----------------------------------------------------------------------------
// Name: gameOverLoop()
// Desc: Shows up when player lost all gis lifes
// 
// 
//-----------------------------------------------------------------------------
int Game::gameOverLoop()
{
	//check if highscore was set
	int new_highscore_place = updateHighscore(player->getScore());




	Menu menu;
	menu.AddItem("Play Again", [] {return 1; });
	menu.AddItem("Highscores", [] {return 2; });
	menu.AddItem("Quit", [] {return 3; });

	int menu_option = -1;

	while (1)
	{
		elapsed_time++;
		startFlip();

		if (WantQuit()) return -1;
		if (IsKeyDown(VK_ESCAPE)) return -1;
		DrawSprite(background, 400, 300, 800, 600, 0, 0xffffffff);

		//new highscore set
		if (new_highscore_place != -1)
			DrawText(width / 2, 100, 45, elapsed_time % 60 <= 30 ? 0xff5d8aa8 : 0xffffffff, true, ("! NEW HIGHSCORE FOR PLACE " + std::to_string(new_highscore_place + 1) + " SET !").c_str());

		DrawText(width / 2, 200, 45, 0xffffffff, true, "GAME OVER");
		DrawText(width / 2, height / 2, 40, 0xffffffff, true, ("SCORE:" + std::to_string(player->getScore())).c_str());
		menu.Draw(width / 2, 400, 40);
		menu_option = menu.HandleInput();
		Flip();


		switch (menu_option)
		{
		case -1:
			break;
		case 1:
			//1 means players want to play a new game 
			return 1;
			break;
		case 2:
			highscoreLoop();
			break;
		case 3:
			//2 means player want to quit
			return 2;
			break;
		}

	}
}



//checks collision between two entities
bool Game::checkCollision(Entity& obj1, Entity& obj2)
{
	RECT bb1 = obj1.getBoundingBox();
	RECT bb2 = obj2.getBoundingBox();

	return (bb1.left <= bb2.right &&
		bb1.right >= bb2.left &&
		bb1.top >= bb2.bottom &&
		bb1.bottom <= bb2.top);
}

void Game::highscoreLoop()
{
	showHighscore();
}
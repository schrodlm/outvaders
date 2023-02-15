/*****************************************************************//**
 * \file   game.cpp
 * \brief  Implements method for game class and utility function for managing highscores of player.
 *
 * \author schrodlm
 * \date   February 2023
 *********************************************************************/



#include "game.h"
#include "menu.h"
#include <fstream>
#include <iostream>


 /**
  * Read highscores from a file "highscore.txt", if that file doesnt exist it will be created
  *
  * \return highscores in a std::vector<int>
  */
std::vector<std::string> readHighscore()
{


	std::ifstream test("highscores.txt");
	if (test.fail() || test.peek() == std::ifstream::traits_type::eof()) {  // check if the file is empty
		std::ofstream outfile("highscores.txt");
		for (int i = 0; i < 5; i++) outfile << "0" << std::endl;
		outfile.close();
	}

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




/**
 * Checks if parameter score is a new highscore (top 5) and if it is it will set it and write it into highscore text file.
 *
 * \param score score to check
 * \return place in the highscores that score was set as, if it will return -1 that means score was not a highscore
 */
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


Game::Game(int _width, int _height)
{


	height = _height;
	width = _width;

	player = new Player(400, 550, 0);
}

Game::~Game()
{
	FreeSprite(background);
	clearSpriteManager();
	delete player;
	clearLevel();
}


void Game::gameLoop()
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

	//level loop
start:
	initializeLevel();
	bool spriteAnim = false;
	const int enemy_speed = 10 + level * 5;
	int most_right_enemy = 0;
	int most_left_enemy = 0;
	bool enemy_direction = true;
	int player_angle = 0; // !< makes the player stride to the side that he is moving in

	levelIntro();

	//game loop
end:


	++elapsed_time;
	if (!StartFlip()) return;
	if (IsKeyDown(VK_ESCAPE)) return;

	DrawSprite(background, 400, 300, 800, 600, 0, WHITE);


	//Managing enemies
	//=============================================================
	for (auto& col : enemies)
	{
		for (auto& enemy : col)
		{
			if (enemy.getDead()) enemy.updateCountdown();
			DrawSprite(enemy.getDead() ? enemy.getDeathSprite() : spriteAnim ? enemy.getStillSprite() : enemy.getMovingSprite(), enemy.getBX(), enemy.getBY(), enemy.getXSize(), enemy.getYSize(), 0, WHITE);
			// Moving enemies after 20 frames, to smooth movement otherwise
			if (elapsed_time % 20 == 0)
			{
				(enemy_direction) ? enemy.updateBX(enemy_speed) : enemy.updateBX(-enemy_speed);
			}

			enemy.updateBoundingBox();

			//creating enemy bullets -> max 3 on the screen at any time
			if (enemy_bullets.size() < 3)
			{
				//shot in one movement with probability (not yet calculated)
				int random = distribution(rng);
				if (&enemy == &col.back() && random == 1)
				{
					enemy_bullets.emplace_back(enemy.getBX(), enemy.getBY(), 0);
				}
			}
		}
	}
	//changing moving animation on enemies
	if (elapsed_time % 20 == 0)
	{
		spriteAnim = !spriteAnim;
	}

	most_right_enemy = enemies.back()[0].getBX();
	most_left_enemy = enemies.front()[0].getBX();

	//setting direction for enemies based on position of most left and most right enemy
	if (most_right_enemy + 10 >= 800 && enemy_direction)
	{
		enemy_direction = false;
		for (auto& col : enemies) for (auto& enemy : col) enemy.updateBY(20), enemy.updateBoundingBox();
	}
	if (most_left_enemy - 10 <= 0 && !enemy_direction)
	{
		enemy_direction = true;
		for (auto& col : enemies)for (auto& enemy : col) enemy.updateBY(20), enemy.updateBoundingBox();

	}


	//Player management
	//=============================================================

	player_angle = 0;
	if (IsKeyDown(VK_RIGHT)) player_angle = -1;
	else if (IsKeyDown(VK_LEFT)) player_angle = 1;

	if (player->getHit() > 0)
	{
		DrawSprite(player->getSprite(), player->setBX(IsKeyDown(VK_LEFT) ? max(player->getBX() - 5, 0) : IsKeyDown(VK_RIGHT) ? min(player->getBX() + 5, width) : player->getBX()), player->getBY(), player->getXSize(), player->getYSize(), player_angle * 0.1, RED);
		player->updateHitCooldown();
	}
	else
		DrawSprite(player->getSprite(), player->setBX(IsKeyDown(VK_LEFT) ? max(player->getBX() - 5, 0) : IsKeyDown(VK_RIGHT) ? min(player->getBX() + 5, width) : player->getBX()), player->getBY(), player->getXSize(), player->getYSize(), player_angle * 0.1, WHITE);

	player->updateBoundingBox();



	//Player bullet management
	//=============================================================

	static int playerFireCooldown = 0; // !< get set after player shoots a bullet, player cannot shoot while this > 0
	if (playerFireCooldown) --playerFireCooldown;
	if (IsKeyDown(VK_SPACE) && playerFireCooldown == 0) { playerFireCooldown = 40; player->updateShotsFired(), player_bullets.emplace_back(player->getBX(), player->getBY() - player->getYSize() / 2, 0); }


	//Rare enemy management
	//=============================================================

	//spawn rare enemy after player shot 23 shots
	if (player->getShotsFired() % 23 == 0 && rare_enemy == nullptr)
	{

		//TODO
		rare_enemy = new EnemyRare();
		rare_enemy->setBX(-100);
		rare_enemy->setBY(100);
		rare_enemy->updateBoundingBox();
	}
	//if rare enemy exists, move it and check if it is out of bounds
	if (rare_enemy)
	{
		//enemy is dead, we draw the death animation
		if (rare_enemy->getDead()) rare_enemy->updateCountdown(), DrawSprite(rare_enemy->getDeathSprite(), rare_enemy->getBX(), rare_enemy->getBY(), rare_enemy->getXSize(), rare_enemy->getYSize(), 0, WHITE);
		//enemy is alive, we draw it
		else DrawSprite(rare_enemy->getStillSprite(), rare_enemy->updateBX(4), rare_enemy->getBY(), rare_enemy->getXSize(), rare_enemy->getYSize(), 0, WHITE);
		rare_enemy->updateBoundingBox();
	}

	//Player bullet management
	//=============================================================

	//drawing bullet sprites
	for (int n = 0; n < player_bullets.size(); ++n)
	{
		DrawSprite(player_bullets[n].getSprite(), player_bullets[n].getBX(), player_bullets[n].updateBY(-4), player_bullets[n].getXSize(), player_bullets[n].getYSize(), 0, WHITE);

		player_bullets[n].updateBoundingBox();
	}

	//drawing enemy bullet sprites
	for (int n = 0; n < enemy_bullets.size(); ++n)
	{
		DrawSprite(enemy_bullets[n].getSprite(), enemy_bullets[n].getBX(), enemy_bullets[n].updateBY(4) + level, enemy_bullets[n].getXSize(), enemy_bullets[n].getYSize(), 0, WHITE);

		enemy_bullets[n].updateBoundingBox();
		//if bullet is out of map -> delete it
		if (enemy_bullets[n].getBoundingBox().bottom > 640) enemy_bullets[n].setState(0);
	}

	// Collision checking
	//=============================================================

	//enemy bullet : player
	for (auto& enemyBullet : enemy_bullets)
	{
		if (checkCollision(enemyBullet, *player))
		{
			enemyBullet.setState(0);

			//players got git recently and cannot be hit again for some time
			if (player->getHit() > 0) continue;


			if (player->getLifes() > 0) player->updateLifes(), player->setHitCooldown();



		}
	}

	//player bullets : enemies
	for (auto& bullet : player_bullets)
	{
		//bullet is out of bounds
		if (bullet.getBoundingBox().top < 0)
		{
			bullet.setState(0);
			continue;
		}
		//enemies : player bullets
		for (auto& col : enemies)
		{
			for (auto& enemy : col)
			{
				if (checkCollision(bullet, enemy))
				{
					enemy.setDead();
					bullet.setState(0);
					//adding score to the player
					player->updateScore(enemy.getScore());
				}
			}
		}

		//rare_enemy : players bullets
		if (rare_enemy && checkCollision(bullet, *rare_enemy))
		{
			rare_enemy->setDead();
			bullet.setState(0);
			player->updateScore(rare_enemy->getScore());
		}
	}



	//check if player bullet hit enemy bullet -> if yes delete player bullet
	for (auto& enemyBullet : enemy_bullets)
	{
		for (auto& playerBullet : player_bullets)
		{
			if (checkCollision(playerBullet, enemyBullet))
			{
				playerBullet.setState(0);
			}

		}
	}

	//Removing entities
	//=============================================================

	//removing bullets that hit enemies or are out of bounds
	player_bullets.erase(std::remove_if(player_bullets.begin(), player_bullets.end(), [](Bullet& b) { return (b.getState() == 0); }), player_bullets.end());
	enemy_bullets.erase(std::remove_if(enemy_bullets.begin(), enemy_bullets.end(), [](EnemyBullet& e) { return (e.getState() == 0); }), enemy_bullets.end());

	//removing killed enemies and their vector if it is empty
	for (auto& col : enemies) col.erase(std::remove_if(col.begin(), col.end(), [](Enemy& e) { return (e.getDeathCountdown() <= 0); }), col.end());
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](std::vector<Enemy>& e) { return e.empty(); }), enemies.end());
	if (rare_enemy)
		if (rare_enemy->getDeathCountdown() <= 0 || rare_enemy->getBX() - (rare_enemy->getXSize() / 2) > 800) delete rare_enemy, rare_enemy = nullptr;





	//Draw game info
	DrawText(width - 100, 30, 40, WHITE, true, ("SCORE:" + std::to_string(player->getScore())).c_str());
	DrawText(width - 100, 55, 40, WHITE, true, ("LIFES:" + std::to_string(player->getLifes())).c_str());

	//Pause
	//=============================================================
	if (KeyPressed(0x50))
	{
		EndFlip();
		pauseLoop();
	}


	// Game Over checks
	//=============================================================

	for (auto& col : enemies)
	{

		if (player->getLifes() == 0 || (!col.empty() && col.back().getBY() > player->getBY() - player->getYSize() / 2 - 20))
		{
			EndFlip();
			clearLevel();
			switch (gameOverLoop())
			{
			case 1:
				initializeNewPlayer();
				level = 0;
				goto start;
				break;

			case 2:
				return;
				break;

			}
		}
	}
	//check if player won
	if (enemies.empty())
	{
		//player advances
		clearLevel();
		level++;
		goto start;
	}
	EndFlip();

	goto end;
}


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

		if (!StartFlip()) return 2;
		if (IsKeyDown(VK_ESCAPE)) return 2;
		DrawSprite(background, 400, 300, 800, 600, 0, WHITE);

		//new highscore set
		if (new_highscore_place != -1)
			DrawText(width / 2, 100, 45, elapsed_time % 60 <= 30 ? CYAN : WHITE, true, ("! NEW HIGHSCORE FOR PLACE " + std::to_string(new_highscore_place + 1) + " SET !").c_str());

		DrawText(width / 2, 200, 45, WHITE, true, "GAME OVER");
		DrawText(width / 2, height / 2, 40, WHITE, true, ("SCORE:" + std::to_string(player->getScore())).c_str());
		menu.Draw(width / 2, 400, 40);
		menu_option = menu.HandleInput();
		EndFlip();


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
	Menu menu;
	menu.AddItem("Back", [] {return 1; });

	//check if highscore was set
	std::vector<std::string> highscores = readHighscore();
	int quitHighscoreScreen = -1;
	while (1)
	{
		if (!StartFlip()) return;
		if (IsKeyDown(VK_ESCAPE)) return;

		for (int i = 0; i < highscores.size(); i++)
		{
			DrawText(800 / 2, 200 + i * 40, 45, WHITE, true, highscores[i].c_str());

		}

		menu.Draw(800 / 2, 500, 40);
		quitHighscoreScreen = menu.HandleInput();

		EndFlip();
		if (quitHighscoreScreen == 1) return;

	}
}


void Game::clearLevel()
{
	if (rare_enemy)
		delete rare_enemy, rare_enemy = nullptr;

	enemies.clear();
	player_bullets.clear();
	enemy_bullets.clear();
}


void Game::initializeLevel()
{
	//enemy vector
	std::vector<Enemy> column; // !< one column of enemies
	for (int i = 0; i < 2; i++)
		column.push_back(EnemyFront());
	for (int i = 0; i < 2; i++)
		column.push_back(EnemyMiddle());
	column.push_back(EnemyBack());
	enemies.resize(11, column);

	//filling the enemy vector
	for (int i = 0; i < enemies.size(); i++)
	{
		for (int j = 0; j < enemies[0].size(); j++)
		{
			enemies[i][j].setBX((i % 11) * 50 + 120);
			enemies[i][j].setBY(j * 60 + 70);
			enemies[i][j].updateBoundingBox();
		}
	}
}


void Game::levelIntro()
{

	if (!StartFlip()) return;
	DrawSprite(background, 400, 300, 800, 600, 0, WHITE);
	DrawText(width / 2, 300, 45, WHITE, true, ("LEVEL " + std::to_string(level)).c_str());
	EndFlip();
	Sleep(1000);
}


void Game::initializeNewPlayer()
{
	delete player;
	player = new Player(400, 550, 0);
}

void Game::pauseLoop()
{
	while (1)
	{
		if (!StartFlip()) return;
		if (IsKeyDown(VK_ESCAPE)) return;

		DrawText(800 / 2, 300, 60, WHITE, true, "PAUSE");

		DrawText(800 / 2, 500, 45, WHITE, true, "Press 'P' to resume the game");

		if (KeyPressed(0x50)) break;

		EndFlip();
	}
	EndFlip();

	for (int i = 3; i > 0; i--)
	{
		if (!StartFlip()) return;
		if (IsKeyDown(VK_ESCAPE)) return;
		DrawText(800 / 2, 300, 60, WHITE, true, ("Resuming in " + std::to_string(i) + "...").c_str());
		EndFlip();
		Sleep(1000);

	}

	return;
}



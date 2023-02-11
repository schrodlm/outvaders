#include "game.h"



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

	bool spriteAnim = false;

end:
	startFlip();

	++elapsed_time;
	if (WantQuit()) return;
	if (IsKeyDown(VK_ESCAPE)) return;

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
					EnemyBullet B; B.BX = enemy.BX; B.BY = enemy.BY; enemyBullets.push_back(B);
				}
			}
		}
	}
	if (elapsed_time % 20 == 0)
	{
		spriteAnim = !spriteAnim;
	}

	most_right = enemies.back()[0].BX;
	most_left = enemies.front()[0].BX;

	//setting direction for enemies
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


	DrawSprite(player.getSprite(), player.BX += IsKeyDown(VK_LEFT) ? -7 : IsKeyDown(VK_RIGHT) ? 7 : 0, player.BY, player.getXSize(), player.getYSize(), 3.141592 + sin(elapsed_time * 0.1) * 0.1, 0xffffffff);
	player.updateBoundingBox();


	// FIRE
	static int playerFireCooldown = 0;
	if (playerFireCooldown) --playerFireCooldown;
	//if (!IsKeyDown(VK_SPACE)) count = 0;
	if (IsKeyDown(VK_SPACE) && playerFireCooldown == 0) { Bullet B;  B.BX = player.BX; B.BY = player.BY; playerFireCooldown = 40;  bullets.push_back(B); }


	//drawing bullet sprites -> we also add angle to them so they rotate?
	for (int n = 0; n < bullets.size(); ++n)
	{
		DrawSprite(bullets[n].getSprite(), bullets[n].BX, bullets[n].BY -= 4, bullets[n].getXSize(), bullets[n].getYSize(), bullets[n].BA += 0.1f, 0xffffffff);

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
		if (checkCollision(enemyBullet, player))
		{
			enemyBullet.setState(0);

			if (player.getLives() > 0) player.setLives(player.getLives() - 1);



		}
	}
	if (player.getLives() == 0)
	{
		Flip();
		gameOverLoop();
		return;
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
		for (auto& col : enemies)
		{
			for (auto& enemy : col)
				if (checkCollision(bullet, enemy))
				{
					enemy.dead = true;
					bullet.setState(0);
					//adding score to the player
					player.updateScore(enemy.score);
					//DrawSprite(enemy.sprite_death, enemy.BX, enemy.BY, enemy.getXSize(), enemy.getYSize(), 0, 0xffffffff);
				}
		}
	}

	//removing bullets that hit enemies or are out of bounds
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return (b.getState() == 0); }), bullets.end());
	enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(), [](EnemyBullet& e) { return (e.getState() == 0); }), enemyBullets.end());

	for (auto& col : enemies) col.erase(std::remove_if(col.begin(), col.end(), [](Enemy& e) { return (e.dead_countdown <= 0); }), col.end());
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](std::vector<Enemy>& e) { return e.empty(); }), enemies.end());


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

	DrawText(width - 100, 30, 40, 0xffffffff, true, ("SCORE:" + std::to_string(player.getScore())).c_str());
	DrawText(width - 100, 55, 40, 0xffffffff, true, ("LIFES:" + std::to_string(player.getLives())).c_str());



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
void Game::gameOverLoop()
{
	while (1)
	{
		startFlip();

		if (WantQuit()) return;
		if (IsKeyDown(VK_ESCAPE)) return;
		DrawSprite(background, 400, 300, 800, 600, 0, 0xffffffff);
		DrawText(width / 2, 200, 45, 0xffffffff, true, "GAME OVER");
		DrawText(width / 2, height / 2, 40, 0xffffffff, true, ("SCORE:" + std::to_string(player.getScore())).c_str());

		Flip();
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

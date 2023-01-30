#include "lib/leetlib.h"
#include "lib/entities.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

//randomness
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
std::uniform_int_distribution<int> distribution(0, 100);


int elapsed_time = 0;

/**
 * @fn	void Game()
 *
 * @brief	Games this object
 *
 * @author	Schrodlm
 * @date	30.10.2022
 */

bool checkCollision(Entity& obj1, Entity& obj2);
void DrawTxt(std::string text, int x, int y, void* Text[]);

/*
	- names of sprites often include " " which is generally a bad practice

*/

void Game()
{
	/*
	*
	* saved sprites inside an array -> these sprites represent the name of the game - SPACE INVADERS
	* - path are hardcoded
	* - random integer 0 ->that is representing " "
	*/

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




	// SETUP


	std::vector<std::vector<Enemy>> enemies(11, std::vector<Enemy>(5));
	std::vector<Bullet> bullets;
	std::vector<EnemyBullet> enemyBullets;
	Player player;
	player.BX = 400;
	player.BY = 550;



	for (int i = 0; i < enemies.size(); i++)
	{
		for (int j = 0; j < enemies[0].size(); j++)
		{
			enemies[i][j].BX = (i % 11) * 50 + 120;
			enemies[i][j].BY = j * 60 + 70;
			enemies[i][j].updateBoundingBox();
		}
	}

	//game loop -> realized by goto
	bool direction = true;


end:

	++elapsed_time;
	if (WantQuit()) return;
	if (IsKeyDown(VK_ESCAPE)) return;

	int most_right = INT_MIN;
	int most_left = INT_MAX;
	//drawing enemies on the screen
	for (auto& col : enemies)
	{
		for (auto& enemy : col)
		{
			DrawSprite(enemy.getSprite(), enemy.BX, enemy.BY, enemy.getXSize(), enemy.getYSize(), 0, 0xffffffff);
			if (elapsed_time % 10 == 0)
				(direction) ? enemy.BX += 10 : enemy.BX -= 10;

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

			most_right = max(most_right, enemy.getBoundingBox().right);
			most_left = min(most_left, enemy.getBoundingBox().left);

		}
	}

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

	// FIRE
	//can show only 10 bullets at once on a screen -> size of bullets[]
	// as it is right now -> we dont have any time limit for shooting bullets, count doesnt work sínce it is instantly nullified by !keyDown check
	static int b = 0;
	static int count = 0;
	if (count) --count;
	//if (!IsKeyDown(VK_SPACE)) count = 0;
	if (IsKeyDown(VK_SPACE) && count == 0) { Bullet B;  B.BX = player.BX; B.BY = player.BY; count = 40;  bullets.push_back(B); }


	//drawing bullet sprites -> we also add angle to them so they rotate?
	// -> also hardcoded loop with n < 10, size of the bullets is hardcoded 
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

	//removing bullets out of bounds
	enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(), [](EnemyBullet& e) { return (e.getState() == 0); }), enemyBullets.end());



	//Collision checking
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
					enemy.setState(0);
					bullet.setState(0);
				}
		}
	}

	//removing bullets that hit enemies and dead enemies
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return (b.getState() == 0); }), bullets.end());
	for (auto& col : enemies) col.erase(std::remove_if(col.begin(), col.end(), [](Enemy& e) { return (e.getState() == 0); }), col.end());
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](std::vector<Enemy>& e) { return e.empty(); }), enemies.end());

	//all enemies dead -> game won 
	if (enemies.empty())
		return;


	//draws space invaders letters sprite on the screen 
	// - hardcoded loop 
	// - also for some reason angle change is dependent on the index of the sprite, so first letter will not be rotating and last will be rotating much more than others
	//for (int n = 0; n < strlen("space invaders"); ++n) if (n != 5)DrawSprite(Text[n], n * 40 + 150, 30, 20, 20, sin(elapsed_time * 0.1) * n * 0.01);
	DrawTxt("hello world", 40, 40, Text);
	//rendering provided by the lib
	Flip();

	goto end;
}


//checks collision between two entities
bool checkCollision(Entity& obj1, Entity& obj2)
{
	RECT bb1 = obj1.getBoundingBox();
	RECT bb2 = obj2.getBoundingBox();

	return (bb1.left <= bb2.right &&
		bb1.right >= bb2.left &&
		bb1.top >= bb2.bottom &&
		bb1.bottom <= bb2.top);
}



/*
	Creates any text with provided sprites
*/
void DrawTxt(std::string text, int x, int y, void* Text[]) {
	int i = 0;
	for (auto c : text) {
		if (c != ' ') {
			DrawSprite(Text[c - 'a'], x + i * 40, y, 20, 20, sin(elapsed_time * 0.1) * i * 0.01);
		}
		i++;
	}
}



/**
 * @fn	void OldGame()
 *
 * @brief	Old game
 *
 * @author	Schrodlm
 * @date	30.10.2022
 */

void OldGame()
{
	void* sprite = LoadSprite("sprite.png");
	float size = 10;
	float angle = 0;
	while (!WantQuit() && !IsKeyDown(VK_ESCAPE))
	{
		angle += 0.01f;
		float mx, my;
		GetMousePos(mx, my);
		DWORD col = 0xffffffff; // white
		if (IsKeyDown(VK_LBUTTON)) col = 0xffff0000; // solid red
		if (IsKeyDown(VK_RBUTTON)) col = 0x8000ff00; // 50% transparent green
		if (IsKeyDown(VK_UP)) size++;
		if (IsKeyDown(VK_DOWN)) size--;

		DrawSprite(sprite, 400, 300, 100, 100, angle);
		DrawSprite(sprite, mx, my, size, size, 0, col);
		Flip();
	}
}

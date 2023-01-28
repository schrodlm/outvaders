#include "lib/leetlib.h"
#include "lib/entities.h"
#include <math.h>
#include <iostream>
#include <vector>

int x[50];
int y[50];
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

	int UX = 400, UY = 550;
	//void* Enemy = LoadSprite((Path + "Little Invader.png").c_str());

	std::vector<Enemy> enemies(55);
	std::vector<Bullet> bullets;
	void* U = LoadSprite("gfx/Big Invader.png");
	void* bull = LoadSprite("gfx/bullet.png");



	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i].BX = (i % 11) * 50 + 120;
		enemies[i].BY = (i / 11) * 60 + 70;
		enemies[i].setBoundingBox(enemies[i].BX, enemies[i].BY);
	}

	//game loop -> realized by goto
end:

	++elapsed_time;
	if (WantQuit()) return;
	if (IsKeyDown(VK_ESCAPE)) return;


	//drawing enemies on the screen
	for (auto& enemy : enemies)
	{
		int xo = 0, yo = 0;
		//int n1 = elapsed_time;
		//int n2 = elapsed_time;
		//if (((n1 >> 6) & 0x7) == 0x7)xo += (1 - cos((n1 & 0x7f) / 64.0f * 2.f * 3.141592)) * (20 + ((n * n) % 9));
		//if (((n1 >> 6) & 0x7) == 0x7)yo += (sin((n1 & 0x7f) / 64.0f * 2.f * 3.141592)) * (20 + ((n * n) % 9));
		//if (((n2 >> 8) & 0xf) == 0xf)yo += (1 - cos((n2 & 0xff) / 256.0f * 2.f * 3.141592)) * (150 + ((n * n) % 9));
		if (enemy.getState() == 0) continue;
		DrawSprite(enemy.sprite, enemy.BX + xo, enemy.BY + yo, enemy.getXSize(), enemy.getYSize(), sin(elapsed_time * 0.1) * 0.1, 0xffffffff);
	}





	DrawSprite(U, UX += IsKeyDown(VK_LEFT) ? -7 : IsKeyDown(VK_RIGHT) ? 7 : 0, UY, 50, 50, 3.141592 + sin(elapsed_time * 0.1) * 0.1, 0xffffffff);

	// FIRE
	//can show only 10 bullets at once on a screen -> size of bullets[]
	// as it is right now -> we dont have any time limit for shooting bullets, count doesnt work sínce it is instantly nullified by !keyDown check
	static int b = 0;
	static int count = 0;
	if (count) --count;
	if (!IsKeyDown(VK_SPACE)) count = 0;
	if (IsKeyDown(VK_SPACE) && count == 0) { Bullet B;  B.BX = UX; B.BY = UY; count = 15;  bullets.push_back(B); }


	//drawing bullet sprites -> we also add angle to them so they rotate?
	// -> also hardcoded loop with n < 10, size of the bullets is hardcoded 
	for (int n = 0; n < bullets.size(); ++n)
	{
		DrawSprite(bull, bullets[n].BX, bullets[n].BY -= 4, 10, 10, bullets[n].BA += 0.1f, 0xffffffff);
		//set new bounding boxes for each bullet
		bullets[n].setBoundingBox(bullets[n].BX, bullets[n].BY);
	}

	//Collision checking
	for (auto& bullet : bullets)
	{
		for (auto& enemy : enemies)
		{
			if (checkCollision(bullet, enemy))
			{
				enemy.setState(0);
				bullet.setState(0);
			}
		}
	}

	//removing bullets that hit enemies and dead enemies
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return (b.getState() == 0); }), bullets.end());
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy& e) { return (e.getState() == 0); }), enemies.end());


	//all enemies dead -> game won 
	if (enemies.empty())
		return;


	//draws space invaders letters sprite on the screen 
	// - hardcoded loop 
	// - also for some reason angle change is dependent on the index of the sprite, so first letter will not be rotating and last will be rotating much more than others
	//for (int n = 0; n < strlen("space invaders"); ++n) if (n != 5)DrawSprite(Text[n], n * 40 + 150, 30, 20, 20, sin(elapsed_time * 0.1) * n * 0.01);
	DrawTxt("podrazka od bot", 40, 40, Text);
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
		bb1.top <= bb2.bottom &&
		bb1.bottom >= bb2.top);
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

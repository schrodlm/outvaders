#pragma once

#include <windows.h>

typedef unsigned long       DWORD;

#define PI ((float)3.1415926535)


//COLOURS
#define WHITE 0xffffffff
#define CYAN 0xff5d8aa8
#define RED 0xffff0000


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 'system' - screen is always 800 x 600
// simply call StartFrame and Flip alternately to run the game; StartFrame returns false if the user is trying to quit

bool WantQuit(DWORD clearcolor = 0);
void EndFlip(); // flips the screen, frame locked to 60 hz
void startFlip();
//void Game(int width, int height); // you write this :)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// input
void GetMousePos(float& x, float& y); // 0,0 is top left; 800,600 is bottom right
bool IsKeyDown(int key); // use windows VK_ codes for special keys, eg VK_LEFT; use capital chars for letter keys eg 'A', '0'. use VK_LBUTTON and VK_RBUTTON for mouse buttons. 
bool KeyPressed(int key);
bool IsKeyHitSinceLastFlip(int key);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// output
// 'sprite output' 
void* LoadSprite(const char* fname);
void FreeSprite(void* sprite);
void DrawSprite(void* sprite, float xcentre, float ycentre, float xsize, float ysize, float rotate_angle_radians = 0, DWORD tint_col_argb = 0xffffffff);


//Text
int DrawText(int x, int y, int size, int col, bool centered, const char* pformat, ...);
void DrawTextFromSprites(const char* text, int x, int y, void* Text[]);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sound
int PlayMusic(const char* fname, float volume = 1); // returns a handle which you only need if you want to change volume later with ChangeVolume
void StopMusic();

// sorry for this being 'snd' but otherwise it clashes with a windows #define (PlaySound) grr
void* LoadSnd(const char* fname, bool loop = false); // if you set loop the sample will go on looping forever, until you call StopSound
int PlaySnd(void* sound, float volume = 1); // returns a handle which you only need if you are going to call StopSound or ChangeVolume()
void StopSnd(int handle);
void ChangeVolume(int handle, float newvolume = 1);

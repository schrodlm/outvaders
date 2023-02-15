#pragma once

#include <windows.h>
#include "resource.h"

typedef unsigned long       DWORD;

#define PI ((float)3.1415926535)


//COLOUR MACROS
#define WHITE 0xffffffff
#define CYAN 0xff5d8aa8
#define RED 0xffff0000


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 'system' - screen is always 800 x 600
// simply call StartFrame and Flip alternately to run the game; StartFrame returns false if the user is trying to quit

//System
//=============================================================

/**
 * clearing the device, setting various rendering states and beginning the scene.
 * The function also has a message loop to check for a quit message and return true if a WM_QUIT message is received.
 */
bool StartFlip(DWORD clearcolor = 0);

/**
 *  responsible for finalizing the rendering and updating of a frame and presenting it to the user.
 * It also provides basic input handling by clearing the keyboard input buffer and resetting the cursor. Also locks the framerate to 60 Hz.
 */
void EndFlip();


//Input
//=============================================================

/**
 * This game doesn't need mouse and it isn't using one, i left it here if i later decided otherwise. Gets mouse position.
 */
void GetMousePos(float& x, float& y); // 0,0 is top left; 800,600 is bottom right

bool IsKeyDown(int key);
bool KeyPressed(int key);
/**
 * Checks if key was pressed since last render of the scene.
 */
bool IsKeyHitSinceLastFlip(int key);


//Output
//=============================================================

/**
 * Loads up sprite from a resource path.
 */
void* LoadSprite(const char* fname);

/**
 * Free memory used by sprite.
 */
void FreeSprite(void* sprite);

/**
 * Renders provided sprite on screen.
 */
void DrawSprite(void* sprite, float xcentre, float ycentre, float xsize, float ysize, float rotate_angle_radians = 0, DWORD tint_col_argb = 0xffffffff);


//Output
//=============================================================

/**
 * Renders provided text on screen.
 */
int DrawText(int x, int y, int size, int col, bool centered, const char* pformat, ...);

/**
 * Draw text on the screen, text consist of sprites representing letters.
 */
void DrawTextFromSprites(const char* text, int x, int y, void* Text[]);

//Sound
//=============================================================
int PlayMusic(const char* fname, float volume = 1); // returns a handle which you only need if you want to change volume later with ChangeVolume
void StopMusic();


void* LoadSnd(const char* fname, bool loop = false); // if you set loop the sample will go on looping forever, until you call StopSound
int PlaySnd(void* sound, float volume = 1); // returns a handle which you only need if you are going to call StopSound or ChangeVolume()
void StopSnd(int handle);
void ChangeVolume(int handle, float newvolume = 1);

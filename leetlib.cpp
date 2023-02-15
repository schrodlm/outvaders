

#pragma warning(disable:4995)
#include "leetlib.h"
#include "game.h"
#include "menu.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <strsafe.h>
#include <math.h>
#include <map>
#include <direct.h>
#include <malloc.h>
#include "resource.h"

//debugging
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


//sound
#include "lib/fmod/api/inc/fmod.h"
#pragma comment(lib,"lib/fmod/api/lib/fmodvc.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


#pragma warning(disable:4244)

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold tea2

bool fullscreen;


// A structure for our custom vertex type
struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw; // The transformed position for the vertex
	DWORD color;        // The vertex color
	float u, v;
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)


// TEXT HANDLING


// font manager
std::map<int, LPD3DXFONT> fonts;
int intextbatch = 0;
LPD3DXSPRITE fontsprite;
#define MA_RELEASE(x) {int c=0;if (x) c=(x)->Release();x=NULL;}
typedef unsigned int u32;

void ReleaseFonts();

void StartTextBatch(int size = 0);
void EndTextBatch();

void SetCurrentTexture(void* tex);




LPD3DXFONT FindFont(int size)
{
	std::map<int, LPD3DXFONT>::iterator fit = fonts.find(size);
	if (fit == fonts.end())
	{
		LPD3DXFONT m_pFont = NULL;
		if (D3DXCreateFont(g_pd3dDevice, size, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Cosmic Alien"), &m_pFont) != S_OK)
			return NULL;


		fit = fonts.insert(std::pair<int, LPD3DXFONT>(size, m_pFont)).first;

	}
	if (fit != fonts.end())
	{
		return fit->second;
	}
	else return NULL;
}

/**
 * Release any fonts that have been created with Direct3D..
 *
 */
void ReleaseFonts()
{
	if (intextbatch) EndTextBatch();
	for (std::map<int, LPD3DXFONT>::iterator it = fonts.begin(); it != fonts.end(); ++it)
	{
		MA_RELEASE(it->second);
	}
	fonts.clear();
	MA_RELEASE(fontsprite);
	intextbatch = 0;
}

/**
 * Set up a Direct3D sprite object for rendering text..
 *
 * \param size font size
 */
void StartTextBatch(int size)
{
	if (intextbatch) EndTextBatch();
	intextbatch = 1;
	if (size) FindFont(size);
	if (!fontsprite)
	{
		D3DXCreateSprite(g_pd3dDevice, &fontsprite);
	}
	if (fontsprite)
	{
		fontsprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	}


}

void EndTextBatch()
{
	if (intextbatch)
	{
		intextbatch = 0;
		if (fontsprite)
		{
			fontsprite->End();
		}
		//for (std::map<int, LPD3DXFONT>::iterator fit=fonts.begin();fit!=fonts.end();++fit) fit->second->End();
	}
}


//-----------------------------------------------------------------------------
// Name: DrawText()
// Desc: Renders provided text on the screen
//-----------------------------------------------------------------------------
int DrawText(int x, int y, int size, int col, bool centered, const char* pformat, ...)
{
	char debugtext[8192];
	va_list	parameter;
	va_start(parameter, pformat);
	vsprintf(debugtext, pformat, parameter);
	LPD3DXFONT m_pfont = FindFont(size);
	if (!m_pfont) return 0;
	RECT r = { x,y,x + 100,y + 100 };

	m_pfont->DrawText(intextbatch ? fontsprite : NULL, debugtext, -1, &r, DT_CALCRECT, 255);
	int wid = r.right - r.left;
	if (centered) r.left -= wid / 2;
	if (centered) r.right -= wid / 2;
	m_pfont->DrawText(intextbatch ? fontsprite : NULL, debugtext, -1, &r, DT_TOP | DT_LEFT, col);
	return r.right - r.left;


}

//-----------------------------------------------------------------------------
// Name: DrawTextFromSprites()
// Desc: Renders text consisting of provided sprites
//-----------------------------------------------------------------------------
void DrawTextFromSprites(const char* text, int x, int y, void* Text[]) {
	int i = 0;
	for (int c = 0; c < strlen(text); c++) {
		if (text[c] != ' ') {
			DrawSprite(Text[text[c] - 'a'], x + i * 40, y, 20, 20, 0);
		}
		i++;
	}
}



//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D(HWND hWnd)
{
	// Create the D3D object.
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = !fullscreen;
	d3dpp.SwapEffect = fullscreen ? D3DSWAPEFFECT_FLIP : D3DSWAPEFFECT_DISCARD;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dpp.BackBufferFormat = fullscreen ? D3DFMT_A8R8G8B8 : D3DFMT_UNKNOWN;
	d3dpp.BackBufferWidth = 800;
	d3dpp.BackBufferHeight = 600;
	d3dpp.FullScreen_RefreshRateInHz = fullscreen ? 60 : 0;


	// Create the D3DDevice
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	// Device state would normally be set here

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitVB()
// Desc: Creates a vertex buffer and fills it with our tea2. The vertex
//       buffer is basically just a chuck of memory that holds tea2. After
//       creating it, we must Lock()/Unlock() it to fill it. For indices, D3D
//       also uses index buffers. The special thing about vertex and index
//       buffers is that they can be created in device memory, allowing some
//       cards to process them in hardware, resulting in a dramatic
//       performance gain.
//-----------------------------------------------------------------------------
HRESULT InitVB()
{

	if (FAILED(g_pd3dDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX),
		D3DUSAGE_DYNAMIC, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}



	//D3DXCreateTextureFromFile(g_pd3dDevice,"bg.jpg",&g_bgtex);
	//D3DXCreateTextureFromFile(g_pd3dDevice,"arrow.png",&g_arrow);


	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	if (g_pVB != NULL)
		g_pVB->Release();

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();


}



//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
bool g_keydown[256];
int g_keyhit[256];
int g_mb;
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		g_mb |= 1;
		g_keydown[VK_LBUTTON] = true;
		g_keyhit[VK_LBUTTON]++;
		break;
	case WM_RBUTTONDOWN:
		SetCapture(hWnd);
		g_keydown[VK_RBUTTON] = true;
		g_keyhit[VK_RBUTTON]++;
		g_mb |= 2;
		break;
	case WM_MBUTTONDOWN:
		SetCapture(hWnd);
		g_mb |= 4;
		g_keydown[VK_MBUTTON] = true;
		g_keyhit[VK_MBUTTON]++;
		break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		g_mb &= ~1;
		g_keydown[VK_LBUTTON] = false;
		break;
	case WM_RBUTTONUP:
		ReleaseCapture();
		g_mb &= ~2;
		g_keydown[VK_RBUTTON] = false;
		break;
	case WM_MBUTTONUP:
		ReleaseCapture();
		g_mb &= ~4;
		g_keydown[VK_MBUTTON] = false;
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		g_keydown[wParam & 255] = true;
		g_keyhit[wParam & 255]++;
		return 0;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		g_keydown[wParam & 127] = false;
		break;

	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	case WM_ACTIVATEAPP:
		if (!wParam)
		{
			memset(g_keydown, 0, sizeof(g_keydown));
		}
		break;

	case WM_ACTIVATE:
		if (WA_INACTIVE != wParam)
		{
			// Make sure the device is acquired, if we are gaining focus.

		}
		break;
	}


	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//Time management
LARGE_INTEGER frequency;
LARGE_INTEGER previousTime;
LARGE_INTEGER currentTime;
LARGE_INTEGER startTime;

float deltaTime;

extern HWND hWnd;
HWND hWnd;

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR cmd, INT)
{
	HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));

	// message box for a fullscreen
	int id = MessageBox(NULL, "Run the game in fullscreen?", "Space Outvaders!", MB_YESNOCANCEL);
	if (id == IDCANCEL) return 0;
	fullscreen = (id == IDYES);



	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
					  GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
					  "SpaceOutvaders", NULL };
	wc.hIcon = hIcon;
	RegisterClassEx(&wc);

	/*
	This code is likely a part of a window creation process. It creates a RECT structure that defines the size and position of the window
	, and a variable 'style' that defines the appearance and behavior of the window. The fullscreen variable is used to decide whether the window should be fullscreen or not.
	If fullscreen is true, the window style is set to WS_POPUP which creates a fullscreen window without any title bar or borders.
	If fullscreen is false, the window style is set to WS_OVERLAPPEDWINDOW which creates a standard window with a title bar, borders, and a close button.
	The last line of code adjusts the RECT structure to include the area for the non-client elements, such as the title bar and borders.
	*/
	RECT r = { 0,0,800,600 };
	int style = fullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
	style |= WS_VISIBLE;
	AdjustWindowRect(&r, style, false);


	hWnd = CreateWindow("SpaceOutvaders", "Space Outvaders",
		style, 0, 0, r.right - r.left, r.bottom - r.top,
		GetDesktopWindow(), NULL, wc.hInstance, NULL);


	/*
		function from FMOD library -> this library provides audio playback and mixing functionality
		-> FSOUND_Init() = initialize FMOD library and set up the audio output

		! FMOD is external library and is not part of DirectX !
	*/
	//FSOUND_Init(44100, 42, 0);

	// Get the current time
	QueryPerformanceCounter(&startTime);
	startTime = previousTime;



	// Get the frequency of the performance counter
	QueryPerformanceFrequency(&frequency);
	// Initialize Direct3D
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		// Create the vertex buffer
		if (SUCCEEDED(InitVB()))
		{
			//Disable mouse
			ShowCursor(false);
			SetCapture(GetForegroundWindow());

			// Show the window
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);


			Menu mainMenu;
			mainMenu.AddItem("Start Game", [] {return 1; });
			mainMenu.AddItem("Highscores", [] {return 2; });
			mainMenu.AddItem("Quit", [] {return 3; });
			int menu_option = 0;
			Game* game = new Game(r.right - r.left, r.bottom - r.top);

			while (menu_option != 3 && menu_option != 1)
			{
				menu_option = mainMenu.Loop();

				//option handler
				switch (menu_option)
				{
				case 1:
					game->gameLoop();
					break;

				case 2:
					game->highscoreLoop();
					break;

				case 3:
					break;
				}
			}


			delete game;

		}
	}

	UnregisterClass("SpaceOutvaders", wc.hInstance);

	//dumps out memory leaks 
	_CrtDumpMemoryLeaks();
	return 0;
}



bool StartFlip(DWORD clearcol)
{
	// Enter the message loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT) return false;
	}

	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, clearcol, 1.0f, 0);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);

	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	//g_pd3dDevice->SetTexture(0,NULL);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DVIEWPORT9 vp = { 0,0, 800, 600, 0, 1 };
	//g_pd3dDevice->SetViewport(&vp);

	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	// Begin the scene
	g_pd3dDevice->BeginScene();

	//UpdateDirectInput();

	return true;
}


int	GetTimeInMS() // ...since start of program
{
	LARGE_INTEGER arse;
	QueryPerformanceCounter(&arse);
	return ((arse.QuadPart - startTime.QuadPart) * 1000) / frequency.QuadPart;
}

void EndFlip()
{
	// Get the current time

	QueryPerformanceCounter(&currentTime);

	// Calculate the time elapsed since the last frame
	deltaTime = (float)(currentTime.QuadPart - previousTime.QuadPart) / frequency.QuadPart;

	// Limit the frame rate to 60 frames per second
	if (deltaTime < 1.0f / 60.0f)
	{
		Sleep((DWORD)((1.0f / 60.0f - deltaTime) * 1000.0f));
	}

	g_pd3dDevice->EndScene();



	// Present the backbuffer contents to the display
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	Sleep(0);
	memset(g_keyhit, 0, sizeof(g_keyhit));
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	previousTime = currentTime;
}




void GetMousePos(float& x, float& y) // 0,0 is top left; 800,600 is bottom right
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(hWnd, &p);
	x = p.x;
	y = p.y;
}

bool KeyPressed(int key)
{
	return g_keyhit[key & 255];

}

bool IsKeyDown(int key) // use windows VK_ codes for special keys, eg VK_LEFT; use capital chars for letter keys eg 'A', '0'
{
	return g_keydown[key & 255];
}

bool IsKeyHitSinceLastFlip(int key)
{
	return g_keyhit[key & 255] > 0;
}




// 'sprite output' 
void* LoadSprite(const char* fname)
{
	IDirect3DTexture9* tex = NULL;
	D3DXCreateTextureFromFile(g_pd3dDevice, fname, &tex);
	if (tex == NULL) throw EXCEPTION_ACCESS_VIOLATION;
	return tex;
}

void SetCurrentTexture(void* tex)
{
	IDirect3DTexture9* t = (IDirect3DTexture9*)tex;
	g_pd3dDevice->SetTexture(0, t);
}

void FreeSprite(void* sprite)
{
	IDirect3DTexture9* pTexture = static_cast<IDirect3DTexture9*>(sprite);
	pTexture->Release();

}




void DrawSprite(void* sprite, float x, float y, float xsize, float ysize, float angle, DWORD col)
{
	SetCurrentTexture(sprite);
	float c = cosf(angle);
	float s = sinf(angle);

	float x1 = -xsize / 2.0f;
	float y1 = -ysize / 2.0f;

#define ROTATE(xx,yy) x + (xx)*c+(yy)*s, y + (yy)*c-(xx)*s
	CUSTOMVERTEX vertex[] =
	{
	{ ROTATE(x1, y1), 0.5f, 1.0f, col, 0, 0, },
	{ ROTATE(x1 + xsize, y1), 0.5f, 1.0f, col, 1, 0, },
	{ ROTATE(x1, y1 + ysize), 0.5f, 1.0f, col, 0, 1, },
	{ ROTATE(x1 + xsize, y1 + ysize), 0.5f, 1.0f, col, 1, 1, },
	};
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(CUSTOMVERTEX));
}


FSOUND_STREAM* music;

int PlayMusic(const char* fname, float volume)
{
	if (music) StopMusic();
	music = FSOUND_Stream_Open(fname, FSOUND_LOOP_NORMAL, 0, 0);
	int chan = FSOUND_Stream_Play(FSOUND_FREE, music);
	if (volume <= 0) volume = 0;
	if (volume > 1) volume = 1;
	FSOUND_SetVolume(chan, (int)(volume * 255));
	return chan;
}

void StopMusic()
{
	if (music)
	{
		FSOUND_Stream_Close(music);
	}
	music = NULL;
}

void* LoadSnd(const char* fname, bool looped)
{
	int flags = 0;
	if (looped) flags |= FSOUND_LOOP_NORMAL;
	return FSOUND_Sample_Load(FSOUND_FREE, fname, flags, 0, 0);
}

int PlaySnd(void* sound, float volume)
{
	if (!sound) return -1;
	if (volume <= 0) volume = 0;
	if (volume > 1) volume = 1;
	int chan = FSOUND_PlaySound(FSOUND_FREE, (FSOUND_SAMPLE*)sound);
	FSOUND_SetVolume(chan, (int)(volume * 255));
	return chan;
}

void StopSnd(int handle)
{
	if (handle <= 0) return;
	FSOUND_StopSound(handle);
}

void ChangeVolume(int handle, float volume)
{
	if (handle <= 0) return;
	if (volume <= 0) volume = 0;
	if (volume > 1) volume = 1;
	FSOUND_SetVolume(handle, (int)(volume * 255));
}

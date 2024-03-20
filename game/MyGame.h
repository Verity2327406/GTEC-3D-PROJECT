#pragma once


#include "Game.h"


class CMyGame : public CGame
{
public:
	CMyGame();
	~CMyGame();

    // ----  Declare your game variables and objects here -------------

	// Variables
	int score;
	int level;
	int damage = 30; // Damage of the bullets
	string curWeapon = "basic";
	CVector mousePos;

	int levelMax = 25;
	int spawned = 0; // How many have been spawned since the start of the level

	int debugMode = false;
	bool textSpawned;

	// Models
	CModel box;
	CModel sand;
	CModel player;
	CModel gemTest;
	
	// Model Lists
	CModelList enemyList;
	CModelList shotList;
	
	// game world floor
	CFloor floor;
	
	// Font
	CFont font;

	// Sound

	// Positions
	CVector north = CVector(0, 0, -1000);
	CVector south = CVector(0, 0, 1000);
	CVector west = CVector(-1000, 0, 0);
	CVector east = CVector(1000, 0, 0);
	

	// -----   Add you member functions here ------
   
   void PlayerControl();
   void EnemyControl();
   void CameraControl(CGraphics* g);
   void CollisionManager();
   void LevelManager();
	
   // ---------------------Event Handling --------------------------

	// Game Loop Funtions
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);
	virtual void OnRender3D(CGraphics* g);

	// Game Life Cycle
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartLevel(int level);
	virtual void OnStartGame();
	virtual void OnGameOver();
	virtual void OnTerminate();

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnRButtonDown(Uint16 x,Uint16 y);
	virtual void OnRButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonDown(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);
};

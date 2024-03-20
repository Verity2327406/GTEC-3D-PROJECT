#include "MyGame.h"
#include <chrono>

using namespace std;

CMyGame::CMyGame(void) 
{
	score = 0;
}

CMyGame::~CMyGame(void) {}

std::chrono::steady_clock::time_point lastShotTime;



// --------  game initialisation --------
void CMyGame::OnInitialize()
{
	// Loading graphics and sound assets
	cout << "Loading assets" << endl;
	font.LoadDefault();
	Light.Enable();

	// load player obj model from the models
	

	// load box obj model from the models/box folder
	box.LoadModel("box/box.obj");
	box.SetScale(2.0f);

	gemTest.LoadModel("gemtest/gem.obj");
	gemTest.SetScale(40.0f);



	// show floor grid
	floor.ShowGrid(true);
	floor.LoadTexture("sand.bmp");
	floor.SetTiling(true);
	//ShowBoundingBoxes(true);	
}

// Game Logic in the OnUpdate function called every frame

void CMyGame::OnUpdate()
{
	if (IsMenuMode() || IsGameOver()) return;

	long t = GetTime();

	if (player.GetPositionV().DistanceXZ(north) <= 100)
		cout << "At North";

	// --- updating models ----
	player.Update(t);
	shotList.Update(t);
	enemyList.Update(t);
	gemTest.Update(t);
	
	
    // My Control Functions
	PlayerControl();
	EnemyControl();
	CollisionManager();
	LevelManager();

	// Removed deleted
	shotList.delete_if(deleted);
	enemyList.delete_if(deleted);
}

void CMyGame::CollisionManager() {
#pragma region Basic Bullets
	if (curWeapon == "basic") {
		for (CModel* shot : shotList) {
			for (CModel* enemy : enemyList) {
				if (shot->HitTest(enemy)) {
					// Change enemy health
					enemy->SetHealth(enemy->GetHealth() - damage);
					// Delete the shot
					shot->Delete();
					// Return so we dont hit two enemies with one bullet!
					return;
				}
			}
		}
	}
#pragma endregion
}

void CMyGame::PlayerControl()
{
	#pragma region Movement
	float speed = 500;

	if (IsKeyDown(SDLK_w))
		player.SetZVelocity(-speed);
	else if (IsKeyDown(SDLK_s))
		player.SetZVelocity(speed);
	else
		player.SetZVelocity(0);

	if (IsKeyDown(SDLK_a))
		player.SetXVelocity(-speed);
	else if (IsKeyDown(SDLK_d))
		player.SetXVelocity(speed);
	else
		player.SetXVelocity(0);
	#pragma endregion

	player.SetRotationToPoint(mousePos.x, mousePos.z);

#pragma region Player Shoot
	// Define fire rate (shots per second)
	float fireRate = 10; // Example: 2 shots per second
	// Calculate time interval between shots based on fire rate
	float timeBetweenShots = 1.0f / fireRate;
	// Get the current time
	auto currentTime = std::chrono::steady_clock::now();

	// Calculate the time elapsed since the last shot
	std::chrono::duration<float> elapsed = currentTime - lastShotTime;

	// If enough time has elapsed, allow shooting
	if (IsLButtonDown() && elapsed.count() >= timeBetweenShots) {
		// Update the last shot time to the current time
		lastShotTime = currentTime;

		// Create and configure the shot
		CModel* shot = new CLine(CVector(0, 0, 0), 150);
		shot->SetPositionV(player.GetLocalPositionV(CVector(100, 0, 0)));
		shot->SetDirectionV(player.GetRotationV());
		shot->SetRotationV(player.GetRotationV());
		shot->SetSpeed(5000);
		shot->SetColor(255, 255, 0);
		shot->Die(1000);
		shotList.push_back(shot);
	}
#pragma endregion

	if (player.GetHealth() <= 0) {
		GameOver();
	}
}

void CMyGame::EnemyControl()
{
#pragma region Movement
		for (CModel* box : enemyList) {
			// Movement
			for (CModel* otherBox : enemyList) {
				if ((box != otherBox && box->HitTestFront(otherBox)) || box->HitTestFront(&player)) {
					box->SetSpeed(0);
					break; // Exit the inner loop once a collision is detected
				}
				else {
					box->MoveTo(player.GetPositionV().x, player.GetPositionV().z, 100);
					box->SetRotationV(player.GetPositionV());
				}
			}
#pragma endregion

#pragma region CheckIfDead
			if (box->GetHealth() <= 0) {
				score += 100;
				box->Delete();
			}
#pragma endregion

	}
}

void CMyGame::LevelManager() {
	if (spawned < levelMax) {
		if (enemyList.size() < 15) {
			if (rand() % 60 == 0) {
				CModel* newBox = new CModel(box);
				newBox->SetColor(255, 1, 1);
				newBox->SetPosition(float(750 - rand()%1500), 50, float(750 - rand() % 1500));
				enemyList.push_back(newBox);
				spawned++;
			}
		}
	}
	else if(spawned >= levelMax && enemyList.size() == 0) {
		level++;

		// Choose one of four directions (North, East, South, West) on the map

		// Drop gems on the ground for score!!

		// Randomise rooms (loot rooms etc)

		OnStartLevel(level);
	}
}


void CMyGame::OnDraw(CGraphics* g)
{
#pragma region DEBUG
	if (debugMode) {
		font.SetColor(CColor::Red()); font.SetSize(25); font.DrawTextW(10, Height - 25, "DEBUG MODE");
		// draw bullets in world
		font.SetColor(CColor::Red()); font.SetSize(25); font.DrawTextW(10, Height - 50, "Bullets In World: " + to_string(shotList.size()));

		// draw enemies in world
		font.SetColor(CColor::Red()); font.SetSize(25); font.DrawText(10, Height - 75, "Enemies In World: " + to_string(enemyList.size()));

		// draw current level in world
		font.SetColor(CColor::Red()); font.SetSize(25); font.DrawText(10, Height - 100, "Current Level: " + to_string(level));

		// draw total spawned in world
		font.SetColor(CColor::Red()); font.SetSize(25); font.DrawText(10, Height - 125, "Spawned Since Level Start: " + to_string(spawned));

		// draw total left to spawn in world
		font.SetColor(CColor::Red()); font.SetSize(25); font.DrawText(10, Height - 150, "Enemies till level " + to_string(level + 1) + " : " + to_string(levelMax - spawned));

		// print the player X and Z positions in the console
		cout << player.GetPositionV().x << " " << player.GetPositionV().z << endl;
	}
#pragma endregion

	font.SetColor(CColor::White()); font.SetSize(25); font.DrawTextW(Width - 400, Height - 25, "Score: " + to_string(score));

	// draw GAME OVER if game over
   	if (IsGameOver())
   	{
		font.SetSize(64); font.SetColor( CColor::Red()); 
		font.DrawText( 250,300, "GAME OVER");	
	}
}

void CMyGame::OnRender3D(CGraphics* g)
{
	CameraControl(g);

	// ------- Draw your 3D Models here ----------
	floor.Draw(g);
	player.Draw(g);
	gemTest.Draw(g);

	shotList.Draw(g);
	enemyList.Draw(g);
	

	//ShowBoundingBoxes(true);
	ShowCoordinateSystem();
	
}


void CMyGame::CameraControl(CGraphics* g)
{
	world.rotation.x = 45;     // tilt: rotation of game world around x-axis
	world.rotation.y = 0;    // rotation: rotation of game world around y-axis
	world.scale = 1.0f;	       // scaling the game world

	// ------ Global Transformation Functions -----
	glScalef(world.scale, world.scale, world.scale);  // scale the whole game world
	glTranslatef(world.position.x, world.position.y, world.position.z);  // translate game world
	glRotatef(world.rotation.x, 1, 0, 0);	// rotate game world around x-axis
	glRotatef(world.rotation.y, 0, 1, 0); 	// rotate game world around y-axis
	glRotatef(world.rotation.z, 0, 0, 1); 	// rotate game world around z-axis
	

	UpdateView();
	Light.Apply();
}





// called at the start of a new game - display menu here
void CMyGame::OnDisplayMenu()
{
	StartGame();	// this allows to start the game immediately	
}

// called when Game Mode entered
void CMyGame::OnStartGame()
{
	score = 0;
	level = 1;
	enemyList.delete_all();
	shotList.delete_all();
	floor.SetSize(2000, 2000);
	player.SetSize(100, 100, 100);
	player.SetPosition(0, 50, 0);
	gemTest.SetPosition(0, 70, 0);
	gemTest.SetRotation(0, 10, 45);
	OnStartLevel(level);
}

void CMyGame::OnStartLevel(int level)
{
	spawned = 0;
	player.SetHealth(100);
	levelMax = levelMax * (level * 0.75);
}


// called when Game is Over
void CMyGame::OnGameOver()
{
	
}

// one time termination code
void CMyGame::OnTerminate()
{
}

// -------    Keyboard Event Handling ------------

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	
	if (sym == SDLK_SPACE)
	{
		if (IsPaused()) ResumeGame();
		else PauseGame();

	}
	if (sym == SDLK_F2) NewGame();
	
	//  add a cloned box
	if (sym == SDLK_F3)
	{
		debugMode = ~debugMode;
	}
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	
}

// -----  Mouse Events Handlers -------------

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
	mousePos = ScreenToFloorCoordinate(x, y);
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{    
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
	
	
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
	
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}

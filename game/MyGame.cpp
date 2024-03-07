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

	// show floor grid
	floor.ShowGrid(true);

	//ShowBoundingBoxes(true);	
}

// Game Logic in the OnUpdate function called every frame

void CMyGame::OnUpdate()
{
	if (IsMenuMode() || IsGameOver()) return;

	long t = GetTime();

	// --- updating models ----
	player.Update(t);
	shotList.Update(t);
	boxList.Update(t);
	
	
    // My Control Functions
	PlayerControl();
	BoxControl();
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
		shotList.push_back(shot);
	}
}

void CMyGame::BoxControl()
{
	for (CModel* box : boxList) {
		box->MoveTo(player.GetPositionV().x, player.GetPositionV().z, 100);
		box->SetRotationV(player.GetPositionV());
	}
}


void CMyGame::OnDraw(CGraphics* g)
{
	// draw score
	font.SetColor(CColor::Red()); font.DrawNumber(10,Height-50, score);
	
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

	shotList.Draw(g);
	boxList.Draw(g);
	

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
	boxList.delete_all();
	floor.SetSize(1500, 1500);
	player.SetSize(100, 100, 100);
	player.SetPosition(0, 50, 0);
}

void CMyGame::OnStartLevel(int level)
{
	
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
	if (sym == SDLK_LCTRL)
	{
		CModel* newBox = new CModel(box);
		newBox->SetColor(255, 1, 1);
		newBox->SetPosition(0, 50, 0);
		boxList.push_back(newBox);
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

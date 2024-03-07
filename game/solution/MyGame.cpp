#include "MyGame.h"

using namespace std;

CMyGame::CMyGame(void) 
{
	score = 0;
}

CMyGame::~CMyGame(void) {}



// --------  game initialisation --------
void CMyGame::OnInitialize()
{
	// Loading graphics and sound assets
	cout << "Loading assets" << endl;
	font.LoadDefault();
	Light.Enable();

	// load player obj model from the models
	player.LoadModel("player.obj");
	player.SetScale(6.0f);

	// load box obj model from the models/box folder
	box.LoadModel("box/box.obj");
	box.SetScale(2.0f);

	// show floor grid
	floor.ShowGrid(true);

	//ShowBoundingBoxes();	
}

// Game Logic in the OnUpdate function called every frame

void CMyGame::OnUpdate()
{
	if (IsMenuMode() || IsGameOver()) return;

	long t = GetTime();

	// --- updating models ----
	box.Update(t);
	player.Update(t);

	for (CModel* m : boxList)
	{
		m->Update(t);
	}
	
    // My Control Functions
	PlayerControl();
	BoxControl();
}

void CMyGame::PlayerControl()
{
	// control the player movements

 // control the player movements

 // rotate player with RIGHT and LEFT arrow keys
	if (IsKeyDown(SDLK_RIGHT)) player.Rotate(0, -3, 0);
	if (IsKeyDown(SDLK_LEFT)) player.Rotate(0, 3, 0);

	// align direction with rotation
	player.SetDirectionV(player.GetRotationV());

	// move player forward when UP key is pressed
	if (IsKeyDown(SDLK_UP)) player.SetSpeed(300);
	else player.SetSpeed(0);

	
	// collision detection between player and boxes
	for (CModel* pModel : boxList)
	{
		if (player.HitTest(pModel))
		{
			pModel->Delete(); // mark for deletion
			score++;
		}
	}
	boxList.delete_if(deleted); // remove from list

}

void CMyGame::BoxControl()
{
	if (rand()%60==0)
	{
		CModel* newBox = box.Clone();

		//CModel* newBox = new CModel(box);

		newBox->SetPosition(750.0f - rand() % 1500, 50, 750.0f - rand() % 1500);
		newBox->SetOmega(50);
		boxList.push_back(newBox);
		
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
	box.Draw(g);
	player.Draw(g);

	for (CModel* m : boxList)
	{
		m->Draw(g);
	}

	//ShowBoundingBoxes(true);
	ShowCoordinateSystem();
	
}


void CMyGame::CameraControl(CGraphics* g)
{
	// Set View Transformations
	world.rotation.x = 45;     // tilt: rotation of game world around x-axis
	world.rotation.y = -45;    // rotation: rotation of game world around y-axis
	world.scale = 1.0f;	       // scaling the game world


	// ------ Global Transformation Functions -----
	glScalef(world.scale, world.scale, world.scale);  // scale the game world
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
	
	if (sym == SDLK_b)
	{
		CModel* newBox = box.Clone();
		newBox->SetPosition(750.0f - rand() % 1500, 50, 750.0f - rand() % 1500);
		boxList.push_back(newBox);
	}
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	
}

// -----  Mouse Events Handlers -------------

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
	//CVector pos=ScreenToFloorCoordinate(x,y);
	
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

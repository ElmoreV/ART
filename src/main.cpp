#define IMAGE
#include "SDLFramework.h"
#include "Player.h"
#include "Map.h"
#include "DrawingObject.h"
#include "Assets.h"
#include "Menu.h"
#include "Enemy.h" 
#include "GameState.h" 

class FPS
{
public:
	FPS(int fps):_fps(fps),_startTime(clock()){};
	void Delay()
	{
		int timeDiff=clock()-_startTime-2;
		if (timeDiff<1000/_fps)
		{
			::SDL_Delay((1000/_fps)-timeDiff);
		}
		_startTime=clock();
	}
private:
	int _fps;
	int _startTime;
};

int main( int argc, char* args[] ) 
{
	InitSDL();
	WindowSurface screen(800,600);

	//SDL_ShowCursor(0);
	long Timer = clock();//For Frame Independent Movement
	bool gameRunning=true;
	FPS fps(30);
	SDL_Event sEvent;

	Settings setting;
	Maps levels;
	GameStateManager gameStates(&setting);
	gameStates.NewState(GSIntro);


	Surface gameLogo;gameLogo.LoadImage("Images/titlescreen.png", 34, 177, 76);
	int introCount=0; float logoPositionY = 0;
	
	EnemyHandler enemies;
	Map map("Images/tilesheet.png", 50, 50, "Map/map1.txt");
	levels.count++;
	/*
	map.AddTile('x', 0, 50, 0, 50);
	map.AddTile('y', 100, 50, 50, 0);
	map.AddTile('a', 0, 0);
	map.AddTile('b', 100, 0);
	*/
	
	map.AddTile('A', 0, 0);
	map.AddTile('B', 50, 0);
	map.AddTile('C', 100, 0);
	map.AddTile('D', 0, 50);
	map.AddTile('E', 50, 50);
	map.AddTile('F', 100, 50);
	map.AddTile('G', 0, 150);
	map.AddTile('H', 50, 150);
	map.AddTile('I', 100, 150);
	map.AddTile('J', 0, 100);
	map.AddTile('K', 50, 100);
	map.AddTile('L', 100, 100);
	map.AddTile('M', 150, 100);
	map.AddTile('N', 150, 0, 50, 0);
	map.AddTile('O', 150, 50, 0, 50);
	map.AddTile('P', 200, 0, 50, 0);
	map.AddTile('Q', 200, 50, 0, 50);
	
	map.AddTile('a', 250, 50);
	map.AddTile('b', 300, 50);
	map.AddTile('c', 350, 50);
	map.AddTile('d', 250, 100);
	map.AddTile('e', 300, 100);
	map.AddTile('f', 350, 100);
	map.AddTile('g', 250, 150);
	map.AddTile('h', 300, 150);
	map.AddTile('i', 350, 150);
	map.AddTile('n', 400, 100, 50, 0);
	map.AddTile('o', 400, 150, 0, 50);
	map.AddTile('p', 400, 0, 50, 0);
	map.AddTile('q', 400, 50, 0, 50);
	map.AddTile('r', 250, 0);
	map.AddTile('s', 300, 0);
	map.AddTile('t', 350, 0);

	map.AddTile('w', 200, 150);
	map.AddTile('x', 150, 150, 0, 50);
	map.AddTile('y', 200, 100, 50, 0);
	map.AddTile('z', 65, 65, false, true);
	map.SetMaskColor(255, 242, 0);

	Player player("Images/Rat.png", map.GetSpawnLocation().X, map.GetSpawnLocation().Y, 3, 3, 2);
	player.SetVelocity(200, 400); //If Timer is set in draw of player (50 pixels per second) else (50pixels per frame)
	player.MaskColor(255, 242, 0);

	Menu menu("Main menu", &setting, 255, 255, 255);
	menu.ShowHeader(false);
	menu.SetCenter(true);
	menu.SetVerticalSpace(20);
	menu.AddButtonChild("New game", &Settings::OnClickNewGame);
	menu.AddButtonChild("Load game", &Settings::OnClickLoadGame);
	menu.AddChild("Options", true);
		menu.GetChild(2)->SetVerticalSpace(20);
		menu.GetChild(2)->AddChild("Graphics");
		menu.GetChild(2)->AddChild("Sounds");
		menu.GetChild(2)->AddChild("Keys");
	menu.AddButtonChild("Exit", &Settings::OnClickExitGame);

	Menu newLevelMenu("Level completed", &setting);
	newLevelMenu.AddChild("Congratulations");
	newLevelMenu.AddChild("");
	newLevelMenu.AddChild("What do you want to do?");
	std::vector<std::string> opt; opt.push_back("Main menu"); opt.push_back("Restart"); opt.push_back("Next level");
	newLevelMenu.AddOptionChild(opt, &Settings::NewLevelOptions, "       ");
	//((OptionMenuItem*)newLevelMenu.GetChild(3))->GetOption(1)->Enabled = false;

	while (gameRunning)
	{
		while (SDL_PollEvent(&sEvent))
		{
			if (sEvent.type==SDL_QUIT || setting.exitGame){gameRunning=false;}
			if (sEvent.type == SDL_VIDEORESIZE ) {screen.CreateWindowSurface( sEvent.resize.w,sEvent.resize.h);}

			switch(gameStates.CurrentState()){
				case GSMenuMain:
					if(sEvent.type == SDL_KEYDOWN) 
						if(sEvent.key.keysym.sym == SDLK_ESCAPE) 
						{gameStates.BackState();menu.Reset();}
					menu.HandleEvent(sEvent);
					break;
				case GSGame:
					if(sEvent.type == SDL_KEYDOWN) if(sEvent.key.keysym.sym == SDLK_ESCAPE) gameStates.PushState(GSMenuMain);
					player.HandleEvent(sEvent);
					map.HandleEvent(sEvent,player.GetBoundR(-map.GetMapPosition().X, -map.GetMapPosition().Y));
					break;
				case GSMenuNewLevel:
					newLevelMenu.HandleEvent(sEvent);
					break;
			}
		}
		if(gameStates.CurrentState() == GSNone) gameStates.PushState(GSMenuMain);
		screen.ClearWindow();
		switch(gameStates.CurrentState()){
			case GSIntro:
				if(introCount > 255){
					logoPositionY -= 5;
					if(logoPositionY <= 20){ logoPositionY = 20; gameStates.NewState(GSMenuMain); }
					gameLogo.Draw(screen, screen.GetWidth()/2 - gameLogo.GetWidth()/2, logoPositionY);
				}
				else {
					gameLogo.SetTransparency(introCount);
					logoPositionY = screen.GetHeight()/2 - gameLogo.GetHeight()/2;
					gameLogo.Draw(screen, screen.GetWidth()/2 - gameLogo.GetWidth()/2, logoPositionY);
				}
				introCount+=4;
				break;
			case GSMenuMain:
				if(setting.newGame){
					setting.newGame = false;
					levels.count = 1;
					map.NewMap(levels.levels[0]);
					enemies.PopulateEnemies(&map);
					player.SetPosition(map.GetSpawnLocation());
					gameStates.NewState(GSGame);
				}
				gameLogo.Draw(screen, screen.GetWidth()/2 - gameLogo.GetWidth()/2, logoPositionY);
				menu.Open(screen, Point2D(50, logoPositionY + gameLogo.GetHeight()));
				break;

			case GSGame:
				if(gameStates.CurrentState() != GSMenuNewLevel){
					if(map.NewMapEnabled(player.GetBoundR())) 
						gameStates.PushState(GSMenuNewLevel);
				}
				player.Update(map, screen.GetWidth(), screen.GetHeight(), Timer);
				enemies.Update(&map, &player, Timer);
			
				map.Draw(screen);
				enemies.Draw(screen, map.GetMapPosition());
				player.Draw(screen, map.GetMapPosition());
			
				break;

			case GSMenuNewLevel:
				if(setting.betweenLevelOptions == 0){
					newLevelMenu.Open(screen, Point2D(100, 100));
					if(levels.count == levels.maxCount)
						((OptionMenuItem*)newLevelMenu.GetChild(3))->GetOption(2)->Enabled = false;
					else
						((OptionMenuItem*)newLevelMenu.GetChild(3))->GetOption(2)->Enabled = true;
				}
				else {
					if(setting.betweenLevelOptions == 1) //MainMenu
						gameStates.NewState(GSMenuMain);
					else if(setting.betweenLevelOptions == 2){ //Restart 
						map.NewMap(levels.levels[levels.count-1]);
						enemies.PopulateEnemies(&map);
						player.SetPosition(map.GetSpawnLocation());
						gameStates.BackState();
					}
					else {//next level
						map.NewMap(levels.levels[levels.count]);
						enemies.PopulateEnemies(&map);
						levels.count++;
						player.SetPosition(map.GetSpawnLocation());
						gameStates.BackState();
					}
					setting.betweenLevelOptions = 0;
				}
				break;
		}

		Timer = clock(); //Set timer to last Update (For Frame Independent Movement)
		screen.UpdateWindow();
		fps.Delay();
	}
	player.Free();
	ClearSDL();
	return 0;
};

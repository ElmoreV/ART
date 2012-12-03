#define IMAGE
#include "SDLFramework.h"
#include "Player.h"
#include "Map.h"
#include "DrawingObject.h"
#include "Assets.h"
#include "Menu.h"
#include "Enemy.h" 
#include "GameState.h" 
#include "Assets.h"

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

	MusicHandler musicHandler;
	Graphics graphics;
	Sounds sounds;
	Settings setting;
	Maps levels;
	GlobalSettings gSettings;
	GameStateManager gameStates(&setting);

	Surface surface; surface.LoadImage("Images/icon.png", 255, 255, 0);
	SDL_WM_SetIcon(surface, NULL);
	screen.SetCaption("A Rat's Tail");

	//SDL_ShowCursor(0);
	long Timer = clock();//For Frame Independent Movement
	bool gameRunning=true;
	FPS fps(30);
	SDL_Event sEvent;

	sounds._titleScreen.InitIfNeccessary("Music/ratintro.ogg",128);
	sounds._titleScreen.SetLoopPosition(48300);
	sounds._titleScreen.SetVolumeModifier(0.4f);
	musicHandler.SetNewMusic(&sounds._titleScreen);
	
	gameStates.NewState(GSIntro);
	
	gSettings.SetVolume(100);
	gSettings._SfxMusicProportion=0.5;
	
	int introCount=0; float logoPositionY = 0;
	
	EnemyHandler enemies;
	Map map(&graphics._tileSheet, 50, 50, "Map/map1.txt");
	levels.count++;
	
	map.AddTile('A', 0, 0);
	map.AddTile('B', 50, 0);
	map.AddTile('C', 100, 0);
	map.AddTile('D', 0, 50);
	map.AddTile('E', 50, 50);
	map.AddTile('F', 100, 50);
	map.AddTile('G', 0, 100);
	map.AddTile('H', 50, 100);
	map.AddTile('I', 100, 100);
	map.AddTile('J', 0, 150);
	map.AddTile('K', 50, 150);
	map.AddTile('L', 100, 150);
	map.AddTile('M', 150, 0);
	map.AddTile('N', 150, 50);
	map.AddTile('O', 150, 100);

	map.AddTile('P', 200, 0);
	map.AddTile('Q', 250, 0);
	map.AddTile('R', 200, 50);
	map.AddTile('S', 250, 50);
	map.AddTile('T', 200, 100);
	map.AddTile('U', 250, 100);
	map.AddTile('V', 150, 150);
	map.AddTile('W', 200, 150);
	map.AddTile('X', 250, 150);
	map.AddTile('Y', 150, 200);
	map.AddTile('Z', 200, 200);
	map.AddTile('(', 250, 200);
	map.AddTile(')', 300, 200);

	map.AddTile('>', 300, 0, 50, 0);
	map.AddTile('<', 300, 50, 0, 50);
	map.AddTile('\\', 300, 100, 50, 0);
	map.AddTile('/', 300, 150, 0, 50);

	map.AddTile('a', 350, 50);
	map.AddTile('b', 400, 50);
	map.AddTile('c', 450, 50);
	map.AddTile('d', 350, 100);
	map.AddTile('e', 400, 100);
	map.AddTile('f', 450, 100);
	map.AddTile('g', 350, 150);
	map.AddTile('h', 400, 150);
	map.AddTile('i', 450, 150);
	map.AddTile('j', 350, 0);
	map.AddTile('k', 400, 0);
	map.AddTile('l', 450, 0);

	map.AddTile('w', 500, 0, 50, 0);
	map.AddTile('v', 500, 50, 0, 50);
	map.AddTile('x', 500, 100, 50, 0);
	map.AddTile('y', 500, 150, 0, 50);
	
	map.AddTile('m', 0, 200, 0, 50);
	map.AddTile('n', 50, 200);
	map.AddTile('o', 100, 200, 50, 0);
	
	map.AddTile('p', 350, 200, TStop);
	map.AddTile('q', 400, 200, TSleft);
	map.AddTile('r', 450, 200, TSright);
	map.AddTile('s', 500, 200, TSbottom);

	map.AddTile('z', 50, 250, false, true);
	map.AddTile('@', 0, 255);

	Player player(&graphics._player, map.GetSpawnLocation().X, map.GetSpawnLocation().Y, 3, 3, 2);
	player.SetVelocity(300, 400); //If Timer is set in draw of player (50 pixels per second) else (50pixels per frame)

	Menu menu("Main menu", &setting, 255, 255, 255);
	menu.ShowHeader(false);
	menu.SetCenter(true);
	menu.SetVerticalSpace(20);
	menu.AddButtonChild("New game",false,255,255,255, &Settings::OnClickNewGame);
	menu.AddButtonChild("Load game",false,255,255,255, &Settings::OnClickLoadGame);
	menu.AddChild("Options", true);
		menu.GetChild(2)->SetVerticalSpace(20);
		menu.GetChild(2)->AddChild("Graphics",true);
			menu.GetChild(2)->GetChild(0)->AddChild("FPS Setting");
			menu.GetChild(2)->GetChild(0)->AddChild("Resolution");
		menu.GetChild(2)->AddChild("Sounds",true);
			menu.GetChild(2)->GetChild(1)->AddChild("Master Volume");
			menu.GetChild(2)->GetChild(1)->AddSliderChild(256,10,true,155,155,155,&SetNewVolume,&gSettings);
			((SliderMenuItem*)(menu.GetChild(2)->GetChild(1)->GetChild(1)))->SetStatus((int)(gSettings._volume*256));
			menu.GetChild(2)->GetChild(1)->AddChild("SFX-Music");
			menu.GetChild(2)->GetChild(1)->AddSliderChild(256,10,true,30,200,30);
			((SliderMenuItem*)(menu.GetChild(2)->GetChild(1)->GetChild(3)))->SetStatus((int)(gSettings._SfxMusicProportion*256));
		menu.GetChild(2)->AddChild("Keys");
	menu.AddButtonChild("Exit",false,255,255,255, &Settings::OnClickExitGame);
	
	Menu newLevelMenu("Level completed", &setting);
	newLevelMenu.SetCenter(true);
	newLevelMenu.SetVerticalSpace(20);
	newLevelMenu.AddChild("Congratulations");
	newLevelMenu.AddChild("");
	newLevelMenu.AddChild("What do you want to do?");
	std::vector<std::string> opt; opt.push_back("Main menu"); opt.push_back("Restart"); opt.push_back("Next level");
	newLevelMenu.AddOptionChild(opt, "       ",false,255,255,255,&Settings::NewLevelOptions);

	//((OptionMenuItem*)newLevelMenu.GetChild(3))->GetOption(1)->Enabled = false;
	
	while (gameRunning)
	{
		while (SDL_PollEvent(&sEvent))
		{
			if (sEvent.type==SDL_QUIT || setting.exitGame){gameRunning=false;}
			if (sEvent.type == SDL_VIDEORESIZE ) {screen.CreateWindowSurface( sEvent.resize.w,sEvent.resize.h);}

			switch(gameStates.CurrentState())
			{
			case GSIntro:
				if (sEvent.type == SDL_KEYDOWN||sEvent.type == SDL_MOUSEBUTTONDOWN)
				{
					logoPositionY=20;
					introCount=256;
					graphics._gameLogo.SetTransparency(256);
				}
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
		if(clock() > Timer + 500) Timer = clock();
		if(gameStates.CurrentState() == GSNone) gameStates.PushState(GSMenuMain);
		screen.ClearWindow();
		musicHandler.SetGlobalVolume((int)(gSettings._volume*128));
		switch(gameStates.CurrentState())
		{
		case GSIntro:
			if(introCount > 255){
				logoPositionY -= 5;
				if(logoPositionY <= 20){ logoPositionY = 20; gameStates.NewState(GSMenuMain); }
				graphics._gameLogo.Draw(screen, screen.GetWidth()/2 - graphics._gameLogo.GetWidth()/2, (unsigned int)logoPositionY);
			}
			else {
				graphics._gameLogo.SetTransparency(introCount);
				logoPositionY = (float)screen.GetHeight()/2 - (float)graphics._gameLogo.GetHeight()/2;
				graphics._gameLogo.Draw(screen, screen.GetWidth()/2 - graphics._gameLogo.GetWidth()/2, (unsigned int)logoPositionY);
			}
			introCount+=2;
			break;
		case GSMenuMain:
			if(setting.newGame){
				musicHandler.SetNewMusic(&sounds._forest);
				setting.newGame = false;
				levels.count = 1;
				map.NewMap(levels.levels[0]);
				enemies.PopulateEnemies(&map, &graphics);
				player.SetPosition(map.GetSpawnLocation());
				gameStates.NewState(GSGame);
			}
			musicHandler.Update();
			graphics._gameLogo.Draw(screen, screen.GetWidth()/2 - graphics._gameLogo.GetWidth()/2, (unsigned int)logoPositionY);
			menu.Open(screen, graphics._menuFont, Point2D(50, logoPositionY + graphics._gameLogo.GetHeight()));
			break;

		case GSGame:
			if(gameStates.CurrentState() != GSMenuNewLevel){
				if(map.NewMapEnabled(player.GetBoundR())) 
					gameStates.PushState(GSMenuNewLevel);
			}
			player.Update(&map, screen.GetWidth(), screen.GetHeight(), Timer);
			enemies.Update(&map, &player, Timer);
			map.DrawBackground(screen, &graphics);
			map.Draw(screen);
			enemies.Draw(screen, map.GetMapPosition());
			player.Draw(screen, map.GetMapPosition());
			player.DrawHealthBar(screen, 1, 5, 5, 100, 20);
			player.DrawInkBar(screen, 1, 5, 35, 100, 20, &graphics._statFont);
			break;

		case GSMenuNewLevel:
			if(setting.betweenLevelOptions == 0){
				if(levels.count == levels.maxCount)
					((OptionMenuItem*)newLevelMenu.GetChild(3))->GetOption(2)->Enabled = false;
				else
					((OptionMenuItem*)newLevelMenu.GetChild(3))->GetOption(2)->Enabled = true;
				newLevelMenu.Open(screen, graphics._menuFont, Point2D(0, 100));
			}
			else {
				if(setting.betweenLevelOptions == 1) //MainMenu
				{
					gameStates.NewState(GSMenuMain);
					musicHandler.SetNewMusic(&sounds._titleScreen);
				}
				else if(setting.betweenLevelOptions == 2){ //Restart 
					map.NewMap(levels.levels[levels.count-1]);
					enemies.PopulateEnemies(&map, &graphics);
					player.Reset(map.GetSpawnLocation(), true);
					gameStates.BackState();
				}
				else {//next level
					map.NewMap(levels.levels[levels.count]);
					enemies.PopulateEnemies(&map, &graphics);
					levels.count++;
					player.SetPosition(map.GetSpawnLocation());
					gameStates.BackState();
					player.Reset(map.GetSpawnLocation(), false);
				}
				setting.betweenLevelOptions = 0;
			}
			break;
		}

		Timer = clock(); //Set timer to last Update (For Frame Independent Movement)
		screen.UpdateWindow();
		fps.Delay();
	}
	ClearSDL();
	return 0;
};

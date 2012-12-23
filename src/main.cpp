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
#include "Serialize.h"

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
	
	if (InitSDL())
	{
		WindowSurface screen(800,600);
		MusicHandler musicHandler;
		SoundEffectsHandler sfxHandler;
		Graphics graphics;
		Sounds sounds;
		Settings setting;
		Maps levels;
		GlobalSettings gSettings;
		GameStateManager gameStates(&setting);
		SettingSaverLoader ssl;
		ssl.LoadSettings(gSettings);
		long Timer = clock();//For Frame Independent Movement
		bool gameRunning=true;
		FPS fps(30);
		SDL_Event sEvent;
		musicHandler.SetNewMusic(&sounds.titleScreen);	
		sfxHandler.AddSoundEffect(&sounds.death);
		sfxHandler.AddSoundEffect(&sounds.bell);
		sfxHandler.AddSoundEffect(&sounds.damage);
		sfxHandler.AddSoundEffect(&sounds.enemyDeath);
		gameStates.NewState(GSIntro);

		int introCount=0; float logoPositionY = 0;

		Surface surface; surface.LoadImage("Images/icon.png", 255, 255, 0);
		SDL_WM_SetIcon(surface, NULL);
		screen.SetCaption("A Rat's Tale");

		EnemyHandler enemies;
		Map map(&graphics.tileSheet, 50, 50, "Map/map1.txt");
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
		
		map.AddTile('u', 200, 250, false, true);
		map.AddTile('z', 50, 250, false, true);
		map.AddTile('@', 150, 250);
		map.AddTile('!', 100, 250);

		Player player(&graphics.player, map.GetSpawnLocation().X, map.GetSpawnLocation().Y, 3, 3, 2);
		player.SetVelocity(200, 300); //If Timer is set in draw of player (50 pixels per second) else (50pixels per frame)
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
		menu.GetChild(2)->GetChild(1)->AddSliderChild(256,10,true,30,200,30,&SetNewProportion,&gSettings);
		((SliderMenuItem*)(menu.GetChild(2)->GetChild(1)->GetChild(3)))->SetStatus((int)(gSettings._sfxMusicProportion*256));
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

		Menu gameOverMenu("Game Over!", &setting);
		gameOverMenu.SetCenter(true);
		gameOverMenu.SetVerticalSpace(20);
		gameOverMenu.AddChild("Your journey has ended. You want to restart?");
		std::vector<std::string> opts; opts.push_back("Yes"); opts.push_back("No"); opts.push_back("Quit");
		gameOverMenu.AddOptionChild(opts, "    ", false, 255, 255, 255, &Settings::GameOverOptions);

		Menu inGameMenu("Pause",&setting,255,0,0);
		inGameMenu.SetCenter(true);
		inGameMenu.SetVerticalSpace(20);
		inGameMenu.AddButtonChild("Resume" ,true,255,255,255,&Settings::OnClickResume);
		inGameMenu.AddChild("Show statistics" ,true,255,255,255);
			inGameMenu.GetChild(1)->AddChild("Current level: ");
			inGameMenu.GetChild(1)->AddChild("Remaining Lives: ");
			inGameMenu.GetChild(1)->AddChild("Progression: ");
			inGameMenu.GetChild(1)->AddChild("Enemies killed: ");
			inGameMenu.GetChild(1)->AddChild("Game completion: ");
			inGameMenu.GetChild(1)->AddChild("Total distance drawn: ");
		inGameMenu.AddChild("Options", true);
			inGameMenu.GetChild(2)->SetVerticalSpace(20);
			inGameMenu.GetChild(2)->AddChild("Graphics",true);
			inGameMenu.GetChild(2)->GetChild(0)->AddChild("FPS Setting");
			inGameMenu.GetChild(2)->GetChild(0)->AddChild("Resolution");
			inGameMenu.GetChild(2)->AddChild("Sounds",true);
				inGameMenu.GetChild(2)->GetChild(1)->AddChild("Master Volume");
				inGameMenu.GetChild(2)->GetChild(1)->AddSliderChild(256,10,true,155,155,155,&SetNewVolume,&gSettings);
				((SliderMenuItem*)(menu.GetChild(2)->GetChild(1)->GetChild(1)))->SetStatus((int)(gSettings._volume*256));
				inGameMenu.GetChild(2)->GetChild(1)->AddChild("SFX-Music");
				inGameMenu.GetChild(2)->GetChild(1)->AddSliderChild(256,10,true,30,200,30,&SetNewProportion,&gSettings);
				((SliderMenuItem*)(menu.GetChild(2)->GetChild(1)->GetChild(3)))->SetStatus((int)(gSettings._sfxMusicProportion*256));
			inGameMenu.GetChild(2)->AddChild("Keys");
		inGameMenu.AddChild("Quit",true,255,0,0);
			inGameMenu.GetChild(3)->AddButtonChild("Exit to main menu",true,0,255,0, &Settings::OnClickNewGame);
			inGameMenu.GetChild(3)->AddButtonChild("Exit game",true,255,255,255, &Settings::OnClickExitGame);

		int NewLevelID = 0;
		int LevelCounter = 0;
		bool actionButtonPressed = false;
		//((OptionMenuItem*)newLevelMenu.GetChild(3))->GetOption(1)->Enabled = false;

		while (gameRunning)
		{
			while (SDL_PollEvent(&sEvent))
			{
				if (sEvent.type==SDL_QUIT || setting.GetResult() == MRExitGame){
					gameRunning=false;
					setting.Finish();
				}
				if (sEvent.type == SDL_VIDEORESIZE ) {screen.CreateWindowSurface( sEvent.resize.w,sEvent.resize.h);}
				switch(gameStates.CurrentState())
				{
				case GSIntro:
					if (sEvent.type == SDL_KEYDOWN||sEvent.type == SDL_MOUSEBUTTONDOWN)
					{
						logoPositionY=20;
						introCount=256;
						graphics.gameLogo.SetTransparency(256);
					}
				case GSMenuMain:
					if(sEvent.type == SDL_KEYDOWN){
						if(sEvent.key.keysym.sym == SDLK_ESCAPE) 
						{gameStates.BackState();menu.Reset();}
					}
						menu.HandleEvent(sEvent);
						break;
				case GSPause:
					{
						if(sEvent.type == SDL_KEYDOWN){
							if(sEvent.key.keysym.sym == SDLK_ESCAPE) 
							{gameStates.BackState();inGameMenu.Reset();}
						}
						inGameMenu.HandleEvent(sEvent);
						break;
					}
				case GSGame:
					if(sEvent.type == SDL_KEYDOWN) {
						if(sEvent.key.keysym.sym == SDLK_ESCAPE) {gameStates.PushState(GSPause);}
						else if(sEvent.key.keysym.sym == SDLK_s||sEvent.key.keysym.sym == SDLK_DOWN) {actionButtonPressed=true;}
					}
					if(sEvent.type == SDL_KEYUP){
						if(sEvent.key.keysym.sym == SDLK_s||sEvent.key.keysym.sym == SDLK_DOWN)
						{actionButtonPressed=false;}
					}
					player.HandleEvent(sEvent);
					map.HandleEvent(sEvent);
					break;
				case GSMenuNewLevel:
					newLevelMenu.HandleEvent(sEvent);
					break;
				case GSGame_Over:
					gameOverMenu.HandleEvent(sEvent);
					break;
				}
			}

			if(clock() > Timer + 500) Timer = clock();
			if(gameStates.CurrentState() == GSNone) gameStates.PushState(GSMenuMain);
			if(gameStates.CurrentState() == GSGame && player.Health <= 0)
			{
				gameStates.PushState(GSGame_Over);
				sounds.forest.SetVolumeModifier(0.1f);
				sounds.death.Play(false);
			}
			screen.ClearWindow();
			float musicPercentage=2*gSettings._sfxMusicProportion;
			if (musicPercentage>1.0){musicPercentage=1.0;}
			float sfxPercentage=2-2*gSettings._sfxMusicProportion;
			if (sfxPercentage>1.0){sfxPercentage=1.0;}
			musicHandler.SetGlobalVolume((int)(gSettings._volume*musicPercentage*128));
			sfxHandler.SetGlobalVolume((int)(gSettings._volume*sfxPercentage*128));
			switch(gameStates.CurrentState())
			{
			case GSIntro:
				if(introCount > 255){
					logoPositionY -= 5;
					if(logoPositionY <= 20){ logoPositionY = 20; gameStates.NewState(GSMenuMain); }
					Timer = clock();
					graphics.gameLogo.Draw(screen, screen.GetWidth()/2 - graphics.gameLogo.GetWidth()/2, (unsigned int)logoPositionY);
				}
				else {
					graphics.gameLogo.SetTransparency(introCount);
					logoPositionY = (float)screen.GetHeight()/2 - (float)graphics.gameLogo.GetHeight()/2;
					Timer = clock();
					graphics.gameLogo.Draw(screen, screen.GetWidth()/2 - graphics.gameLogo.GetWidth()/2, (unsigned int)logoPositionY);
				}
				introCount+=2;
				break;
			case GSMenuMain:
				if(setting.GetResult() == MRNewGame){
					musicHandler.SetNewMusic(&sounds.forest);
					levels.count = 0;
					LevelCounter = 0;
					map.Reset();
					map.NewMap(levels.levels[0]);
					enemies.PopulateEnemies(&map, &graphics);
					player.Reset(map.GetSpawnLocation(), true);
					gameStates.NewState(GSGame);
					setting.Finish();
				}else if (setting.GetResult()==MRLoadGame)
				{
					Loader load;
					if (load.StartAndCheck("Save1.sav"))
					{
						int newMapId;
						load.LoadCheckpoint(LevelCounter,newMapId);
						if(levels.maxCount <= newMapId || newMapId < 0)	newMapId = 0;
						levels.count = newMapId;
						load.LoadMap(map);
						load.LoadPlayer(player);
						load.Close();
						map.NewMap(levels.levels[newMapId]);
						enemies.PopulateEnemies(&map, &graphics);
						gameStates.NewState(GSGame);
						musicHandler.SetNewMusic(&sounds.forest);
						player.Reset(map.GetSpawnLocation(), false);
					}
					setting.Finish();
				}
				musicHandler.Update();
				Timer = clock();
				graphics.gameLogo.Draw(screen, screen.GetWidth()/2 - graphics.gameLogo.GetWidth()/2, (unsigned int)logoPositionY);
				menu.Open(screen, graphics.another, Point2D(50, logoPositionY + graphics.gameLogo.GetHeight()));
				break;
			case GSPause:
				{
					if(setting.GetResult() == MRResume){
						gameStates.BackState();
						inGameMenu.Reset();
						setting.Finish();
					}else if(setting.GetResult() == MRLoadGame)
					{
						Loader load;
						if (load.StartAndCheck())
						if (load.StartAndCheck("Save1.sav"))
						{
							int newMapId;
							load.LoadCheckpoint(LevelCounter,newMapId);
							if(levels.maxCount <= newMapId || newMapId < 0)	newMapId = 0;
							levels.count = newMapId;
							load.LoadMap(map);
							load.LoadPlayer(player);
							load.Close();
							map.NewMap(levels.levels[newMapId]);
							enemies.PopulateEnemies(&map, &graphics);
							gameStates.NewState(GSGame);
							musicHandler.SetNewMusic(&sounds.forest);
							player.Reset(map.GetSpawnLocation(), false);
						}
						setting.Finish();
					}else if (setting.GetResult() == MRNewGame)
					{
						gameStates.NewState(GSMenuMain);
						musicHandler.SetNewMusic(&sounds.titleScreen);
						setting.Finish();

					}
					
					musicHandler.Update();

					Timer = clock();
					map.DrawBackground(screen, &graphics);
					map.Draw(screen, LevelCounter);
					player.DrawHealthBar(screen, 1, 5, 5, 100, 20, &graphics.another);
					player.DrawInkBar(screen, 1, 5, 35, 100, 20, &graphics.another);
					inGameMenu.Open(screen, graphics.another, Point2D(0, 50));
					break;
				}

			case GSGame:
				if(gameStates.CurrentState() != GSMenuNewLevel && actionButtonPressed){
					if(map.NewMapEnabled(player.GetBoundR(), NewLevelID))
						gameStates.PushState(GSMenuNewLevel);
					actionButtonPressed=false;
				}
				player.Update(&map, screen.GetWidth(), screen.GetHeight(),sounds, Timer);
				enemies.Update(&map, &player,sounds, Timer);
				map.Update(player.GetBoundR(-map.GetMapPosition().X, -map.GetMapPosition().Y),player.InkPool);
				musicHandler.Update();
				Timer = clock();
				map.DrawBackground(screen, &graphics);
				map.Draw(screen, LevelCounter);
				enemies.Draw(screen, map.GetMapPosition());
				player.Draw(screen, map.GetMapPosition());
				player.DrawHealthBar(screen, 1, 5, 5, 100, 20, &graphics.another);
				player.DrawInkBar(screen, 1, 5, 35, 100, 20, &graphics.another);
				break;

			case GSMenuNewLevel:{
 				if(levels.count > LevelCounter) LevelCounter++;
				if(NewLevelID < 0) levels.count++;
				else if(NewLevelID >= levels.maxCount) levels.count = levels.maxCount-1;
				else levels.count = NewLevelID;

				map.NewMap(levels.levels[levels.count]);
				enemies.PopulateEnemies(&map, &graphics);
				player.Reset(map.GetSpawnLocation(), false);
				gameStates.BackState();
				sounds.bell.Play(false);
				Saver saver;
				saver.StartAndOpen();
				saver.SaveCheckpoint(LevelCounter,levels.count);
				saver.SaveMap(map);
				saver.SavePlayer(player);
				saver.EndAndClose("Save1.sav");
				break;
								}
			case GSGame_Over:
				musicHandler.Update();
				Timer = clock();
				gameOverMenu.Open(screen, graphics.another, Point2D(0, 50));
				if(setting.GetResult() != MRExitGame && setting.GetResult() != MRNone) {
					if(setting.GetResult() == MRMainMenu)
					{
						musicHandler.SetNewMusic(&sounds.titleScreen);
						sounds.forest.SetVolumeModifier(0.4f);
						gameStates.NewState(GSMenuMain);
					}
					else if(setting.GetResult() == MRNewGame){
						levels.count = 1;
						map.NewMap(levels.levels[0]);
						enemies.PopulateEnemies(&map, &graphics);
						player.Reset(map.GetSpawnLocation(), false);
						player.Health  =(float) 100;
						player.InvulnerableTime = 0;
						sounds.forest.SetVolumeModifier(0.4f);
						gameStates.NewState(GSGame);
					}
					setting.Finish();
				}
				break;
			}
			screen.UpdateWindow();
			fps.Delay();
		}
	}
	ClearSDL();
	return 0;
};

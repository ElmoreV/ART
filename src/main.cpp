#define IMAGE
#include "SDLFramework.h"
#include "Player.h"
#include "Map.h"
#include "DrawingObject.h"
#include "Assets.h"
#include "Menu.h"
#include "Tail.h"

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
	Map map1("Images/tilesheet.png", 100, 50, "Map/map1.txt");

	map1.AddTile('x', 0, 50, 0, 50);
	map1.AddTile('y', 100, 50, 50, 0);
	map1.AddTile('a', 0, 0);
	map1.AddTile('b', 100, 0);
	map1.AddTile('z', 65, 65, false, true);
	map1.SetMaskColor(255, 242, 0);

	Player player("Images/Rat.png", map1.GetSpawnLocation().X, map1.GetSpawnLocation().Y, 3, 3, 2);
	player.SetVelocity(100, 250); //If Timer is set in draw of player (50 pixels per second) else (50pixels per frame)
	player.MaskColor(255, 242, 0);
	Tail tail;
	Settings setting;

	std::vector<std::string> option; option.push_back("Ja"); option.push_back("Nee"); option.push_back("Ooit");option.push_back("Misschien");

	Menu menu("Menu", &setting);
	menu.AddChild("Hallo", 255, 5, 255);
	menu.GetChild(0)->AddChild("hallo", 255, 0, 0);
	menu.GetChild(0)->AddOptionChild(option, &Settings::OnOptionClick);
	menu.AddChild("Cool");
	menu.AddButtonChild("Wat", &Settings::OnClick);
	menu.AddOptionChild(option, &Settings::OnOptionClick);
	menu.AddTextChild(&Settings::OnTextChange, "Welkom", 10, true, 50, 50, 255);
	menu.AddSliderChild(&Settings::OnValueChange, 150, 30, 100, 100, 100);
	menu.AddButtonChild("h)llo");

	/* If using array for map
		const char* map1Array[] = {
		"x-x-x-x-x-x", 
		"yxyxyxyxyxy"};
	*/
	while (gameRunning)
	{
		while (SDL_PollEvent(&sEvent))
		{
			if (sEvent.type==SDL_QUIT){gameRunning=false;}
			if (sEvent.type == SDL_VIDEORESIZE )
			{screen.CreateWindowSurface( sEvent.resize.w,sEvent.resize.h);}
			player.HandleEvent(sEvent);
			map1.HandleEvent(sEvent,player.GetBoundR(-map1.GetMapPosition().X, -map1.GetMapPosition().Y));
			menu.HandleEvent(sEvent);
			tail.HandleEvent(sEvent);
		}
		//IF a new map can be load
		if(map1.NewMapEnabled()){
			map1.NewMap("Map/map1.txt");
			player.SetPosition(map1.GetSpawnLocation());
		}
		player.Update(map1, screen.GetWidth(), screen.GetHeight(), Timer);
		tail.Update(player.GetBoundR(-map1.GetMapPosition().X, -map1.GetMapPosition().Y),player.GetHorizontalDir());
		screen.ClearWindow();
		map1.Draw(screen);
		player.Draw(screen, map1.GetMapPosition());
		tail.Draw(screen);
		Rectangle playerBounds=player.GetBoundR(-map1.GetMapPosition().X, -map1.GetMapPosition().Y);
		::aaellipseRGBA(screen,(Sint16)(playerBounds.X+0.5*playerBounds.W),(Sint16)(playerBounds.Y+0.5*playerBounds.H),100,100,255,255,255,255);
		menu.Open(screen, Point2D(50, 50));		
		Timer = clock(); //Set timer to last Update (For Frame Independent Movement)
		screen.UpdateWindow();
		fps.Delay();
	}
	player.Free();
	ClearSDL();
	return 0;
}

#define IMAGE
#include "SDLFramework.h"
#include "Object.h"
#include "map.h"
#include "Drawing.h"

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
	DrawingObject drawer(400,400);
	Map map1("Images/tilesheet.png", 64, 36, "Map/map.txt");
	//normal blocks
	map1.AddTile('x', 0, 0, 0, 36);
	map1.AddTile('y', 64, 0, 36, 0);
	//slopes
	map1.AddTile('a', 0, 36);
	map1.AddTile('b', 64, 36);
	map1.AddTile('c', 0, 72, 0, 18);
	map1.AddTile('d', 64, 72, 19, 36);
	map1.AddTile('e', 0, 108, 36, 19);
	map1.AddTile('f', 64, 108, 18, 0);

	Player player("Images/player.png", 0, 0, 3, 3, 4);
	player.SetVelocity(100, 250); //If Timer is set in draw of player (50 pixels per second) else (50pixels per frame)
	player.MaskColor(120, 195, 128);

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
			player.HandleEvent(sEvent);
			drawer.HandleEvent(sEvent);
		}
		player.Update(map1, Timer);
		screen.ClearWindow();
		map1.Draw(screen);
		drawer.Draw(screen);
		player.Draw(screen);
		Timer = clock(); //Set timer to last Update (For Frame Independent Movement)

		screen.UpdateWindow();
		fps.Delay();
	}
	player.Free();
	//ball2.Free();
	ClearSDL();
	return 0;
}

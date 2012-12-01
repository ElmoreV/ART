#ifndef ASSETS_H
#define ASSETS_H
#include "SDLFramework.h"

class Graphics
{
public:
	~Graphics()
	{
		_tileSheet.Free();
		_player.Free();
	};
	//Window surface
	WindowSurface _screen;
	//Normal surfaces
	Surface _tileSheet;
	Surface _player;
	//Fonts
};
class Sounds
{
public:
	~Sounds(){_titleScreen.Free();};
	//Music
	Music _titleScreen;
	//Sound effects
};
class Maps
{
public:
	std::string levels[4];
	int count;
	int maxCount;
	Maps(){
		count = 0;
		maxCount = 4;
		levels[0] = "Map/map1.txt";
		levels[1] = "Map/map2.txt";
		levels[2] = "Map/map3.txt";
		levels[3] = "Map/map4.txt";
	};

};

#endif
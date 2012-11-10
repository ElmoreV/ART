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
	~Sounds();
	//Music
	//Sound effects
};

#endif
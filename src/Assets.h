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
		_gameLogo.Free();
		for(int i = 0; i < 3; i++)
			_air[i].Free();
		for(int i =0; i<4; i++)
			_forest[i].Free();
		_space1.Free();
		_space2.Free();
	};
	Graphics(){
		_tileSheet.LoadImage("Images/tilesheet.png", 255, 242, 0);
		_gameLogo.LoadImage("Images/titlescreen.png", 34, 177, 76);
		_player.LoadImage("Images/Rat.png",255, 242, 0);
		_air[0].LoadImage("Images/background/air1.png");
		_air[1].LoadImage("Images/background/air2.png");
		_air[2].LoadImage("Images/background/air3.png");
		_forest[0].LoadImage("Images/background/forest1.png");
		_forest[1].LoadImage("Images/background/forest2.png");
		_forest[2].LoadImage("Images/background/forest3.png");
		_forest[3].LoadImage("Images/background/forest4.png");
		_space1.LoadImage("Images/background/space1.png");
		_space2.LoadImage("Images/background/space2.png");
		_enemy[0].LoadImage("Images/enemy1.png", 0, 0, 0);
		_enemy[1].LoadImage("Images/enemy2.png", 255, 242, 0);
		_enemy[2].LoadImage("Images/enemy3.png", 0, 0, 0);
		_enemy[3].LoadImage("Images/enemy4.png", 181, 230, 29);
		_statFont.OpenFont("Another.ttf");
		_menuFont = _statFont;

	}
	//Normal surfaces
	Surface _tileSheet;
	Surface _player;
	Surface _gameLogo;
	Surface _air[3];
	Surface _forest[4];
	Surface _space1;
	Surface _space2;
	Surface _enemy[4];
	//Fonts
	Font _menuFont;
	Font _statFont;
};
class Sounds
{
public:
	~Sounds(){_titleScreen.Free();_forest.Free();};
	//Music
	Music _titleScreen;
	Music _forest;
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
#ifndef ASSETS_H
#define ASSETS_H
#include "SDLFramework.h"

class Graphics
{
public:
	~Graphics()
	{
		tileSheet.Free();
		player.Free();
		gameLogo.Free();
		for(int i = 0; i < 3; i++)
			air[i].Free();
		for(int i =0; i<4; i++){
			forest[i].Free();
			enemy[i].Free();
		}
		space1.Free();
		space2.Free();
		another.Free();
	};
	Graphics(){

		tileSheet.LoadImage("Images/tilesheet.png", 255, 242, 0);
		gameLogo.LoadImage("Images/titlescreen.png", 34, 177, 76);
		player.LoadImage("Images/Rat.png",255, 242, 0);
		air[0].LoadImage("Images/background/air1.png");
		air[1].LoadImage("Images/background/air2.png");
		air[2].LoadImage("Images/background/air3.png");
		forest[0].LoadImage("Images/background/forest1.png");
		forest[1].LoadImage("Images/background/forest2.png");
		forest[2].LoadImage("Images/background/forest3.png");
		forest[3].LoadImage("Images/background/forest4.png");
		space1.LoadImage("Images/background/space1.png");
		space2.LoadImage("Images/background/space2.png");
		enemy[0].LoadImage("Images/enemy1.png", 0, 0, 0);
		enemy[1].LoadImage("Images/enemy2.png", 255, 242, 0);
		enemy[2].LoadImage("Images/enemy3.png", 0, 0, 0);
		enemy[3].LoadImage("Images/enemy4.png", 181, 230, 29);
		another.OpenFont("Another.ttf");
	}
	//Normal surfaces
	Surface tileSheet;
	Surface player;
	Surface gameLogo;
	Surface air[3];
	Surface forest[4];
	Surface space1;
	Surface space2;
	Surface enemy[4];
	//Fonts
	Font another;
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
	std::string levels[6];
	int count;
	int maxCount;
	Maps(){
		count = 0;
		maxCount = 6;
		levels[0] = "Map/map0.txt";
		levels[1] = "Map/map1.txt";
		levels[2] = "Map/map2.txt";
		levels[3] = "Map/map3.txt";
		levels[4] = "Map/map4.txt";
		levels[5] = "Map/map5.txt";
	};

};

#endif
#ifndef ASSETS_H
#define ASSETS_H
#include "SDLFramework.h"

class Graphics
{
public:
	//Frees all the graphics
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
		forestParallax.Free();
	};
	//Initializes all the graphics
	Graphics(){

		tileSheet.LoadImage("Images/tilesheet.png", 255, 242, 0);
		gameLogo.LoadImage("Images/titlescreen.png", 34, 177, 76);
		player.LoadImage("Images/Rat.png",255, 242, 0);
		air[0].LoadImage("Images/background/air1.png");
		air[1].LoadImage("Images/background/air2.png");
		air[2].LoadImage("Images/background/air3.png");
		forest[0].LoadImage("Images/background/forest1.png",176,244,132);
		forest[1].LoadImage("Images/background/forest2.png",176,244,132);
		forest[2].LoadImage("Images/background/forest3.png",176,244,132);
		forest[3].LoadImage("Images/background/forest4.png",176,244,132);
		space1.LoadImage("Images/background/space1.png");
		space2.LoadImage("Images/background/space2.png");
		enemy[0].LoadImage("Images/enemy1.png", 0, 0, 0);
		enemy[1].LoadImage("Images/enemy2.png", 255, 242, 0);
		enemy[2].LoadImage("Images/enemy3.png", 0, 0, 0);
		enemy[3].LoadImage("Images/enemy4.png", 181, 230, 29);
		another.OpenFont("Another.ttf");
		forestParallax.LoadImage("Images/background/parallaxforest.png");
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
	Surface forestParallax;
	//Fonts
	Font another;
};
class Sounds
{
public:
	//Initializes all the sounds
	Sounds()
	{
		titleScreen.InitIfNeccessary("Music/aratsburrow.ogg",128);
		titleScreen.SetLoopPosition(48300);
		titleScreen.SetVolumeModifier(0.6f);
		forest.InitIfNeccessary("Music/amouseadventure.ogg" ,128);
		forest.SetVolumeModifier(0.4f);
		death.LoadSoundEffect("SFX/deathbysnusnu.wav");
		bell.LoadSoundEffect("SFX/bell.wav");
		damage.LoadSoundEffect("SFX/damage.wav");
		enemyDeath.LoadSoundEffect("SFX/enemydeath.wav");
	}
	//Frees all the sounds
	~Sounds(){titleScreen.Free();forest.Free();death.Free();};
	//Music
	Music titleScreen;
	Music forest;
	//Sound effects
	SoundEffect death;
	SoundEffect bell;
	SoundEffect damage;
	SoundEffect enemyDeath;
};
class Maps
{
public:
	std::string levels[8];//The filenames for the levels
	int count;//The starting level
	int maxCount; //The maximum of levels
	//Initializes all the maps
	Maps(){
		count = 0;
		maxCount = 8;
		levels[0] = "Map/map0.txt";
		levels[1] = "Map/map1.txt";
		levels[2] = "Map/map2.txt";
		levels[3] = "Map/map3.txt";
		levels[4] = "Map/map4.txt";
		levels[5] = "Map/map5.txt";
		levels[6] = "Map/map6.txt";
		levels[7] = "Map/map7.txt";
	};

};

#endif
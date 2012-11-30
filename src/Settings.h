#ifndef SETTINGS_H
#define SETTINGS_H
#include "SDLFramework.h"

class Settings
{
public:
	Settings(){
		betweenLevelOptions = 0;
		newGame = false;
		loadGame = false;
		exitGame = false;
	}
	int betweenLevelOptions;
	bool newGame;
	bool loadGame;
	bool exitGame;
	bool NewLevelOptions(int id){
		betweenLevelOptions = id+1;
		return false;
	}
	bool OnClickNewGame(){ newGame=true;return false; }
	bool OnClickLoadGame(){loadGame=true;return false;}
	bool OnClickExitGame(){exitGame=true;return false;}
};
#endif
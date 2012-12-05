#ifndef SETTINGS_H
#define SETTINGS_H
#include "SDLFramework.h"

class SettingSaverLoader;

class GlobalSettings
{
public:
	bool SetVolume(float percentage);
	float _volume;
	float _SfxMusicProportion;
};
static bool SetNewVolume(float percentage,void* globalSettings)
{return ((GlobalSettings*)globalSettings)->SetVolume(percentage);};
enum MenuResult{
	MRNewGame, MRLoadGame, MRExitGame, MRMainMenu, MRRestart, MRNone
};
class Settings
{
private :
	MenuResult _result;
public:
	Settings(){
		_result = MRNone;
	}
	bool NewLevelOptions(int id){
		if(id == 0)_result = MRMainMenu;
		else if(id == 1)_result = MRRestart;
		else _result = MRNewGame;
		return false;
	}
	bool GameOverOptions(int id){
		if(id == 0)_result = MRNewGame;
		else if(id == 1)_result = MRMainMenu;
		else _result = MRExitGame;
		return false;
	}
	bool OnClickNewGame(){ _result = MRNewGame; return false; }
	bool OnClickLoadGame(){_result = MRLoadGame; return false;}
	bool OnClickExitGame(){
		_result = MRExitGame; 
		return false;
	}
	MenuResult GetResult(){
		return _result;
	}
	void Finish(){_result = MRNone; }
};
#endif
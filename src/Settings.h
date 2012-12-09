#ifndef SETTINGS_H
#define SETTINGS_H
#include "SDLFramework.h"

class SettingSaverLoader;

class GlobalSettings
{
public:
	GlobalSettings()
	{_volume=1.0f;_sfxMusicProportion=0.5f;}
	bool SetVolume(float ratio);
	bool SetVolumeProportion(float proportion);
	float _volume;
	float _sfxMusicProportion;
};
static bool SetNewVolume(float ratio,void* globalSettings)
{return ((GlobalSettings*)globalSettings)->SetVolume(ratio);};
static bool SetNewProportion(float proportion,void* globalSettings)
{return ((GlobalSettings*)globalSettings)->SetVolumeProportion(proportion);};
enum MenuResult{
	MRNewGame, MRLoadGame, MRExitGame, MRMainMenu, MRRestart,MRResume, MRNone
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
	bool OnClickResume(){_result=MRResume; return false;}
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
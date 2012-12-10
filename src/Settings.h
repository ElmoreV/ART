#ifndef SETTINGS_H
#define SETTINGS_H
#include "SDLFramework.h"

class SettingSaverLoader;

class GlobalSettings
{
public:
	//Initializes the settings to default settings
	GlobalSettings()
	{_volume=1.0f;_sfxMusicProportion=0.5f;}
	//Sets the volume to a ratio between 0.0 and 1.0 (meaning 1.0=full volume and 0.0=no volume)
	bool SetVolume(float ratio);
	//Set volume and soundeffects proportion between 0.0 and 1.0
	//1.0= all music, and no sound effects
	//0.0= sound effects on full volume, no volume on music
	//0.5= equal volume on both sound effects and music
	bool SetVolumeProportion(float proportion);
	float _volume;
	float _sfxMusicProportion;
};
//Callback function for the 'Master Volume'  slider in the menu
//Calls the appropriate function in the GlobalSettings function
static bool SetNewVolume(float ratio,void* globalSettings)
{return ((GlobalSettings*)globalSettings)->SetVolume(ratio);};
//Callback function for the proportion slide in the menu
static bool SetNewProportion(float proportion,void* globalSettings)
{return ((GlobalSettings*)globalSettings)->SetVolumeProportion(proportion);};

//Different types of items that can be clicked
enum MenuResult{
	MRNewGame, MRLoadGame, MRExitGame, MRMainMenu, MRRestart,MRResume, MRNone
};
class Settings
{
private :
	MenuResult _result;//Storing what is clicked/the action to be taken/the result of user input
public:
	//Constructor: sets the clicked value to 0
	Settings(){
		_result = MRNone;
	}
	//If the user clicks on buttons on the next level screen, the menu returns an id to this function
	//This function sets the result to the appropriate action
	bool NewLevelOptions(int id){
		if(id == 0)_result = MRMainMenu;
		else if(id == 1)_result = MRRestart;
		else _result = MRNewGame;
		return false;
	}
	//If the user clicks on buttons on the game over screen, the menu returns an id to this function
	//This function sets the result to the appropriate action
	bool GameOverOptions(int id){
		if(id == 0)_result = MRNewGame;
		else if(id == 1)_result = MRMainMenu;
		else _result = MRExitGame;
		return false;
	}
	//If 'New Game' is clicked
	bool OnClickNewGame(){ _result = MRNewGame; return false; }
	//If 'Load Game'  is clicked
	bool OnClickLoadGame(){_result = MRLoadGame; return false;}
	//If ' Resume'  is clicked
	bool OnClickResume(){_result=MRResume; return false;}
	//If ' Exit'  is clicked
	bool OnClickExitGame(){
		_result = MRExitGame; 
		return false;
	}
	//Returns the result/action to be done/what is clicked
	MenuResult GetResult(){
		return _result;
	}
	//Resets the clicked value
	void Finish(){_result = MRNone; }
};
#endif
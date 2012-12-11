#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <vector>
#include "Settings.h"
//Enumeration with all the possible gamestates that are used within the game
enum GameStates
{
	GSNone,
	GSIntro,
	GSMenuMain,
	GSMenuNewLevel,
	GSGame,
	GSGame_Over,
	GSPause
};
//The manager of the gamestate, which will handle gamestates, and substates
class GameStateManager
{
private:
	std::vector<GameStates> _states; //List of all gamestates that are currently active
	//Event handler, that will be called if pointer != 0 and if the topgamestate changes
	void (Settings::*onStateChange)(GameStates previous, GameStates current); 
	//Pointer to the settings class, necessary for the event handler
	Settings* _setting;
public:
	//The simple constructor
	GameStateManager(Settings* setting);
	//The simple constructor, which also sets the eventhandler, so it can be called if state changes
	GameStateManager(Settings* setting, void (Settings::*onStateChange)(GameStates previous, GameStates current));
	//Removes all the gamestates from the list
	void CleanUp();
	//Adds a new gamestate to the list
	void PushState(GameStates state);
	//Remove the topstate from the statelist
	//The amount of states to remove can be set in the parameter
	void BackState(unsigned int count = 1);
	//Removes all the gamestates in the list, and add a new gamestate to the list
	void NewState(GameStates state);
	//Returns the top gamestate of the list
	GameStates CurrentState();
	//Returns the gamestate at the parameter position
	GameStates StateAt(unsigned int index);
	//Changes the eventhandler
	void SetOnStateChange(void (Settings::*onStateChange)(GameStates previous, GameStates current));
};
#endif
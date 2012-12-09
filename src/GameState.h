#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <vector>
#include "Settings.h"
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
class GameStateManager
{
private:
	std::vector<GameStates> _states;
	void (Settings::*onStateChange)(GameStates previous, GameStates current);
	Settings* _setting;
public:
	GameStateManager(Settings* setting);
	GameStateManager(Settings* setting, void (Settings::*onStateChange)(GameStates previous, GameStates current));
	void CleanUp();
	void PushState(GameStates state);
	void BackState(unsigned int count = 1);
	void NewState(GameStates state);
	GameStates CurrentState();
	GameStates StateAt(unsigned int index);
	void SetOnStateChange(void (Settings::*onStateChange)(GameStates previous, GameStates current));
};
#endif
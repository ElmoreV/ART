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
	void BackState(int count = 1);
	void NewState(GameStates state);
	GameStates CurrentState();
	GameStates StateAt(int index);
	void SetOnStateChange(void (Settings::*onStateChange)(GameStates previous, GameStates current));
};
#endif
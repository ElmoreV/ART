#include "GameState.h"

GameStateManager::GameStateManager(Settings* setting){
	_eventSet = false;
	_setting = setting;
}
GameStateManager::GameStateManager(Settings* setting, void (Settings::*onstatechange)(GameStates previous, GameStates current)){
	_eventSet = true;
	onStateChange = onstatechange;
	_setting = setting;
}
void GameStateManager::CleanUp(){
	if(_eventSet)
		(_setting->*this->onStateChange)(CurrentState(), GSNone);
	_states.clear();
}
void GameStateManager::PushState(GameStates state){
	if(_eventSet)
		(_setting->*this->onStateChange)(CurrentState(), state);
	_states.push_back(state);
}
void GameStateManager::BackState(int count){
	if(_states.size() < count || count < 0){
		CleanUp();
	}
	else 
	{
		for(int i = 0; i < count; i++){
			if(_eventSet)
				(_setting->*this->onStateChange)(CurrentState(), (_states.size() > 1) ? _states.at(_states.size() - 2) : GSNone);
			_states.pop_back();
		}
	}
}
void GameStateManager::NewState(GameStates state){
	if(_eventSet)
		(_setting->*this->onStateChange)(CurrentState(), state);
	_states.clear();
	_states.push_back(state);
}
GameStates GameStateManager::CurrentState(){
	return (_states.size() > 0) ? _states.at(_states.size() - 1) : GSNone;
}
GameStates GameStateManager::StateAt(int index){
	if(index < _states.size())
		return _states.at(index);
	else return GSNone;
}
void GameStateManager::SetOnStateChange(void (Settings::*onstatechange)(GameStates previous, GameStates current)){
	onStateChange = onstatechange;
	_eventSet = true;
}
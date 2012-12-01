#include "GameState.h"

GameStateManager::GameStateManager(Settings* setting){
	_setting = setting;
	onStateChange=0;
}
GameStateManager::GameStateManager(Settings* setting, void (Settings::*onstatechange)(GameStates previous, GameStates current)){
	onStateChange = onstatechange;
	_setting = setting;
}
void GameStateManager::CleanUp(){
	if(onStateChange!=0){
		(_setting->*this->onStateChange)(CurrentState(), GSNone);
	}
	_states.clear();
}
void GameStateManager::PushState(GameStates state){
	if(onStateChange!=0){
		(_setting->*this->onStateChange)(CurrentState(), state);
	}
	_states.push_back(state);
}
void GameStateManager::BackState(unsigned int count){
	if(_states.size() <= count){
		CleanUp();
	}
	else 
	{
		for(unsigned int i = 0; i < count; i++){
			if(onStateChange!=0){
				(_setting->*this->onStateChange)(CurrentState(), (_states.size() > 1) ? _states.at(_states.size() - 2) : GSNone);
			}
			_states.pop_back();
		}
	}
}
void GameStateManager::NewState(GameStates state){
	if(onStateChange!=0){
		(_setting->*this->onStateChange)(CurrentState(), state);
	}
	_states.clear();
	_states.push_back(state);
}
GameStates GameStateManager::CurrentState(){
	return (_states.size() > 0) ? _states.at(_states.size() - 1) : GSNone;
}
GameStates GameStateManager::StateAt(unsigned int index){
	if(index < _states.size())
		return _states.at(index);
	else return GSNone;
}
void GameStateManager::SetOnStateChange(void (Settings::*onstatechange)(GameStates previous, GameStates current)){
	onStateChange = onstatechange;
}
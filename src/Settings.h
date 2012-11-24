#ifndef SETTINGS_H
#define SETTINGS_H
#include "SDLFramework.h"

class Settings
{
public:
	bool OnClick(){
		return true;
	}
	bool OnOptionClick(int id){
		return false;
	}
};
#endif
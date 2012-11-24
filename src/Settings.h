#ifndef SETTINGS_H
#define SETTINGS_H
#include "SDLFramework.h"
#include <sstream>

class Settings
{
public:
	bool OnClick(){
		return true;
	}
	bool OnOptionClick(int id){
		Error r; std::stringstream ss;
		ss << id;
		r.HandleError(Caption, ss.str());
		return false;
	}
	bool OnTextChange(std::string text){
		
		return false;
	}
};
#endif
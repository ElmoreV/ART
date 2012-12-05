#include "Settings.h"
#include "Serialize.h"

bool GlobalSettings::SetVolume(float percentage)
{
	if (percentage>1.0f){percentage=1.0f;}
	if (percentage<0.0f){percentage=0.0f;}
	_volume=percentage;
	SettingSaverLoader ssl;
	ssl.SaveSettings(*this);
	return true;
};
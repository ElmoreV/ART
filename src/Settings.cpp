#include "Settings.h"
#include "Serialize.h"

bool GlobalSettings::SetVolume(float ratio)
{
	if (ratio>1.0f){ratio=1.0f;}
	if (ratio<0.0f){ratio=0.0f;}
	_volume=ratio;
	SettingSaverLoader ssl;
	ssl.SaveSettings(*this);
	return true;
};
bool GlobalSettings::SetVolumeProportion(float proportion)
{
	if (proportion>1.0f){proportion=1.0f;}
	if (proportion<0.0f){proportion=0.0f;}
	_sfxMusicProportion=proportion;
	SettingSaverLoader ssl;
	ssl.SaveSettings(*this);
	return true;
};
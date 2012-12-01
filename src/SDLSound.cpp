#include "SDLSound.h"
#include "SDLFramework.h"
#include <ctime>

Music::Music():_music(0),_position(0),_playing(false),_volume(128),_loopPosition(0)
{}
Music::Music(std::string filename):_music(0),_position(0),_playing(false),_volume(128),_loopPosition(0)
{LoadMusic(filename);}
void Music::Free()
{
	if (_music!=0)
	{::Mix_FreeMusic(_music);_music=0;}
}
bool Music::LoadMusic(std::string filename)
{
	Free();
	_music=::Mix_LoadMUS(filename.c_str());
	if (_music== 0)
	{
		//Handle Error
		std::string str="The following music file could not be found: ";
		str+=filename;
		Error error(Caption,str.c_str(),1,true);
		return false;
	}
	return true;
}
bool Music::IsInit()
{return (_music!=0);}
bool Music::InitIfNeccessary(std::string filename, int volume)
{
	if (!IsInit())
	{
		bool retVal=LoadMusic(filename);
		if (volume!=-1)
		{
			Volume(volume);
		}
		return retVal;
	}
	return true;
};

void Music::Volume(int volume)
{
	if (volume>128){volume=128;}else if (volume<0){volume=0;}
	_volume=volume;
	if (_playing)
	{Mix_VolumeMusic((int)((float)_volume*_volumeModifier));}
}

bool Music::SetPosition(int position)
{
	_position=position;
	return (Mix_SetMusicPosition((double)position/1000.0)==0);
}

bool Music::Play(int position,int fadeInMilliseconds)
{
	bool alreadyFading=(::Mix_FadingMusic()==MIX_FADING_OUT);
	if (::Mix_PlayingMusic()==false||alreadyFading)
	{
		if (alreadyFading)
		{::Mix_HaltMusic();}

		if (fadeInMilliseconds>0)
		{Mix_FadeInMusic(_music, 1, fadeInMilliseconds);}
		else
		{::Mix_PlayMusic(_music,1);}
		_playing=true;
		Mix_VolumeMusic(_volume);
		_position=0;
		_startTime=clock();
		if (position>0)
		{
			if (SetPosition(position))
			{
				_position+=position;
				_startTime-=position;
			}
		}
		
		return true;
	}
	return false;
};
bool Music::Continue(int fadeIn)
{
	return Play(_position,fadeIn);
}
bool Music::Stop(int fadeOut)
{
	if (_playing)
	{
		if (fadeOut>0)
		{
			Mix_FadeOutMusic(fadeOut);
		}else
		{
			Mix_HaltMusic();
		}
		_position=clock()-_startTime;
		_playing=false;
		return true;
	}
	return false;
}
bool Music::StopStart(int fade)
{
	if (Mix_PlayingMusic())
	{
		return Stop(fade);
	}else
	{
		return Play(fade);
	}
}
void Music::SetLoopPosition(int loopPositionMilliseconds)
{_loopPosition=loopPositionMilliseconds;}
void Music::SetVolumeModifier(float modifier)
{
	if (modifier>1.0f){modifier=1.0f;}
	if (modifier<0.0f){modifier=0.0f;}
	_volumeModifier=modifier;
Volume(_volume);
}
bool Music::Loop()
{
	if (_playing==true&&(::Mix_PlayingMusic()==false||::Mix_FadingMusic()==MIX_FADING_OUT))
	{
		Play(_loopPosition,400);
		return true;
	}
	return false;
};


MusicHandler::MusicHandler()
	{
	_currentMusic=0;_prevMusic=0;_volume=128;
	}
	void MusicHandler::SetNewMusic(Music* newMusic)
	{
		if (newMusic==0){return;}
		
			_prevMusic=_currentMusic;
			_currentMusic=newMusic;
		if (_prevMusic!=0)
		{_prevMusic->Stop();}
		if (newMusic->IsInit())
		{	
			_currentMusic->Continue();
			_currentMusic->Volume(_volume);
			
		}
		return;
	}
	void MusicHandler::Update()
	{

		if (_currentMusic!=0)
		{_currentMusic->Loop();}
	}
	void MusicHandler::Stop()
	{
		if (_currentMusic!=0)
		{_currentMusic->Stop(true);}
	}
	void MusicHandler::SetGlobalVolume(int volume)
	{
		_volume=volume;
		if (_currentMusic!=0)
		{_currentMusic->Volume(volume);}
	}
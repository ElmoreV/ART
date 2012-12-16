#include "SDLSound.h"
#include "SDLFramework.h"
#include <ctime>

SoundEffect::SoundEffect():_sfx(0),_volumeModifier(1.0f),_volume(128),_channelId(-1)
{};
SoundEffect::SoundEffect(std::string filename)
{
	SoundEffect();
	LoadSoundEffect(filename);
};
void SoundEffect::SetChannelId(int id=-1)
{_channelId=id;}
void SoundEffect::Free()
{
	if (_sfx!=0)
	{
		::Mix_FreeChunk(_sfx);
		_sfx=0;
	}
};
bool SoundEffect::LoadSoundEffect(std::string filename)
{
	Free();
	_sfx=::Mix_LoadWAV(filename.c_str());
	if (_sfx== 0)
	{
		//Handle Error
		Error error(Caption,"The following sound effect file could not be found: "+filename,1,true);
		return false;
	}
	return true;
};
bool SoundEffect::IsInit()
{return (_sfx!=0);}
bool SoundEffect::InitIfNeccessary(std::string filename, int volume)
{
	if (!IsInit())
	{LoadSoundEffect(filename);
	Volume(volume);}
};
void SoundEffect::Play(bool loop=false)
{
	if (IsInit())
	{::Mix_PlayChannel(_channelId,_sfx,loop?-1:0);}
};
void SoundEffect::Stop()
{
	if (IsInit())
	{::Mix_HaltChannel(_channelId);}
}
void SoundEffect::Volume(int volume)
{
	if (volume>128){volume=128;}else if (volume<0){volume=0;}
		_volume=volume;
	if (IsInit())
	{
		Mix_VolumeChunk(_sfx,(int)((float)_volume*_volumeModifier));
		Mix_Volume(_channelId,(int)((float)_volume*_volumeModifier));
	}
}
void SoundEffect::SetVolumeModifier(float volumeModifier)
{
	_volumeModifier=volumeModifier;
	Volume(_volume);
}





Music::Music():_music(0),_position(0),_playing(false),_volume(128),_loopPosition(0)
{}
Music::Music(std::string filename)
{
	Music();
	LoadMusic(filename);
}
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
		Error error(Caption,"The following music file could not be found: "+filename,1,true);
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
	bool playingSomething=(bool)(::Mix_PlayingMusic()!=0?true:false)|(::Mix_FadingMusic()==MIX_FADING_OUT)|(::Mix_FadingMusic()==MIX_FADING_IN);
	if (playingSomething)
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
		_currentMusic->Play();
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

SoundEffectsHandler::SoundEffectsHandler(){memset(&_sfx,0,16*sizeof(_sfx[0]));_volume=128;};
void SoundEffectsHandler::AddSoundEffect(SoundEffect* newSoundEffect)
{
	if (newSoundEffect!=0)
	{
		for (unsigned i=0;i<16;i++)
		{
			if (!_sfx[i])
			{
				_sfx[i]=newSoundEffect;
				_sfx[i]->SetChannelId(i);
				_sfx[i]->Volume(_volume);
				break;
			}
		};
	}
};
void SoundEffectsHandler::StopAll()
{
	for (unsigned i=0;i<16;i++)
	{
		if (_sfx[i])
		{_sfx[i]->Stop();}
	};
}
void SoundEffectsHandler::SetGlobalVolume(int volume)
{
	_volume=volume;
	for (unsigned i=0;i<16;i++)
	{
		if (_sfx[i])
		{_sfx[i]->Volume(_volume);}
	}
};
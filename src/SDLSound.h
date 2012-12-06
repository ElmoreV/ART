#ifndef SOUND_H
#define SOUND_H
#include <string>

#include <SDL\\SDL_mixer.h>
#pragma comment(lib,"SDL_mixer.lib")

class SoundEffect
{
public:
	SoundEffect();
	SoundEffect(std::string filename);
	void Free();
	bool LoadSoundEffect(std::string filename);
	bool IsInit();
	bool InitIfNeccessary(std::string filename, int volume=128);
	void Play(bool loop);
	void Stop();
	void SetChannelId(int id);
	void SetVolumeModifier(float volumeModifier);
	void Volume(int volume);
private:
	Mix_Chunk* _sfx;
	int _channelId;
	float _volumeModifier;
	int _volume;
};

class Music
{
public:
	Music();
	Music(std::string filename);
	void Free();
	bool LoadMusic(std::string filename);
	bool IsInit();
	bool InitIfNeccessary(std::string filename, int volume=128);
	void Volume(int volume);
	bool SetPosition(int position);
	bool Play(int position=0,int fadeInMilliseconds=1000);
	bool Continue(int fadeIn=1000);
	bool Stop(int fadeOut=1000);
	bool StopStart(int fade=1000);
	bool Loop();
	void SetLoopPosition(int loopPosition);
	void SetVolumeModifier(float modifier);
private:
	Mix_Music* _music;
	int _volume;
	float _volumeModifier;
	bool _playing;
	int _startTime;
	int _position;
	int _loopPosition;

};


class SoundEffectsHandler
{
public:
	SoundEffectsHandler();
	void AddSoundEffect(SoundEffect* newSoundEffect);
	void StopAll();
	void SetGlobalVolume(int volume);
private:
	SoundEffect* _sfx[16];
	int _volume;
};

class MusicHandler
{
public:
	MusicHandler();
	void SetNewMusic(Music* newMusic);
	void Update();
	void Stop();
	void SetGlobalVolume(int volume);
private:
	Music* _currentMusic;
	Music* _prevMusic;
	int _volume;
};

#endif
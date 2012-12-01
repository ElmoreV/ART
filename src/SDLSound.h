#ifndef SOUND_H
#define SOUND_H
#include <string>

#include <SDL\\SDL_mixer.h>
#pragma comment(lib,"SDL_mixer.lib")





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
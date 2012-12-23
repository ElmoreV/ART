#ifndef SOUND_H
#define SOUND_H
#include <string>

#include <SDL\\SDL_mixer.h>
#pragma comment(lib,"SDL_mixer.lib")

class SoundEffect
{
public:
	//Initializes the sound effect
	SoundEffect();
	//Initialize and load the sound effect
	SoundEffect(std::string filename);
	//Free the resource
	void Free();
	//Load the sound effect from a file
	bool LoadSoundEffect(std::string filename);
	//Check if the sfx is initialized
	bool IsInit();
	//If the sfx is not yet initialized, initialize it
	bool InitIfNeccessary(std::string filename, int volume=128);
	//Play the sound effect
	//(Loop it if loop=true)
	void Play(bool loop);
	//Stop the sound effect
	void Stop();
	//Select on which channel the sound effect should be played 
	void SetChannelId(int id);
	//Set the modifier for this particular sound effect
	void SetVolumeModifier(float volumeModifier);
	void Volume(int volume);
private:
	Mix_Chunk* _sfx; //A pointer to the object used natively by SDL_mixer
	int _channelId; //The id of the channel to be used. Is -1 when unspecified
	float _volumeModifier;//The volume modifier. Is multiplied by the volume
	int _volume; //The volume, can be changed for all songs at once by the handler
};

class Music
{
public:
	//Music constructors
	Music();
	//Immediately loads the music
	Music(std::string filename);
	//Free the music resources, if any
	void Free();
	//Load the music from a file
	bool LoadMusic(std::string filename);
	//check wether the music file is initialized
	bool IsInit();
	//If it isn't initialized, initialize it
	bool InitIfNeccessary(std::string filename, float volumeModifier=1.0);
	//Set the volume between 128 (full) and 0(no)
	void Volume(int volume);
	//Set the position in milliseconds
	bool SetPosition(int position);
	//Play the music, starting at position (in milliseconds) and fade in for the specified time
	bool Play(int position=0,int fadeInMilliseconds=1000);
	//Continue from where is was stoppen
	bool Continue(int fadeIn=1000);
	//Stop the music
	bool Stop(int fadeOut=1000);
	//If it was stopped, start. If it plays, stop it
	bool StopStart(int fade=1000);
	//Check if the music is stopped, and then loop is (starting at the loopposition)
	bool Loop();
	//Set the position where the loop must begin
	void SetLoopPosition(int loopPosition);
	//Set the volume modifier for this particular song
	void SetVolumeModifier(float modifier);
private:
	Mix_Music* _music;//A pointer to the music object natively used by SDL_mixer
	float _volumeModifier;//The volume modifier. Is multiplied by the volume
	int _volume; //The volume, can be changed for all songs at once by the handler
	bool _playing; //True: this music is playing. false= this music isn't playing
	int _startTime; //used for determining the position
	int _position; //The position where it is started/ended (used for continue)
	int _loopPosition; //The position to start when looping and the music had ended

};


class SoundEffectsHandler
{
public:
	SoundEffectsHandler();
	//Add a sound effect to the list
	void AddSoundEffect(SoundEffect* newSoundEffect);
	//Stop all the playing sound effect
	void StopAll();
	//Set the volume of all the sfx at once
	void SetGlobalVolume(int volume);
private:
	SoundEffect* _sfx[16]; //16 different sound effect object (each their own channel)
	int _volume; //The global sfx volume
};

class MusicHandler
{
public:
	MusicHandler();
	//Set the music for the next level/gamestate
	//Stop the previous musc
	//Play the new music from the beginning
	void SetNewMusic(Music* newMusic);
	//Check if the music needs looping
	void Update();
	//Stop the music
	void Stop();
	//Set the music volume between 128 (full) and 0(no)
	void SetGlobalVolume(int volume);
private:
	Music* _currentMusic;//pointer to the current music.
	Music* _prevMusic; //pointer to the previous music
	int _volume;//global music volume
};

#endif
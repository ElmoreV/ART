#ifndef SERIALIZE_H
#define SERIALIZE_H
#include <string>
#include <fstream>
#include "Settings.h"
#include "Player.h"
#include "Map.h"


class Serializer
{
public:
	Serializer();
	Serializer(std::string const & filename,bool newFile);
	~Serializer();
	void OpenStream(std::string const & filename,bool newFile);
	void SetByte(unsigned char byte);
	void SetUnsignedInt(unsigned int ui);
	void SetFloat(float f);
	void SetString(std::string str);
	std::ofstream _ostream;
};

class DeSerializer
{
public:	
	DeSerializer();
	DeSerializer(std::string const & filename);
	~DeSerializer();
	bool OpenStream(std::string const & filename);
	unsigned int GetUnsignedInt();
	float GetFloat();
	char GetChar();
	unsigned char GetByte();
	std::string GetString(int len);
	unsigned int GetSize();
	std::ifstream _istream;	
private:
	unsigned int _size;
};

class SettingSaverLoader
{
public:
	void SaveSettings(GlobalSettings& gSettings);
	void LoadSettings(GlobalSettings& gSettings);
};

class Saver
{
public:
	void StartAndOpen();
	void SavePlayer(Player& player);
	void SaveMap(Map& map);
	void SaveCheckpoint(int previousMapId,int nextMapId);
	void EndAndClose(std::string filename);
private:
	unsigned int _checkSum;
	Serializer _serial;
};
class Loader
{
public:
	bool StartAndCheck(std::string filename);
	void LoadPlayer(Player& player);
	void LoadMap(Map& map);
	void LoadCheckpoint(int& previousMapId,int& nextMapId);
	void Close();
private:
	DeSerializer _deserial;
};


#endif
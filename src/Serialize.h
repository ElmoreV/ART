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
	//Constructors of the serializer, which is a intermediate class to handle saving different types to a file more easily
	Serializer();
	//Creates and uses OpenStream
	Serializer(std::string const & filename,bool newFile);
	~Serializer();
	//Opens a (write-only) file stream to save data.
	//Filename is the file to create/write to
	//newFile: true when a new file must be created, 
	//newFile:false= no new file will be created, and already existing will not be overwritten, rather appended
	void OpenStream(std::string const & filename,bool newFile);
	//Sets different types, one byte, unsigned int (4 bytes), float(4 bytes) and string (variable) respectively
	void SetByte(unsigned char byte);
	void SetUnsignedInt(unsigned int ui);
	void SetFloat(float f);
	void SetString(std::string str);
	std::ofstream _ostream;//The actual filestream as defined in <fstream>
};

class DeSerializer
{
public:	
	//Constructors of the deserializer, which is a intermediate class to handle loading different types from a file more easily
	DeSerializer();
	//Creates and uses OpenStream
	DeSerializer(std::string const & filename);
	~DeSerializer();
	//Opens a (read-only) file stream to save data.
	//Filename is the file to read from
	//returns true when the file exists and is opened
	//return false when the file could not be opened
	bool OpenStream(std::string const & filename);
	//Gets different types, one byte, unsigned int (4 bytes), float(4 bytes) and string (of size 'len') respectively
	unsigned int GetUnsignedInt();
	float GetFloat();
	char GetChar();
	unsigned char GetByte();
	std::string GetString(int len);
	//Gets the file size
	unsigned int GetSize();
	std::ifstream _istream;//The actual filestream as defined in <fstream>
private:
	unsigned int _size;//Stores the file size
};

class SettingSaverLoader
{
public:
	//Saves the global settings into an 8 byte file ("Settings.sav")
	void SaveSettings(GlobalSettings& gSettings);
	//Loads the settings from the settings file ("Settings.sav")
	void LoadSettings(GlobalSettings& gSettings);
};

class Saver
{
public:
	//Open a temporary file to save to
	void StartAndOpen();


	//Save the important player attributes (health, total ink received) and add it to the checksum as unsigned int
	void SavePlayer(Player& player);
	//Save the important map attributes (total distance drawn)and add it to the checksum as unsigned int
	void SaveMap(Map& map);
	//Save the current and next level and add it to the checksum as unsigned int
	void SaveCheckpoint(int previousMapId,int nextMapId);
	//Adds the checksum to the end of file, and uses a fail-safe way to replace the previous safe file with the current.
	//And close the filestream
	void EndAndClose(std::string filename);
private:
	unsigned int _checkSum;//The checksum to remember
	Serializer _serial;
};
class Loader
{
public:
	//Checks if the checksum is valid, and the file is open
	//If that is not the case, return false.
	//If that IS the case, return true
	bool StartAndCheck(std::string filename);
	//Load the important things of the player
	void LoadPlayer(Player& player);
	//Load the important things of the map
	void LoadMap(Map& map);
	//Load the previous and next map id
	void LoadCheckpoint(int& previousMapId,int& nextMapId);
	//Close the loading file stream
	void Close();
private:
	DeSerializer _deserial;
};


#endif
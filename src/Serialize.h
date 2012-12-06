#ifndef SERIALIZE_H
#define SERIALIZE_H
#include <string>
#include <fstream>
#include "Settings.h"

class Serializer
{
public:
	Serializer(std::string const & filename,bool newFile)
		: _ostream(filename.c_str(),std::ios_base::out|std::ios_base::binary|(newFile?std::ios_base::trunc:1))
	{
		if (!_ostream.is_open())
		{}
	}
	~Serializer()
	{_ostream.flush();_ostream.clear();_ostream.close(); }
	void SetByte(unsigned char byte)
	{
		_ostream.write((char*)&byte,1);
	}
	void SetUnsignedInt(unsigned int ui)
	{
		_ostream.write((char*)&ui,4);
	};
	void SetFloat(float f)
	{
		_ostream.write((char*)&f,4);
	};
	void SetString(std::string str)
	{
		_ostream.write(&str[0],str.length());
	}
std::ofstream _ostream;
};

class DeSerializer
{
public:	
	DeSerializer(std::string const & filename)
		: _istream(filename.c_str(),std::ios_base::in|std::ios_base::binary)
	{
		if (!_istream.is_open())
		{}
	}
	~DeSerializer()
	{
		_istream.clear();
		_istream.close();
	}
	unsigned int GetUnsignedInt()
	{
		unsigned int ui=0;
		_istream.read((char*)&ui,4);
		return ui;
	}
	float GetFloat()
	{
		float f=0;
		_istream.read((char*)&f,4);
		return f;
	}
	char GetChar()
	{
		if (_istream.eof())
		{}
		char c=0;
		{_istream.get(c);}
		return c;
	}

	unsigned char GetByte()
	{
		unsigned char byte=0;
		{byte=_istream.get();}
		return byte;
	}
	std::string GetString(int len)
	{
		std::string str;
		str.resize(len+1);
		{_istream.read(&str[0],len);}
		return str;
	}	
	std::ifstream _istream;	
};

class SettingSaverLoader
{
public:
	void SaveSettings(GlobalSettings& gSettings)
	{
		Serializer saver("Settings.sav",true);
		saver.SetFloat(gSettings._volume);
		saver.SetFloat(gSettings._sfxMusicProportion);

	};
	void LoadSettings(GlobalSettings& gSettings)
	{
		DeSerializer loader("Settings.sav");
		if (loader._istream.is_open())
		{
			gSettings._volume=loader.GetFloat();
			gSettings._sfxMusicProportion=loader.GetFloat();
		}
	};
};

class Saver
{

};
class Loader{};


#endif
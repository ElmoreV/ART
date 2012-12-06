#include "Serialize.h"

Serializer::Serializer(std::string const & filename,bool newFile)
{OpenStream(filename,newFile);}
Serializer::Serializer()
{}
Serializer::~Serializer()
{_ostream.flush();_ostream.clear();_ostream.close(); }
void Serializer::OpenStream(std::string const & filename,bool newFile)
{
	_ostream.open(filename.c_str(),std::ios_base::out|std::ios_base::binary|(newFile?std::ios_base::trunc:1));
	if (!_ostream.is_open())
	{
		Error error(Caption,"Could not open file for saving:"+filename,5);
	}
}
void Serializer::SetByte(unsigned char byte)
{
	if (!_ostream.bad())
	{_ostream.write((char*)&byte,1);}
}
void Serializer::SetUnsignedInt(unsigned int ui)
{
	if (!_ostream.bad())
	{_ostream.write((char*)&ui,4);}
};
void Serializer::SetFloat(float f)
{
	if (!_ostream.bad())
	{_ostream.write((char*)&f,4);}
};
void Serializer::SetString(std::string str)
{
	if (!_ostream.bad())
	{_ostream.write(&str[0],str.length());}
}
DeSerializer::DeSerializer():_size(0)
{}
DeSerializer::DeSerializer(std::string const & filename)
{OpenStream(filename);}
void DeSerializer::OpenStream(std::string const & filename)
{
	if (_istream.is_open()){_istream.close();}
	_istream.open(filename.c_str(),std::ios_base::in|std::ios_base::binary);
	if (_istream.is_open())
	{
		 _istream.seekg (0, std::ios::end);
		_size = (int)_istream.tellg();
		 _istream.seekg (0, std::ios::beg);
	}else{_size=0;}
}
DeSerializer::~DeSerializer()
{
	_istream.clear();
	_istream.close();
}
unsigned int DeSerializer::GetSize()
{return _size;}
unsigned int DeSerializer::GetUnsignedInt()
{
	unsigned int ui=0;
	_istream.read((char*)&ui,4);
	return ui;
}
float DeSerializer::GetFloat()
{
	float f=0;
	_istream.read((char*)&f,4);
	return f;
}
char DeSerializer::GetChar()
{
	char c=0;
	{_istream.get(c);}
	return c;
}

unsigned char DeSerializer::GetByte()
{
	unsigned char byte=0;
	{byte=_istream.get();}
	return byte;
}
std::string DeSerializer::GetString(int len)
{
	std::string str;
	str.resize(len+1);
	{_istream.read(&str[0],len);}
	return str;
}	

void SettingSaverLoader::SaveSettings(GlobalSettings& gSettings)
{
	Serializer saver("Settings.sav",true);
	saver.SetFloat(gSettings._volume);
	saver.SetFloat(gSettings._sfxMusicProportion);

};
void SettingSaverLoader::LoadSettings(GlobalSettings& gSettings)
{
	DeSerializer loader("Settings.sav");
	if (loader._istream.is_open())
	{
		gSettings._volume=loader.GetFloat();
		gSettings._sfxMusicProportion=loader.GetFloat();
	}
};

void Saver::StartAndOpen()
{
	_serial.OpenStream("tempSave.sav",true);
	_checkSum=0;
};
void Saver::SavePlayer(Player& player)
{
	_serial.SetFloat(player.InkPool);
	_checkSum+=*(unsigned int*)&(player.InkPool);
	_serial.SetFloat(player.Health);
	_checkSum+=*(unsigned int*)&(player.Health);
};
void Saver::SaveCheckpoint(int previousMapId,int nextMapId)
{
	_serial.SetUnsignedInt(previousMapId);
	_checkSum+=previousMapId;
	_serial.SetUnsignedInt(nextMapId);
	_checkSum+=nextMapId;
};
void Saver::EndAndClose(std::string filename)
{
	_serial.SetUnsignedInt(_checkSum);
	_serial._ostream.close();
	std::ifstream saveFileExists(filename.c_str(),std::ios::in);
	if (saveFileExists.is_open())
	{
		saveFileExists.close();
		rename(filename.c_str(),"tempOldSave.sav");
		rename("tempSave.sav",filename.c_str());
		remove("tempOldSave.sav");
	}else
	{
		rename("tempSave.sav",filename.c_str());
	}
};
void Loader::StartAndCheck(std::string filename)
{
	_deserial.OpenStream(filename);
	unsigned int testChecksum=0;
	unsigned int size=_deserial.GetSize();
	while(size>4)
	{
		testChecksum+=_deserial.GetUnsignedInt();
		size-=4;
	}
	if (testChecksum==_deserial.GetUnsignedInt())
	{
		_deserial.OpenStream(filename);
		//Correct
	}else
	{
		Error error(Caption,"Trying to load unvalid file:" + filename,5);
	}
};
void Loader::LoadPlayer(Player& player)
{
	player.InkPool=_deserial.GetFloat();
	player.Health=_deserial.GetFloat();
};
void Loader::LoadCheckpoint(int& previousMapId,int& nextMapId)
{
	previousMapId=_deserial.GetUnsignedInt();
	nextMapId=_deserial.GetUnsignedInt();
};
void Loader::Close()
{
	_deserial._istream.close();
};
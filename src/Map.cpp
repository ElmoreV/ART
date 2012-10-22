#include "map.h"
TileData::TileData(int x, int y, int width, int height){
	X=x;Y=y;Width=width;Height=height;_isSlope=false;
}
TileData::TileData(int x, int y, int width, int height, int slopeleft, int sloperight){
	X=x;Y=y;Width=width;Height=height;_isSlope=true;_slopeLeft=slopeleft;_slopeRight=sloperight;
}
SDL_Rect TileData::Rect(){
	SDL_Rect value =  {X, Y, Width, Height};
	return value;
}
bool TileData::IsSlope(){return _isSlope; }
void TileData::GetSlope(int& y1, int& y2){ y1=_slopeLeft; y2=_slopeRight; }


//Initialize map (leave string map empty if using array)
Map::Map(std::string tileSheet, unsigned int tileWidth, unsigned int tileHeight, std::string map)
{
	_tileSheet.LoadImage(tileSheet.c_str(), 255, 255, 255); //TileSheet
	_tileDimension.X = (float)tileWidth; //Width of a tile
	_tileDimension.Y = (float)tileHeight; //Height of a tile
	if(map != "") ReadFile(map);
	_spawnLocation = '#';
}
//Read the file and push lines into vector
bool Map::ReadFile(std::string filename)
{
	_lines=0;
	std::ifstream streamer;
	streamer.open(filename.c_str());
	if(!streamer) return false;
	std::string line;
	while(!streamer.eof())
	{
		std::getline(streamer, line);
		_mapArray.push_back(line);
	}
	return true;
}
//Adds a new normal tile to the dictionary
bool Map::AddTile(char key, int x, int y){
	TileData value(x, y, (int)_tileDimension.X, (int)_tileDimension.Y);
	//SDL_Rect value = {x, y, width, height};
	Dictionary::iterator it = _tileLibrary.lower_bound(key);
	if(it != _tileLibrary.end() && !(_tileLibrary.key_comp()(key, it->first))) { return false; } //Key already exist
	else {_tileLibrary.insert(std::pair<char, TileData>(key, value));return true;} //Key dont exists

}
//Adds a new slopetile to the dictionary
bool Map::AddTile(char key, int x, int y, int slopeLeft, int slopeRight){
	TileData value(x, y, (int)_tileDimension.X, (int)_tileDimension.Y, slopeLeft, slopeRight);
	//SDL_Rect value = {x, y, width, height};
	Dictionary::iterator it = _tileLibrary.lower_bound(key);
	if(it != _tileLibrary.end() && !(_tileLibrary.key_comp()(key, it->first))) { return false; } //Key already exist
	else {_tileLibrary.insert(std::pair<char, TileData>(key, value));return true;} //Key dont exists

}
//Draws the excisting map on the screen
void Map::Draw(SDL_Surface* screen)
{
	const char* charline;
	for(unsigned int y = 0; y < _mapArray.size(); y++)
	{
		charline = _mapArray.at(y).c_str();
		for(unsigned int x = 0; x < strlen(charline); x++)
		{
			if(_tileLibrary.count(charline[x]) != 0)
			{
				TileData td = _tileLibrary.find(charline[x])->second;
				SDL_Rect clip = td.Rect();
				SDL_Rect offset = {(Uint16)(x * _tileDimension.X), (Uint16)(y * _tileDimension.Y), clip.w, clip.h};
				SDL_BlitSurface(_tileSheet, &clip, screen, &offset);
			}
		}
	}
}
//Draws a new map
void Map::Draw(SDL_Surface* screen,const char* mapArray[], unsigned int aantalRijen)
{
	const char* charline;
	unsigned int y = 0;
	for(unsigned int y = 0; y < aantalRijen; y++)
	{
		charline = mapArray[y];
		for(unsigned int x = 0; x < strlen(charline); x++)
		{
			if(_tileLibrary.count(charline[x]) != 0)
			{
				TileData td = _tileLibrary.find(charline[x])->second;
				SDL_Rect clip = td.Rect();
				SDL_Rect offset = {(Uint16)(x * _tileDimension.X), (Uint16)(y * _tileDimension.Y), clip.w, clip.h};
				SDL_BlitSurface(_tileSheet, &clip, screen, &offset);
			}
		}
	}
}
//Get the type of collision of the inserten point
int Map::GetCharType(Point2D collisionPoint){
	//0 = outofrange
	//1 = none
	//2 = normal
	//3 = slope
	const char* charline;
	if(_mapArray.size() > collisionPoint.Y){
		charline = _mapArray.at((int)collisionPoint.Y).c_str();
		if(strlen(charline) > collisionPoint.X){
			if(charline[(int)collisionPoint.X] == '-') return 1;
			else {
				TileData td = _tileLibrary.find(charline[(int)collisionPoint.X])->second;
				if(td.IsSlope()) return 3;
				return 2;
			}
		}
	}
	return 0;
}
TileData Map::GetTileData(unsigned int x, unsigned int y){
	TileData td(0, 0, 0, 0);
	if(_mapArray.size()>y){
		const char* charline = _mapArray.at(y).c_str();
		td = _tileLibrary.find(charline[x])->second;
	}
	return td;

}

//Returns the dimensions of a single tile
Point2D Map::GetTileDimension() { return _tileDimension; }
//Returns the position of the map which is displayed
Point2D Map::GetMapPosition() { return _mapPosition; }
//Returns the height of empty pixels @ the position
float Map::GetHeightAtPosition(Point2D position){
	//std::stringstream ss;
	int y = (int)(position.Y / _tileDimension.Y);
	int x = (int)(position.X / _tileDimension.X);
	float height = 0;
	//ss << x << "  " << y << "  " << position.X << "  " << position.Y;
	int charType = GetCharType(Point2D((float)x, (float)y));
	if(charType == 2) return 0; //NormalBlock
	y--;
	while(y >= 0){
		if(GetCharType(Point2D((float)x, (float)y)) >= 2){
			break;
		}
		else {y--; height+= _tileDimension.Y;}
	}
	y = (int)(position.Y / _tileDimension.Y);
	while((unsigned)y <= _mapArray.size()){
		charType = GetCharType(Point2D((float)x, (float)y));
		if(charType == 2) break;
		else if(charType == 3){
			TileData td = GetTileData(x, y);
			int y1, y2;
			td.GetSlope(y1, y2);
			float diff = position.X - x * _tileDimension.X;
			float ratio = (y2-y1)/_tileDimension.X;
			float h = y2>y1?_tileDimension.Y - ratio*diff:ratio*diff*-1;
			height += h;
			break;
		}
		else {y++; height+= _tileDimension.Y;}
	}
	//Error er; er.HandleError(Caption, ss.str());
	return height;

}
//Returns the height of a slope @ a positionX
float Map::GetSlopeHeight(Point2D position){
	int x = (int)(position.X / _tileDimension.X);
	int y = (int)(position.Y / _tileDimension.Y);
	const char* charline = _mapArray.at(y).c_str();
	TileData tiledat = _tileLibrary.find(charline[x])->second;
	int y1, y2;
	tiledat.GetSlope(y1, y2);
	if(y1 > y2){
		float diff = position.X-x*_tileDimension.X;
		float ratio = (y1-y2)/_tileDimension.X;
		return diff * ratio - y1 + _tileDimension.Y;
	}
	else {
		float diff = position.X-x*_tileDimension.X;
		float ratio = (y2-y1)/_tileDimension.X;
		return _tileDimension.Y - diff * ratio - y1;
	}
}
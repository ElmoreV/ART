#include "map.h"
TileData::TileData(int x, int y, int width, int height, bool solid, bool drawable){
	X=x;Y=y;Width=width;Height=height;_isSlope=false;_isSolid=solid;_isDrawable=drawable;
}
TileData::TileData(int x, int y, int width, int height, int slopeleft, int sloperight){
	X=x;Y=y;Width=width;Height=height;_isSlope=true;_slopeLeft=slopeleft;_slopeRight=sloperight;_isSolid=true;_isDrawable=false;

}
SDL_Rect TileData::Rect(){
	SDL_Rect value =  {X, Y, Width, Height};
	return value;
}
bool TileData::IsSlope(){return _isSlope; }
bool TileData::IsSolid(){return _isSolid; }
bool TileData::IsDrawable(){return _isDrawable;}
void TileData::GetSlope(int& y1, int& y2){ 
	if(_isSlope){y1=_slopeLeft; y2=_slopeRight;}
	else if(!_isSolid){y1=0;y2=0;}
	else { y1=1;y2=1;}
}
TileData Map::GetTileData(unsigned int x, unsigned int y){	
  TileData td(0, 0, 0, 0);
  if(_mapArray.size()>y){
    const char* charline = _mapArray.at(y).c_str();
    td = _tileLibrary.find(charline[x])->second;
  }
  return td;
}

//Initialize map (leave string map empty if using array)
Map::Map(std::string tileSheet, unsigned int tileWidth, unsigned int tileHeight, std::string map)
{
	LoadTileSheet(tileSheet); //TileSheet
	_tileDimension.X = (float)tileWidth; //Width of a tile
	_tileDimension.Y = (float)tileHeight; //Height of a tile
	_spawnLocation = '#';
	_newMapChar = '@';
	if(map != "") ReadFile(map);
}
void Map::LoadTileSheet(std::string tileSheet){
	_tileSheet.LoadImage(tileSheet.c_str(), 255, 255, 255);
}
//Read the file and push lines into vector
bool Map::ReadFile(std::string filename)
{
	_mapArray.clear();
	_drawObjects.clear();
	_lines=0;
	std::ifstream streamer;
	streamer.open(filename.c_str());
	if(!streamer) return false;
	std::string line;
	size_t dx;
	while(!streamer.eof())
	{
		std::getline(streamer, line);
		//Check if there is a spawnchar in it
		dx = line.find(_spawnLocation);
		if(dx != std::string::npos) {
			_spawnPosition.X = int(dx)*_tileDimension.X;
			_spawnPosition.Y = _lines*_tileDimension.Y;
		}

		_mapArray.push_back(line);
		_lines++;
	}
	return true;
}
void Map::SetMaskColor(int r, int g, int b){
	_tileSheet.MaskColor(r, g, b);
}
//Adds a new normal tile to the dictionary
bool Map::AddTile(char key, int x, int y, bool solid, bool drawable){
	TileData value(x, y, (int)_tileDimension.X, (int)_tileDimension.Y,solid, drawable);
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
bool Map::HandleEvent(SDL_Event sEvent, Rectangle playerBound){
	for(unsigned int i = 0; i < _drawObjects.size(); i++){
		_drawObjects.at(i).HandleEvent(sEvent,playerBound);
	}
	return true;
}
//Draws the excisting map on the screen
void Map::Draw(WindowSurface screen)
{
	unsigned int drawings = 0;
	const char* charline;
	for(unsigned int y = 0; y < _mapArray.size(); y++)
	{
		charline = _mapArray.at(y).c_str();
		for(unsigned int x = 0; x < strlen(charline); x++)
		{
			if(_tileLibrary.count(charline[x]) != 0 && charline[x] != _spawnLocation)
			{
				TileData td = _tileLibrary.find(charline[x])->second;
				SDL_Rect clip = td.Rect();
				//SDL_Rect offset = {(Uint16)(x * _tileDimension.X - _mapPosition.X), (Uint16)(y * _tileDimension.Y - _mapPosition.Y), clip.w, clip.h};
				_tileSheet.Draw(screen, (Uint32)(x * _tileDimension.X - _mapPosition.X), (Uint32)(y * _tileDimension.Y - _mapPosition.Y), &clip);
				//SDL_BlitSurface(_tileSheet, &clip, screen, &offset);
				if(td.IsDrawable()){
					if(_drawObjects.size() > drawings){
						_drawObjects.at(drawings).Draw(screen);
					}
					else {
						_drawObjects.push_back(DrawingObject(_tileDimension.X, _tileDimension.Y, x*_tileDimension.X, y*_tileDimension.Y));
						_drawObjects.at(drawings).Draw(screen);
					}
					drawings++;
				}
			}
		}
	}
}
//Draws a new map
void Map::Draw(WindowSurface screen,const char* mapArray[], unsigned int aantalRijen)
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
				//SDL_Rect offset = {(Uint16)(x * _tileDimension.X - _mapPosition.X), (Uint16)(y * _tileDimension.Y - _mapPosition.Y), clip.w, clip.h};
				_tileSheet.Draw(screen, (Uint32)(x * _tileDimension.X - _mapPosition.X), (Uint32)(y * _tileDimension.Y - _mapPosition.Y), &clip);
				//SDL_BlitSurface(_tileSheet, &clip, screen, &offset);
			}
		}
	}
}
//Get the type of collision of the inserten point
TileType Map::GetCharType(Point2D collisionPoint){
	const char* charline;
	if(_mapArray.size() > collisionPoint.Y){
		charline = _mapArray.at((int)collisionPoint.Y).c_str();
		if(strlen(charline) > collisionPoint.X){
			if(charline[(int)collisionPoint.X] == _newMapChar) return TileTypeNone;
			if(charline[(int)collisionPoint.X] == '-' || charline[(int)collisionPoint.X] == _spawnLocation) return TileTypeNone;
			else {
				TileData td = _tileLibrary.find(charline[(int)collisionPoint.X])->second;
				if(td.IsSlope()) return TileTypeSlope;
				else if(td.IsDrawable()) return TileTypeDrawing;
				else if(td.IsSolid()) return TileTypeNormal;
			}
		}
	}
	return TileTypeNone;
}
float Map::CheckDrawCollision(Rectangle playerBound){
	float maxH=0.0f;
	for(unsigned int i = 0; i < _drawObjects.size(); i++){
		float h = _drawObjects.at(i).CheckCollision(playerBound);
		if(h>maxH)
		{maxH=h;}	
	}
	return maxH;
}
bool Map::NewMapEnabled(Rectangle playerBoundingBox){
	//Ik wed dat deze functie beter kan, dan van float->int->float->int te gaan.
	//Als ik nu eens snapte waar deze functie voor was
	int X1 = (int)(playerBoundingBox.X/_tileDimension.X);
	int Y1 = (int)(playerBoundingBox.Y/_tileDimension.Y);
	int X2 = (int)((playerBoundingBox.X + playerBoundingBox.W)/_tileDimension.X);
	int Y2 = (int)((playerBoundingBox.Y + playerBoundingBox.H)/_tileDimension.Y);
	Point2D collisionPoint[4];
	collisionPoint[0] = Point2D((float)X1, (float)Y1);
	collisionPoint[1] = Point2D((float)X1, (float)Y2);
	collisionPoint[2] = Point2D((float)X2, (float)Y1);
	collisionPoint[3] = Point2D((float)X2, (float)Y2);
	for(int i = 0; i < 4; i++){
		const char* charline;
		if(_mapArray.size() > collisionPoint[i].Y){
			charline = _mapArray.at((int)collisionPoint[i].Y).c_str();
			if(strlen(charline) > collisionPoint[i].X)
				if(charline[(int)collisionPoint[i].X] == _newMapChar) 
					return true;
		}
	}
	return false;
}
//Returns the dimensions of a single tile
Point2D Map::GetTileDimension() { return _tileDimension; }
//Returns the position of the map which is displayed
Point2D Map::GetMapPosition() { return _mapPosition; }
//Returns the spawnposition of the player
Point2D Map::GetSpawnLocation(){return _spawnPosition; }
Point2D Map::GetMapDimension()
{
	return Point2D(_mapArray.at(0).size() * _tileDimension.X, _mapArray.size() * _tileDimension.Y);
}
//Sets the offset of the map, so from where it must be showed
void Map::SetMapPosition(float x, float y){ 
	for(unsigned int i = 0; i < _drawObjects.size(); i++){
		_drawObjects.at(i).GetDrawing().ChangeOffset(_mapPosition.X - x, _mapPosition.Y-y);
	} 
		_mapPosition.X = x; _mapPosition.Y = y;};
//Returns the height of empty pixels @ the position
float Map::GetHeightAtPosition(Point2D position){
	int y = (int)(position.Y / _tileDimension.Y);
	int x = (int)(position.X / _tileDimension.X);

	float height = 0;
	int charType = GetCharType(Point2D((float)x, (float)y));
	
	if(charType == TileTypeNormal) //NormalBlock
		return 0; 
	y--;
	
	while(y >= 0){
		if(GetCharType(Point2D((float)x, (float)y)) != TileTypeNone)
			break;
		else {
			y--; 
			height+= _tileDimension.Y;
		}
	}

	y = (int)(position.Y / _tileDimension.Y);
	while((unsigned)y <= _mapArray.size()){
		charType = GetCharType(Point2D((float)x, (float)y));
		if(charType == TileTypeNormal) 
			break;
		else if(charType == TileTypeSlope) {
			TileData td = GetTileData(x, y);
			int y1, y2; td.GetSlope(y1, y2);

			float diff = position.X - x * _tileDimension.X;
			float ratio = (y2-y1)/_tileDimension.X;
			float h = y2>y1?_tileDimension.Y - ratio*diff:ratio*diff*-1;
			height += h;
			break;
		}
		else {
			y++; 
			height+= _tileDimension.Y;
		}
	}
	return height;
}
//Returns the height of a slope @ a positionX
float Map::GetSlopeHeight(Point2D position){
	unsigned int x = (int)(position.X / _tileDimension.X);
	unsigned int y = (int)(position.Y / _tileDimension.Y);
	//out of range check
	if(y >= _mapArray.size()) return 0;
	if(x >=_mapArray[y].size()) return 0;

	const char* charline = _mapArray.at(y).c_str();
	TileData tiledat = _tileLibrary.find(charline[x])->second;
	int y1, y2;
	tiledat.GetSlope(y1, y2);
	float h;
	if(y1 > y2){
		float diff = position.X-x*_tileDimension.X;
		float ratio = (y1-y2)/_tileDimension.X;
		h = diff * ratio - y1 + _tileDimension.Y;
	}
	else {
		float diff = position.X-x*_tileDimension.X;
		float ratio = (y2-y1)/_tileDimension.X;
		h = _tileDimension.Y - diff * ratio - y1;
	}
	if(h > 1000) return 0;
	return h;
}
//Center the map
void Map::SetNewMapPosition(Point2D screenSize, Point2D centerPoint){
	float maxX = _tileDimension.X * _mapArray[0].size();
	float maxY = _tileDimension.Y * _mapArray.size();
	float newX=0, newY=0;
	if(maxX > screenSize.X){
		if(centerPoint.X <= screenSize.X / 2)
			newX = 0;
		else if(centerPoint.X >= maxX - screenSize.X / 2)
			newX = maxX - screenSize.X;
		else 
			newX = centerPoint.X - screenSize.X / 2;
	}
	else newX = 0;
	if(maxY > screenSize.Y){
		if(centerPoint.Y <= screenSize.Y / 2)
			newY = 0;
		else if(centerPoint.Y >= maxY - screenSize.Y / 2)
			newY = maxY - screenSize.Y;
		else 
			newY = centerPoint.Y - screenSize.Y / 2;
	}
	else newY = 0;
	for(unsigned int i = 0; i < _drawObjects.size(); i++){
		_drawObjects.at(i).GetDrawing().ChangeOffset(_mapPosition.X - newX, _mapPosition.Y-newY);
	}
	_mapPosition.X = newX;
	_mapPosition.Y = newY;
}
//Loads a new map
bool Map::NewMap(std::string map, unsigned int tileWidth, unsigned int tileHeight, std::string tileSheet){
	if(tileSheet != ""){
		_tileSheet.LoadImage(tileSheet, 255, 255, 255);
		_tileDimension.X = (float)tileWidth;
		_tileDimension.Y = (float)tileHeight;
	}
	if(map == "") {return false;}
	_spawnPosition.X =0; _spawnPosition.Y = 0;
	ReadFile(map);
	_drawObjects.clear();
	return true;
}
std::vector<std::string> Map::GetMapArray(){ return _mapArray; }
void Map::RemoveEnemiesFromArray(std::vector<char> enemies){
	std::string charline;
	for(unsigned int y = 0; y <_mapArray.size(); y++){
		charline = _mapArray.at(y);
		for (unsigned int i = 0; i < enemies.size(); ++i)
		{
			for (unsigned int x = 0; x < charline.length(); ++x) {
				if (charline[x] == enemies.at(i))
					charline[x] = '-';
				}
		}
		_mapArray.at(y) = charline;
	}
}
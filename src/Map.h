#ifndef MAP_H
#define MAP_H
#include "SDLFramework.h"
#include "Point.h"
#include <map>
#include <fstream>
#include <vector>
class TileData{
private:
	bool _isSlope, _isSolid;
	int _slopeLeft, _slopeRight;
public:
	int X, Y, Width, Height;
	TileData(int x, int y, int width, int height, bool solid=false);
	TileData(int x, int y, int width, int height, int slopeleft, int sloperight);
	SDL_Rect Rect();
	bool IsSlope();
	bool IsSolid();
	void GetSlope(int& y1, int& y2);
};
typedef std::map<char, TileData> Dictionary;
class Map
{
protected:
	Dictionary _tileLibrary;
	Surface _tileSheet;
	unsigned int _lines;
	std::vector<std::string> _mapArray;
	char _spawnLocation, _newMapChar;
	Point2D _tileDimension, _mapPosition, _spawnPosition;
public:
	Map(std::string tileSheet, unsigned int tileWidth, unsigned int tileHeight, std::string map="");
	void Draw(WindowSurface screen);
	void Draw(WindowSurface screen, const char* mapArray[], unsigned int aantalRijen);
	bool AddTile(char key, int x, int y);
	bool AddTile(char key, int x, int y, int slopeLeft, int slopeRight); 
	bool ReadFile(std::string filename);
	void LoadTileSheet(std::string tileSheet);
	int GetCharType(Point2D collisionPoint);
	Point2D GetTileDimension();
	Point2D GetMapPosition();
	Point2D GetSpawnLocation(){return _spawnPosition; }
	void SetMapPosition(float x, float y){ _mapPosition.X = x; _mapPosition.Y = y; };
	TileData GetTileData(unsigned int x, unsigned int y);
	float GetHeightAtPosition(Point2D position);
	float GetSlopeHeight(Point2D position);
	void SetNewMapPosition(Point2D screenSize, Point2D centerPoint);
	bool NewMap(std::string map, unsigned int tileWidth=0, unsigned int tileHeight=0, std::string tileSheet="");
};
#endif
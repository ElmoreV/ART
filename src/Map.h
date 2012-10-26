#ifndef MAP_H
#define MAP_H
#include "SDLFramework.h"
#include "Point.h"
#include <map>
#include <fstream>
#include <vector>
class TileData{
private:
	bool _isSlope;
	int _slopeLeft, _slopeRight;
public:
	int X, Y, Width, Height;
	TileData(int x, int y, int width, int height);
	TileData(int x, int y, int width, int height, int slopeleft, int sloperight);
	SDL_Rect Rect();
	bool IsSlope();
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
	char _spawnLocation;
	Point2D _tileDimension, _mapPosition;
public:
	Map(std::string tileSheet, unsigned int tileWidth, unsigned int tileHeight, std::string map="");
	void Draw(SDL_Surface* screen);
	void Draw(SDL_Surface* screen, const char* mapArray[], unsigned int aantalRijen);
	bool AddTile(char key, int x, int y);
	bool AddTile(char key, int x, int y, int slopeLeft, int slopeRight); 
	bool ReadFile(std::string filename);
	int GetCharType(Point2D collisionPoint);
	Point2D GetTileDimension();
	Point2D GetMapPosition();
	float GetHeightAtPosition(Point2D position);
	float GetSlopeHeight(int x, int y, float positionX);
};
#endif
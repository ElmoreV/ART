#ifndef MAP_H
#define MAP_H
#include "SDLFramework.h"
#include "Point.h"
#include <map>
#include <fstream>
#include <sstream>
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
	//adds a normal tile
	bool AddTile(char key, int x, int y);
	//Adds a slope tile
	bool AddTile(char key, int x, int y, int slopeLeft, int slopeRight); 
	//Reads an inserted file
	bool ReadFile(std::string filename);
	//Defines what type of char ther is 
	//0 = outofrange
	//1 = none
	//2 = normal block
	//3 = slope
	int GetCharType(Point2D collisionPoint);
	//Returns a tiledata at pos (x, y) in array
	TileData GetTileData(unsigned int x, unsigned int y);
	//Returns the dimensions of a tile
	Point2D GetTileDimension();
	//returns the map position. used for map scrolling
	Point2D GetMapPosition();
	//Get vertical room @ current position
	float GetHeightAtPosition(Point2D position);
	//Get height of slope in current position
	float GetSlopeHeight(Point2D position);
};
#endif
#ifndef MAP_H
#define MAP_H
#include "SDLFramework.h"
#include "Point.h"
#include "DrawingObject.h"
#include <map>
#include <fstream>
#include <vector>
class TileData{
private:
	bool _isSlope, _isSolid, _isDrawable;
	int _slopeLeft, _slopeRight;
public:
	int X, Y, Width, Height;
	TileData(int x, int y, int width, int height, bool solid=false, bool drawable=false);
	TileData(int x, int y, int width, int height, int slopeleft, int sloperight);
	SDL_Rect Rect();
	bool IsSlope();
	bool IsSolid();
	bool IsDrawable();
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
	std::vector<DrawingObject> _drawObjects;
	char _spawnLocation, _newMapChar;
	Point2D _tileDimension, _mapPosition, _spawnPosition;
	bool _newMap;
public:
	Map(std::string tileSheet, unsigned int tileWidth, unsigned int tileHeight, std::string map="");
	void Draw(WindowSurface screen);
	void Draw(WindowSurface screen, const char* mapArray[], unsigned int aantalRijen);
	bool AddTile(char key, int x, int y, bool solid=true, bool drawable=false);
	bool AddTile(char key, int x, int y, int slopeLeft, int slopeRight); 
	bool ReadFile(std::string filename);
	void LoadTileSheet(std::string tileSheet);
	int GetCharType(Point2D collisionPoint);
	Point2D GetTileDimension();
	Point2D GetMapPosition();
	Point2D GetSpawnLocation();
	Point2D GetMapDimension();
	bool HandleEvent(SDL_Event sEvent);
	float CheckDrawCollision(Rectangle playerBound);
	void SetMapPosition(float x, float y);
	TileData GetTileData(unsigned int x, unsigned int y);
	float GetHeightAtPosition(Point2D position);
	float GetSlopeHeight(Point2D position);
	void SetNewMapPosition(Point2D screenSize, Point2D centerPoint);
	bool NewMapEnabled();
	bool NewMap(std::string map, unsigned int tileWidth=0, unsigned int tileHeight=0, std::string tileSheet="");
};
#endif
#ifndef MAP_H
#define MAP_H
#include "SDLFramework.h"
#include "Point.h"
#include "DrawingObject.h"
#include <map>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include "Assets.h"
enum TileType {
	TileTypeNone, TileTypeNormal, TileTypeSlope, TileTypeNewMap, TileTypeDrawing, TileTypeSpike
};
enum TileSides{ TSnone, TStop, TSbottom, TSleft, TSright};
class TileData{
private:
	bool _isSolid;
	int _slopeLeft, _slopeRight;
	TileType _type;
public:
	int X, Y, Width, Height;
	TileData(int x, int y, int width, int height, bool solid=false, bool drawable=false);
	TileData(int x, int y, int width, int height, TileSides side);
	TileData(int x, int y, int width, int height, int slopeleft, int sloperight);
	SDL_Rect Rect();
	bool IsSlope();
	bool IsSolid();
	bool IsDrawable();
	TileType GetType();
	void GetSlope(int& y1, int& y2);
	TileSides Side;
};
typedef std::map<char, TileData> Dictionary;
class Map
{
protected:
	Dictionary _tileLibrary;
	Surface* _tileSheet;
	unsigned int _lines;
	std::vector<std::string> _mapArray;
	std::vector<DrawingObject> _drawObjects;
	char _spawnLocation, _newMapChar;
	Point2D _tileDimension, _mapPosition, _spawnPosition;
	int _forestBbStart;
	unsigned long _totalDistance;
public:
	Map(Surface* tilesheet, unsigned int tileWidth, unsigned int tileHeight, std::string map);
	void Draw(WindowSurface screen);
	bool AddTile(char key, int x, int y, bool solid=true, bool drawable=false);
	bool AddTile(char key, int x, int y, TileSides side);
	bool AddTile(char key, int x, int y, int slopeLeft, int slopeRight); 
	bool ReadFile(std::string filename);
	TileType GetCharType(Point2D collisionPoint);
	Point2D GetTileDimension();
	Point2D GetMapPosition();
	Point2D GetSpawnLocation();
	Point2D GetMapDimension();
	unsigned int GetDrawDistance();
	bool HandleEvent(SDL_Event sEvent);
	bool Update(Rectangle playerBound,float inkPool);
	float CheckDrawCollision(Rectangle playerBound);
	void SetMapPosition(float x, float y);
	TileData GetTileData(unsigned int x, unsigned int y);
	float GetHeightAtPosition(Point2D position);
	float GetSlopeHeight(Point2D position);
	void SetNewMapPosition(Point2D screenSize, Point2D centerPoint);
	void SetMaskColor(int r=255, int g=255, int b=255);
	bool NewMapEnabled(Rectangle playerBoundingBox);
	bool NewMap(std::string map, unsigned int tileWidth=0, unsigned int tileHeight=0, Surface* tilesheet=0);
	std::vector<std::string> GetMapArray();
	void RemoveEnemiesFromArray(std::vector<char> enemies);
	void DrawBackground(WindowSurface screen, Graphics* assets);
};
#endif
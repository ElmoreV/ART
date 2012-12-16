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
//Enumeration with all types of tiles in the map
enum TileType {
	TileTypeNone, TileTypeNormal, TileTypeSlope, TileTypeNewMap, TileTypeDrawing, TileTypeSpike
};
//Enumeration with all sides of a tile, used with the spike tile, to define where the spikes are
enum TileSides{ TSnone, TStop, TSbottom, TSleft, TSright};
//All the information of a tile
class TileData{
private:
	bool _isSolid; //Defines if a tile is solid, so a player cant walk through it
	int _slopeLeft, _slopeRight; //If the tile is a slope, this is the different height of the left and right side of the slope
	TileType _type; //Type of the tile
public:
	//X and Y are the position of the spriteimage in the tilesheet
	//Width and Height are the dimensions of the tile
	int X, Y, Width, Height;
	//Constructor for a normal tile
	TileData(int x, int y, int width, int height, bool solid=false, bool drawable=false);
	//Constructor for a spike tile, where can be defined at which side the spikes are
	TileData(int x, int y, int width, int height, TileSides side);
	//Constructor for a slope tile, where the left and right height of the slope can be set
	TileData(int x, int y, int width, int height, int slopeleft, int sloperight);
	//Returns the rectangle of the tile in the tilesheet
	SDL_Rect Rect();
	//Returns if the tile is a slope
	bool IsSlope();
	//Returns if the player can walk through it
	bool IsSolid();
	//Returns if it is a drawing tile, so the player can draw on it
	bool IsDrawable();
	//Returns the type of the tile
	TileType GetType();
	//Gives the left and rightside of the slopeheight
	void GetSlope(int& y1, int& y2);
	//Defines at what side the spikes are, if it is a spike, else it is none
	TileSides Side;
};
//The tilelibrary, where all char in the map are defined
typedef std::map<char, TileData> Dictionary;
//The declaration of the loader class, which can load earlier save files
class Loader;
//The map
class Map
{
	friend Loader;
protected:
	//Collection of all chars and their tiledata
	Dictionary _tileLibrary;
	//The tilesheet, necessary to render the map
	Surface* _tileSheet;
	//Amount of lines (vertical) of the maparray
	unsigned int _lines;
	//List of all the lines of a text file, reads all characters for the map
	std::vector<std::string> _mapArray;
	//List of all drawingobjects in the map
	std::vector<DrawingObject> _drawObjects;
	//Define the special characters
	//_spawnlocation = char where the player will spawn
	//_newMapChar = char of the entrance to a new map
	//_newMapCharClosed = char of a closed entrance to a new map
	char _spawnLocation, _newMapChar, _newMapCharClosed;
	//_tileDimension = dimensions of a single tile
	//_mapPosition = cameraposition on the map (moves with the player, to center the player)
	//_spawnPosition = position of the player to spawn, set where the _spanLocationChar is
	//if it is not available in _mapArray, this position is (0, 0)
	Point2D _tileDimension, _mapPosition, _spawnPosition;
	//The startcounter of the forest, for background, to define where the bottomside of the forest is
	int _forestBbStart;
	//Total drawn distance
	float _mapDrawDistance;
	//Total Drawn distance of last frame
	unsigned long _previousDrawDistance;
	//List of all map id, to change levels
	//Map id can be defined in a text file like @=5;
	//This means that the newmapchar is the entrance to mapId 5
	std::vector<int> _newMapId;
	//Returns the id of the newmapchar
	int GetNewMapId(int x, int y);
	//If the entrances of the map are lockable
	bool _locked;
public:
	//Contructor, which set the tilesheet, tiledimensions, and the textfile to read for the map
	Map(Surface* tilesheet, unsigned int tileWidth, unsigned int tileHeight, std::string map);
	//Reset all values of the map to their origine
	void Reset();
	//Draws the part of the map that can be seen on the screen, to save time
	void Draw(WindowSurface screen, int completedLvl = -1);
	//Draws the background on the screen (only the part of the images that can be seen on the screen)
	void DrawBackground(WindowSurface screen, Graphics* assets);
	//Add a normal tile to the collection
	bool AddTile(char key, int x, int y, bool solid=true, bool drawable=false);
	//Add a spike tile to the collection
	bool AddTile(char key, int x, int y, TileSides side);
	//Add a slope tile to the collection
	bool AddTile(char key, int x, int y, int slopeLeft, int slopeRight); 
	//Reads the textfile, and set all values of the map, like _mapArray, _spawnlocation, _mapID etc.
	bool ReadFile(std::string filename);
	//Retruns the TileType at a certain point in the mapArray
	TileType GetCharType(Point2D collisionPoint);
	//Returns the tiledimensions
	Point2D GetTileDimension();
	//Returns the position of the camera
	Point2D GetMapPosition();
	//Returns the spawnlocation of the player
	Point2D GetSpawnLocation();
	//Returns the dimensions of the whole map 
	Point2D GetMapDimension();
	//Retruns the Total Drawn distance
	unsigned int GetDrawDistance();
	//Handles all events occured in the map (drawobjects)
	bool HandleEvent(SDL_Event sEvent);
	//Updates the map
	bool Update(Rectangle playerBound,float inkPool);
	//Return the height of the collision of the player with a line (0 if no collision)
	float CheckDrawCollision(Rectangle playerBound);
	//Set the cameraposition
	void SetMapPosition(float x, float y);
	//Return the tiledata of a specific char at the _mapArray
	TileData GetTileData(unsigned int x, unsigned int y);
	//Returns the vertical height at a certain point
	//Used to check if the player can walk up a slope, and not walk through another block or line above him
	float GetHeightAtPosition(Point2D position);
	//Returns the slopeheight at a certain position
	float GetSlopeHeight(Point2D position);
	//Calculate and set the new cameraPosition
	void SetNewMapPosition(Point2D screenSize, Point2D centerPoint);
	//Set the mask color of the tilesheet
	void SetMaskColor(int r=255, int g=255, int b=255);
	//Return whether or not the player can go to the next level (is at an entrance)
	//It also returns the mapId to which the player wants to go
	bool NewMapEnabled(Rectangle playerBoundingBox, int& levelId);
	//Read a new map file, and reset all old values, spawn new enemies, and start a new level
	bool NewMap(std::string map, unsigned int tileWidth=0, unsigned int tileHeight=0, Surface* tilesheet=0);
	//Returns the list of all lines of the _mapArray
	std::vector<std::string> GetMapArray();
	//Removes all enemychars from the _mapArray
	void RemoveEnemiesFromArray(std::vector<char> enemies);
};
#endif
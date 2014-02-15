#ifndef ENEMY_H
#define ENEMY_H
#include "Object.h"
#include "Map.h"
#include "Player.h"
#include <vector>

//Enumeration with all types of enemies that are possible to make
enum EnemyType{
	EnemyBlob, EnemySpider, EnemyFire
};
//The enemy class
class Enemy : public Object
{
	EnemyType _type; //Type of the enemy
	//Required variables for the animation
	//_interval is a constant after how many frames the animation must change to another
	//_countInteval will count the amount of frames that passed (to check with interval if a new frame is necessary)
	//_animationState tracks what the current animation is (walk left, walk right, etc)
	//_frame is the part of the image that must be showed
	int _interval, _countInterval, _animationState, _frame;
public:
	//Constructor, which sets the enemy type, image of the enemy, the position, animationInterval and frames
	Enemy(EnemyType type, Surface* surface, float X, float Y, int interval, int spriteX=1, int spriteY=1);
	//Will update the animation of the enemy, so it looks like it is moving
	void Update();
	//Draws the enemy (a frame of the surface) on the windowsurface
	void Draw(Window screen, Point2D mapPosition);
	//Return the center point of the enemy
	Point2D GetCenter();
	SDL_Rect GetFrame();//Get the part of the image that is showed
	Rectangle GetFrameR();//The function with floats
	void SetFrame(int frame);//Set the animation (if it has more then 1)
	bool CanWalkOff; //If an enemy can walk of a block (tile)
	bool CanWalkSlope; //If an enemy can walk on a slope or not
};
//The enemy handler, which handles all the enemy objects in a map
class EnemyHandler{
private:
	unsigned int counter;
	std::vector<Enemy> _enemyList; //List of the enemy objects in the current map
	std::vector<char> enemyChars; //List of all char in a *.txt file that are enemies
public:
	//Constructor, only defines the enemy chars
	EnemyHandler();
	//Adds an enemyobject to the enemylist, with a specific type and position
	//It will get the enemyimage from the graphics class
	void AddEnemy(EnemyType type, Graphics* graphics, Point2D position);
	//Updates all enemies that are in the list
	//It checks if the player hits any enemy, and handles it (remove enemy or decrease health of player)
	//Move alle enemies to their new position, and checks if a enemy collides with a wall, 
	//If so, this function will handle this
	void Update(Map* map, Player* player, Sounds& sound,long timediff=-1, Graphics* graphics=0);
	//Draws every enemy on the Window
	void Draw(Window screen, Point2D mapPosition);
	//If a new map is created, this function will be called to repopulate all the enemies in the new level.
	//All the old enemies will be destroyed, and new once will be added to the enemylist with te position on the map
	void PopulateEnemies(Map* map, Graphics* graphics);
};
#endif ENEMY_H
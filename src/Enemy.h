#ifndef ENEMY_H
#define ENEMY_H
#include "Object.h"
#include "Map.h"
#include "Player.h"
#include <vector>
enum EnemyType{
	EnemyNormal, EnemyCool, 
};
class Enemy : public Object
{
	EnemyType _type;
	void MoveEnemy(Map& map, long timeDiff);
	int _interval, _countInterval, _animationState, _frame;
public:
	Enemy(EnemyType type, std::string filename, float X, float Y, int interval, int spriteX=1, int spriteY=1);
	void Update(Map& map, long lastTick=-1);
	void Draw(WindowSurface screen, Point2D mapPosition);
	Point2D GetCenter();
	SDL_Rect GetFrame();//Get the part of the image that is showed
	Rectangle GetFrameR();//The function with floats
	void SetFrame(int frame);//Set the animation (if it has more then 1)
	bool CanWalkOff;
	bool CanWalkSlope;
};
class EnemyHandler{
private:
	std::vector<Enemy> _enemyList;
public:
	EnemyHandler();
	void AddEnemy(EnemyType type, Point2D position);
	void Update(Map* map, Player* player, long timediff=-1);
	void Draw(WindowSurface screen, Point2D mapPosition);
};
#endif ENEMY_H
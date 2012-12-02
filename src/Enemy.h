#ifndef ENEMY_H
#define ENEMY_H
#include "Object.h"
#include "Map.h"
#include "Player.h"
#include <vector>

enum EnemyType{
	EnemyNormal, 
};
class Enemy : public Object
{
	EnemyType _type;
	void MoveEnemy(Map& map, long timeDiff);
	int _interval, _countInterval, _animationState, _frame;
public:
	Enemy(EnemyType type, Surface* surface, float X, float Y, int interval, int spriteX=1, int spriteY=1);
	void Update(Map* map, long lastTick=-1);
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
	std::vector<char> enemyChars;
public:
	EnemyHandler();
	void AddEnemy(EnemyType type, Graphics* graphics, Point2D position);
	void Update(Map* map, Player* player, long timediff=-1);
	void Draw(WindowSurface screen, Point2D mapPosition);
	void PopulateEnemies(Map* map, Graphics* graphics);
};
#endif ENEMY_H
#ifndef PLAYER_H
#define PLAYER_H
#include "SDLFramework.h"
#include "Object.h"
class Player : public Object
{
protected:
	Point2D _previousPosition;
	//For the animations
	int _interval, _countInterval, _animationState, _lastFrame;
	int _maxVelocity;
	bool _jumpEnable;
	void HandleCollision(Map& map, int screenWidth, int screenHeight, float timeDiff);
	Rectangle GetPreviousBoundR(float velocityX, float velocityY);
	//bool _newmap;
public:
	//To set the default values
	Player(std::string filename, float X, float Y, int interval, int spriteX=1, int spriteY=1);
	//Update the position based on the velocity and draws the clip on screen
	void SetVelocity(float X, float Y);
	void Draw(WindowSurface screen, Point2D mapPosition);
	void Update(Map& map1, int screenWidth, int screenHeight, long lastTick=-1);
	Point2D GetCenter();
	//bool NewMapEnabled();
	//void SetNewMap(Point2D position);
};
#endif
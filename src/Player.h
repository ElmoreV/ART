#ifndef PLAYER_H
#define PLAYER_H
#include "SDLFramework.h"
#include "Object.h"
#include "Tail.h"
enum HorizontalDirection
{
	HDirLeft,
	HDirRight,
	HDirNone,
};
enum VerticalDirection
{
	VDirUp,
	VDirDown,
	VDirNone
};
class Player : public Object
{
protected:
	Tail tail;
	Point2D _previousPosition;
	VerticalDirection _vDir;
	HorizontalDirection _hDir;
	//For the animations
	int _interval, _countInterval, _animationState, _frame;
	int _maxVelocity;
	bool _jumpEnable;
	void HandleCollision(Map* map, int screenWidth, int screenHeight, float timeDiff);
	Rectangle GetPreviousBoundR(float velocityX, float velocityY);
	bool _buttonUp, _buttonDown, _buttonLeft, _buttonRight;
public:
	//To set the default values
	Player(std::string filename, float X, float Y, int interval, int spriteX=1, int spriteY=1);
	//Update the position based on the velocity and draws the clip on screen
	void SetVelocity(float X, float Y);
	void Draw(WindowSurface screen, Point2D mapPosition);
	void Update(Map* map1, int screenWidth, int screenHeight, long lastTick=-1);
	void HandleEvent(SDL_Event sEvent); //Keep up if arrowbuttons are pressed
	Point2D GetCenter();
	HorizontalDirection GetHorizontalDir();
	VerticalDirection GetVerticalDir();
	SDL_Rect GetFrame();//Get the part of the image that is showed
	Rectangle GetFrameR();//The function with floats
	void SetFrame(int frame);//Set the animation (if it has more then 1)
};
#endif
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
class Saver;
class Loader;
class Player : public Object
{
	friend Saver;
	friend Loader;
protected:
	Tail tail;
	Point2D _previousPosition;
	VerticalDirection _vDir;
	HorizontalDirection _hDir;
	//For the animations
	int _interval, _countInterval, _animationState, _frame;
	int _maxVelocity, _maxHealth, _maxInkPool;
	bool _jumpEnable;
	void HandleCollision(Map* map, int screenWidth, int screenHeight, float timeDiff);
	Rectangle GetPreviousBoundR(float velocityX, float velocityY);
	bool _buttonUp, _buttonDown, _buttonLeft, _buttonRight;
	float _totalInkReceived;
public:
	//To set the default values
	Player(Surface* surface, float X, float Y, int interval, int spriteX=1, int spriteY=1);
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
	float Health, InkPool, InvulnerableTime;
	float HealthRatio(), InkPoolRatio();
	void AddInk(int value);
	void Jump();
	Point2D GetPreviousPosition();
	Rectangle GetPreviousBoundR();
	void DrawHealthBar(WindowSurface screen, int borderWidth, unsigned int X, unsigned int Y, unsigned int Width = 100, unsigned int Height = 20, Font* font = 0);
	void DrawInkBar(WindowSurface screen, int borderWidth, unsigned int X, unsigned int Y, unsigned int Width = 100, unsigned int Height = 20, Font* font = 0);
	void Reset(Point2D position = 0, bool resetStats=false);
};
#endif
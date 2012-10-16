#ifndef STIJN_H
#define STIJN_H
#include "SDLFramework.h"

//This is the basic object, which is not able to move
class Object
{
protected:
	//Position
	int _x, _y;
	//Showing: if it is visible
	bool _showing;
	//Sprite
	Surface _image;
public:
	//Initialize
	Object(int X, int Y, Surface image);
	//Draws the object
	void Draw(Surface screen);
};
//This is an inheritance of the normal, but now with Velocity, so it can move
class MovingObj : public Object
{
protected:
	//Velocity
	int _velocityX, _velocityY;
public:
	//Initialize with velocity
	MovingObj(int x, int y, Surface image, int velocityX, int velocityY);
	//Update the new position en draws it
	void Draw(Surface screen);
};
//Movable object Player, which is based on a simple RPG Maker character image, for test with animations
class Player : public MovingObj
{
protected:
	int _animationState;
	SDL_Rect _clip[12];
	bool _buttonUp, _buttonDown, _buttonLeft, _buttonRight;
	/* 2: top, 4: left, 6: right, 8: down */
	int _lastDirection;
	//SDL_Event sEvent; HELEMAAL FOUT STIJNTJE :P
	// Deze moet je in de "PollEvent(&sEvent)"-while-loop opvragen in een functie, die in het algemeen "handle_events()" genoemd wordt;
public:
	Player(int x, int y, int velocityX, int velocityY, Surface image);
	void HandleEvents(SDL_Event sEvent);
	void Draw(Surface screen);
};
#endif
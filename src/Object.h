#ifndef OBJECT_H
#define OBJECT_H
#include "SDLFramework.h"
#include "Point.h"
#include "Map.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <strstream>
#include <map>
#include <list>

class Object
{
protected:
	//Frame: selected clip
	//spriteX: amount of sprites on horizontal line in the surface
	//spriteY: same as spriteX, but in vertical line
	//tileWidth: width of the clip (or surfacewidth if spriteX=1)
	//tileHeight: height of the clip (or surfaceheight if spriteY=1)
	int _frame, _spriteX, _spriteY; 
	Point2D _position, _velocity, _spriteDimension;
	Surface _surface;
	bool _buttonUp, _buttonDown, _buttonLeft, _buttonRight;
public:
	//Normal function
	Object(std::string filename, float X, float Y, int spriteX=1, int spriteY=1); //Constructor
	SDL_Rect GetBound(float velocityX=0, float velocityY=0);//Get the position and dimentions of the object on the screen
	SDL_Rect GetFrame();//Get the part of the image that is showed
	bool ChangeSprite(std::string filename, int r=-1, int g=-1, int b=-1); //Changes the surface and set maskcolor
	bool MaskColor(int r, int g, int b); //Set the colorkey (mask)
	void Draw(WindowSurface screen);//Draws (clip of) surface on the screen
	void HandleEvent(SDL_Event sEvent); //Keep up if arrowbuttons are pressed
	void SetPosition(float X, float Y);//Changes the position of the object
	void SetPosition(Point2D position);
	void SetVelocity(float X, float Y);
	void SetFrame(int frame);//Set the animation (if it has more then 1)
	void Free(); //Releases resources of the object
	bool _showing;
	//Collision function
	SDL_Rect ConvertToImagePosition(SDL_Rect rect); //Convert the collistion rectangle to the position on the surface
	SDL_Rect GetBoundingBox(SDL_Rect boundsA, SDL_Rect boundsB); //Get the collistionpart between two rectangles
	bool CheckCollision(Object objB); //Check if there is pixelcollision
    bool GetAlphaXY(Object* obj, int x, int y);//Get if the selected pixel is transpirant
};
#endif
#ifndef OBJECT_H
#define OBJECT_H
#include "SDLFramework.h"
#include "Point.h"
#include "Map.h"
#include "Rectangle.h"
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
	int _spriteX, _spriteY; 
	Point2D _position, _velocity, _spriteDimension;
	Surface _surface;
public:
	//Normal function
	Object(std::string filename, float X, float Y, int spriteX=1, int spriteY=1); //Constructor
	Rectangle GetBoundR(float velocityX=0, float velocityY=0);//Get the position and dimentions of the object on the screen
	SDL_Rect GetBound(float velocityX=0, float velocityY=0);//This is the lower quality SDL_Rect version

	bool ChangeSprite(std::string filename, int r=-1, int g=-1, int b=-1); //Changes the surface and set maskcolor
	bool MaskColor(int r, int g, int b); //Set the colorkey (mask)
	void Draw(WindowSurface screen);//Draws (clip of) surface on the screen
	void SetPosition(float X, float Y);//Changes the position of the object
	void SetPosition(Point2D position);
	void SetVelocity(float X, float Y);
	void SetVelocity(Point2D velocity);
	Point2D GetVelocity();
	Point2D GetPosition();
	void Free(); //Releases resources of the object
#ifdef OBJ_PIXEL_COLLISION
	//Collision function
	SDL_Rect ConvertToImagePosition(SDL_Rect rect); //Convert the collistion rectangle to the position on the surface
	Rectangle ConvertToImagePositionR(Rectangle rect);
	SDL_Rect GetBoundingBox(SDL_Rect boundsA, SDL_Rect boundsB); //Get the collistionpart between two rectangles
	Rectangle GetBoundingBoxR(Rectangle boundsA, Rectangle boundsB);
	bool CheckCollision(Object objB); //Check if there is pixelcollision
	bool CheckCollisionR(Object objB);
    bool GetAlphaXY(Object* obj, int x, int y);//Get if the selected pixel is transpirant
#endif
};
#endif
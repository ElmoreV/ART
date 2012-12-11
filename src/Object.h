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
	int _spriteX, _spriteY; 
	//_position is the position of this object at the map
	//_velocity is the amount of pixel that the object moves per second
	//_spritedimensions are the dimensions of a single frame
	Point2D _position, _velocity, _spriteDimension;
	//The object image
	Surface* _surface;
public:
	//Normal function
	Object(Surface* surface, float X, float Y, int spriteX=1, int spriteY=1); //Constructor
	Rectangle GetBoundR(float velocityX=0, float velocityY=0);//Get the position and dimensions of the object on the screen
	SDL_Rect GetBound(float velocityX=0, float velocityY=0);//This is the lower quality SDL_Rect version

	bool ChangeSprite(std::string filename, int r=-1, int g=-1, int b=-1); //Changes the surface and set maskcolor
	bool MaskColor(int r, int g, int b); //Set the colorkey (mask)
	void Draw(WindowSurface screen);//Draws (clip of) surface on the screen
	void SetPosition(float X, float Y);//Changes the position of the object
	void SetPosition(Point2D position);//Changes the position of the object
	void SetVelocity(float X, float Y);//Changes the velocity of the object
	void SetVelocity(Point2D velocity);//Changes the velocity of the object
	Point2D GetVelocity();//Return the velocity of the object
	Point2D GetPosition();//Return the position of the object
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
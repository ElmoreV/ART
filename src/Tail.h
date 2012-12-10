#ifndef TAIL_H
#define TAIL_H
#include "Point.h"
#include "SDLFramework.h"
#include "Rectangle.h"
#include <vector>
extern enum HorizontalDirection;

class Tail
{
public:
	//The constructor sets all the data members to 0
	Tail();
	//Handles  and stores user input
	//Stores cursor position
	void HandleEvent(SDL_Event sEvent);
	//Handles and processes the user input
	//Determines by looking at the playerRectangle and the direction where the base of the tail is
	//Determines by looking at the cursor position where the tip of the tail is to be taken 
	//(with an intersection between the ray, which is between the cursor position and the mouse position, and the boundary circle
	//Then create an extra point based on the base point
	//Create a set of points following a Bezier curve based on the tip, base and the extra point
	void Update(Rectangle playerRect,HorizontalDirection playerDirection);
	//Draw the set of points of the Bezier curve
	//Make it thick the first half of points
	//Make it extra thick the first quarter of point
	void Draw(WindowSurface screen);
private:
	Point2D _base;//base point
	Point2D _tip;//tip point
	Point2D _bezierCurve[101]; //set of points based on bezier
	Point2D _lastCursor;//the last known cursor position
};


#endif
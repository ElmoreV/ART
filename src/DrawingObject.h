#ifndef DRAWING_OBJECT_H
#define DRAWING_OBJECT_H

#include "Drawing.h"
#include "Rectangle.h"
#include "Assets.h"

class DrawingObject
{
public:
	//The constructor creates a 'VectorDraw' object of width and height of the surface, and sets the offset from the beginning (x and y)
	DrawingObject(float width, float height, float x, float y);
	//Resets all booleans and the VectorDraw-object.
	void Reset();
	//Handles and storesthe player input
	//Remembers the mouse position
	//Determines if the mouse button is pressed.
	void HandleEvent(SDL_Event sEvent);
	//Handles and processes the acquired user input
	//checks whether the cursor it is in range of the player character
	//checks whether the cursor isn't on top op the player character
	//Also has the ability to create intersections of a line between two points and the boundary circle
	void Update(Rectangle playerBound);
	//Checks if there is collision between the lines in the 'VectorDraw' and the rectangle of the object.
	//This is used for collision detection and handling of the player
	//Returns a float containing the height above the bottom edge of the object
	//Return 0.0f when there is no collision
	float CheckCollision(Rectangle ObjectRect);
	//Draws lines connecting the points in the surface
	void Draw(Window sfScreen);
	//Get the 'vectordraw'  object.
	inline VectorDraw& GetDrawing(){return _canvas;}
	//The second on/off-switch for drawing.
	//This one set to true when there is enough ink.
	//Set to false when the ink pool is empty.
	void SetDrawMode(bool drawing);
private:
	bool _cursorPressed;//Stores whether the cursor is pressed
	bool _cursorOnPlayer;//Stores whether the cursor is on the player character
	bool _cursorOutOfRange;//Stores whether the cursor is out of range
	bool _drawing;//True=there is enough ink, you can set the new point, false= there isn't enough ink. Don't draw new points
	Point2D _prevCursor;//The previous cursor position
	Point2D _newCursor;//The new cursor position
	VectorDraw _canvas;//The actual drawing surface containing the list of points

};

#endif
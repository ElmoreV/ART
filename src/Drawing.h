#ifndef DRAWING_H
#define DRAWING_H
#include <vector>
#include "Point.h"
#include "SDLFramework.h"
class VectorDraw
{
public:
	//Creates a surface of a certain width and height, and reset the values
	VectorDraw(float width, float height);
	//Set and check for a new point to add to the surface
	//It will check whether the point is on the surface or not
	//It will also check whether the line between last point and the new point is on the surface
	//If so, it will check what the intersecting points are, and sets the new points there.
	void SetNewPoint(float x, float y);
	//Sets the offset from the beginning. Useful for setting the position after starting new maps.
	//X and Y are absolute values
	void SetOffset(float X, float Y);
	//Changing the offset. Is used to handle camera movement.
	//Also takes care of drawing a line when drawing when moving.
	//The x and y are relative, so if you want to move it 2 to the right, you do ChangeOffset(2,0);
	void ChangeOffset(float relativeX, float relativeY);
	//SetDrawMode takes care of setting the surface to on or off.
	//It is set to false (off)  when the cursor is pressed, or the cursor is on the mouse, or out of range of the mous
	void SetDrawMode(bool startDrawing=true);
	//Can be used to reduce the amount of points on the screen.
	//The precision specifies by how much subsequent points will be simplified, determined by their gradient and deviation
	void Optimize(float precision);
	//Delete all the points
	void Clear(){_points.clear();}
	//Get a point at the index'th index of the list
	inline Point2D GetPoint(int index){return _points[index];};
	//Get the amount of points stored in the list
	unsigned int GetSize(){return _points.size();}
	//Get the absolute offset from origin O
	Point2D GetOffset(){return _offset;};
	//Get the width of the surface
	float GetWidth(){return _width;}
	//Get the height of the surface
	float GetHeight(){return _height;}
	//Add the distance of all the lines together, to give the total distance drawn on this surface
	float GetDrawingDistance();

private:
	unsigned int _recurseChecker;//Is used in 'SetNewPoint', to prevent infinite recursion
	bool _drawing; //True=points will be set and saved; false= no new points will be set
	float _width, _height; //The width and height of the surface to be drawn upon
	Point2D _offset; // The absolute offset of the surface to the origin
	Point2D _lastPoint; //The last point that was outside the surface while drawing; Used for determining points that go inside again
	std::vector<Point2D> _points;//The list of points.
};

#endif

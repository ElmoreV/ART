#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle
{
public:
	float X,Y,W,H;//The x and y coordinates in the initial set of references and the width and height of the rectangle
	Rectangle(float positionX=0, float positionY=0, float width=0, float height=0);
	//Returns true when the point is inside the rectangle
	bool Contains(float pointX, float pointY);
	//Returns true when the rectangle 'bounds'  intersects with this rectangle
	bool Intersect(Rectangle bound);
};
#endif
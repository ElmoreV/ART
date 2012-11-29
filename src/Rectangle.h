#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle
{
public:
	float X,Y,W,H;
	Rectangle(float positionX=0, float positionY=0, float width=0, float height=0);
	bool Contains(float pointX, float pointY);
	bool Intersect(Rectangle bound);
};
#endif
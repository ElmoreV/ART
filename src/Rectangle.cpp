#include "Rectangle.h"

Rectangle::Rectangle(float positionX, float positionY, float width, float height):X(positionX),Y(positionY),W(width),H(height)
	{}
bool Rectangle::Contains(float pointX, float pointY){
	if(X<=pointX&&X+W>=pointX&&Y<=pointY&&Y+H>=pointY)return true;
	return false;
}
bool Rectangle::Intersect(Rectangle bound){
	if(Contains(bound.X, bound.Y)) return true;
	else if(Contains(bound.X, bound.Y + bound.H)) return true;
	else if(Contains(bound.X +bound.W, bound.Y)) return true;
	else if(Contains(bound.X + bound.W, bound.Y + bound.H)) return true;
	return false;
}
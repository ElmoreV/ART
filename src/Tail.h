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
	Tail();
	void HandleEvent(SDL_Event sEvent);
	void Update(Rectangle playerRect,HorizontalDirection playerDirection);
	void Draw(WindowSurface screen);
private:
	Point2D _base;
	Point2D _tip;
	Point2D _bezierCurve[101];
	Point2D _lastCursor;
};


#endif
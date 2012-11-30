#ifndef DRAWING_OBJECT_H
#define DRAWING_OBJECT_H

#include "Drawing.h"
#include "Rectangle.h"


class DrawingObject
{
public:
	DrawingObject(float width, float height, float x, float y);
	void Reset();
	void HandleEvent(SDL_Event sEvent,Rectangle playerBound);
	float CheckCollision(Rectangle ObjectRect);
	void Draw(WindowSurface sfScreen);
	inline VectorDraw& GetDrawing(){return _canvas;}
private:
	bool _cursorPressed;
	bool _cursorOnPlayer;
	bool _cursorOutOfRange;
	Point2D _prevCursor;
	VectorDraw _canvas;
};

#endif
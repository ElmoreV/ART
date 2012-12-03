#ifndef DRAWING_OBJECT_H
#define DRAWING_OBJECT_H

#include "Drawing.h"
#include "Rectangle.h"
#include "Assets.h"

class DrawingObject
{
public:
	DrawingObject(float width, float height, float x, float y);
	void Reset();
	void HandleEvent(SDL_Event sEvent);
	void Update(Rectangle playerBound);
	float CheckCollision(Rectangle ObjectRect);
	void Draw(WindowSurface sfScreen);
	inline VectorDraw& GetDrawing(){return _canvas;}
private:
	bool _cursorPressed;
	bool _cursorOnPlayer;
	bool _cursorOutOfRange;
	Point2D _prevCursor;
	Point2D _newCursor;
	VectorDraw _canvas;

};

#endif
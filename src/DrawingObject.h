#ifndef DRAWING_OBJECT_H
#define DRAWING_OBJECT_H

#include "Drawing.h"
#include "Rectangle.h"


class DrawingObject
{
public:
	DrawingObject(float width, float height, float x, float y);
	void HandleEvent(SDL_Event sEvent);
	float CheckCollision(Rectangle ObjectRect);
	void Draw(WindowSurface sfScreen);
	inline VectorDraw& GetDrawing(){return _canvas;}
private:
	bool _mousePressed;
	VectorDraw _canvas;
};


#endif
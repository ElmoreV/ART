#ifndef DRAWING_OBJECT_H
#define DRAWING_OBJECT_H

#include "Drawing.h"
#include "Rectangle.h"



//TODO: Decent drawingobject class. Easily manageable
class DrawingObject
{
public:
	DrawingObject(int width, int height, int x, int y);
	void HandleEvent(SDL_Event sEvent);
	bool CheckCollision(Rectangle ObjectRect);
	void Draw(WindowSurface sfScreen);
	inline VectorDraw& GetDrawing(){return _canvas;}
private:
	bool _mousePressed;
	VectorDraw _canvas;
};


#endif
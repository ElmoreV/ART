#ifndef DRAWING_H
#define DRAWING_H
#include <vector>
#include "Point.h"
#include "SDLFramework.h"
class VectorDraw
{
public:
	VectorDraw(int width, int height);
	void SetNewPoint(float x, float y);
	void ChangeOffset(int relativeX, int relativeY);
	void SetDrawMode(bool startDrawing=true);
	void Optimize(float precision);
	inline Point2D GetPoint(int index){return _points[index];};
	int GetSize(){return _points.size();}
	Point2D GetOffset(){return _offset;};
	void Clear(){_points.clear();}
private:
	unsigned int _recurseChecker;
	bool _drawing;
	int _width, _height;
	Point2D _offset;
	Point2D _lastPoint;
	std::vector<Point2D> _points;
};

//TODO: Decent drawingobject class. Easily manageable
class DrawingObject
{
public:
	DrawingObject(int width, int height):_canvas(width,height){_mousePressed=false;_canvas.SetDrawMode(false);};
	void HandleEvent(SDL_Event sEvent)
	{
		if (sEvent.type==SDL_MOUSEBUTTONDOWN)
		{

			_canvas.SetNewPoint(sEvent.button.x,sEvent.button.y);
			_mousePressed=true;

		}else if (sEvent.type==SDL_MOUSEMOTION)
		{

			if (_mousePressed)
			{
				_canvas.SetNewPoint(sEvent.button.x,sEvent.button.y);

			}
		}else if (sEvent.type==SDL_MOUSEBUTTONUP)
		{
			_mousePressed=false;
			_canvas.SetDrawMode(false);
		}else if (sEvent.type==SDL_KEYDOWN)
		{
				
			if (sEvent.key.keysym.sym==SDLK_RETURN)
			_canvas.Optimize(1);
			if (sEvent.key.keysym.sym==SDLK_DELETE)
			_canvas.Clear();
		}
	}
	void Draw(WindowSurface sfScreen)
	{
		int superSize=_canvas.GetSize();
		for (int i=1;i<superSize;i++)
		{
			Point2D point=_canvas.GetPoint(i);
			Point2D point2=_canvas.GetPoint(i-1);
			if (point.X==-0xffff||point2.X==-0xffff)
			{
			}else
			{
				Point2D off=_canvas.GetOffset();
				off.X=-off.X;
				off.Y=-off.Y;
				sfScreen.DrawLine(point.X-off.X-2,point.Y-off.Y,point2.X-off.X-2,point2.Y-off.Y,255,0,0);
				sfScreen.DrawLine(point.X-off.X-1,point.Y-off.Y-2,point2.X-off.X-1,point2.Y-off.Y-2,255,255,0);
				sfScreen.DrawLine(point.X-off.X,point.Y-off.Y-1,point2.X-off.X,point2.Y-off.Y-1,0,255,255);
				sfScreen.DrawLine(point.X-off.X+1,point.Y-off.Y+1,point2.X-off.X+1,point2.Y-off.Y+1,255,0,255);
				sfScreen.DrawLine(point.X-off.X+2,point.Y-off.Y+2,point2.X-off.X+2,point2.Y-off.Y+2,255,0,0);

			}
		}
	}
private:
	bool _mousePressed;
	VectorDraw _canvas;
	Surface _suffie;
};


#endif

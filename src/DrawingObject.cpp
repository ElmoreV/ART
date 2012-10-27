#include "DrawingObject.h"

DrawingObject::DrawingObject(int width, int height, int x, int y):_canvas(width,height)
{
	_mousePressed=false;
	_canvas.SetDrawMode(false);
	_canvas.SetOffset(x,y);
};
void DrawingObject::HandleEvent(SDL_Event sEvent)
{
	if (sEvent.type==SDL_MOUSEBUTTONDOWN)
	{
		_canvas.SetDrawMode(true);
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
void DrawingObject::Draw(WindowSurface sfScreen)
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
			sfScreen.DrawLine((int)(point.X-off.X-1),(int)(point.Y-off.Y-2),(int)(point2.X-off.X-1),(int)(point2.Y-off.Y-2),255,255,0);
			sfScreen.DrawLine((int)(point.X-off.X-2),(int)(point.Y-off.Y-1),(int)(point2.X-off.X-2),(int)(point2.Y-off.Y-1),255,0,0);
			sfScreen.DrawLine((int)(point.X-off.X),(int)(point.Y-off.Y),(int)(point2.X-off.X),(int)(point2.Y-off.Y-1),255,0,255);
			sfScreen.DrawLine((int)(point.X-off.X+1),(int)(point.Y-off.Y+1),(int)(point2.X-off.X+1),(int)(point2.Y-off.Y+1),255,255,255);
			sfScreen.DrawLine((int)(point.X-off.X+2),(int)(point.Y-off.Y+2),(int)(point2.X-off.X+2),(int)(point2.Y-off.Y+2),0,255,255);

		}
	}
}
bool DrawingObject::CheckCollision(SDL_Rect Object)
{
	//Get two points
	//Add it's offset
	//Check whether the object is actually in the drawing region
	//

	return false;
};
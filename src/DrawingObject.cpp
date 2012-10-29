#include "DrawingObject.h"

inline bool FloatEq(float a,float b){return (a-b>-0.0001&&a-b<0.0001);}

inline float GetXForYBetweenPoints(float Y, float x1, float y1, float x2, float y2)
{
	//x=(y-y1)*(x2-x1)/(y2-y1)+x1
	return (Y-y1)*(x2-x1)/(y2-y1)+x1;
}
inline float GetYForXBetweenPoints(float X, float x1, float y1, float x2, float y2)
{
	//y=(x-x1)*(y2-y1)/(x2-x1)+y1
	return (X-x1)*(y2-y1)/(x2-x1)+y1;
}

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
	Point2D objectPosition(Object.x,Object.y);
	float objectWidth=Object.w, objectHeight=Object.h;
	//Make the object's position relative
	objectPosition-=_canvas.GetOffset();
	/*CHECK IF THE SQUARES OVERLAP*/
	int x1 = Maximum(objectPosition.X, 0);
	int y1 = Maximum(objectPosition.Y, 0);
	int x2 = Minimum(objectPosition.X + objectWidth, _canvas.GetWidth());
	int y2 = Minimum(objectPosition.Y + objectHeight, _canvas.GetHeight());
	int width = x2 - x1; int height = y2 - y1;
	if(width > 0 && height > 0) 
	{
		for (unsigned int i=1;i<_canvas.GetSize();i++)
		{
			Point2D point1=_canvas.GetPoint(i-1);
			Point2D point2=_canvas.GetPoint(i);
			if (!FloatEq(point1.X,-0xFFFF)&&!FloatEq(point2.X,-0xFFFF))
			{
				//Calculate collision 
				float l=Minimum(point1.X,point2.X);
				float r=Maximum(point1.X,point2.X);
				float t=Minimum(point1.Y,point2.Y);
				float b=Maximum(point1.Y,point2.Y);
				int x1 = Maximum(objectPosition.X, l);
				int y1 = Maximum(objectPosition.Y, t);
				int x2 = Minimum(objectPosition.X + objectWidth, r);
				int y2 = Minimum(objectPosition.Y + objectHeight, b);
				//Get width and height of collisionpart
				int width = x2 - x1; int height = y2 - y1;
				//Return collisionpart
				if(width > 0 && height > 0) 
				{
					float mostTopL=GetYForXBetweenPoints(objectPosition.X,point1.X,point1.Y,point2.X,point2.Y);
					float mostTopR=GetYForXBetweenPoints(objectPosition.X+objectWidth,point1.X,point1.Y,point2.X,point2.Y);
					//If the calculated values are below the thingy, but also above the bottom
					if ((mostTopR>objectPosition.Y||mostTopL>objectPosition.Y)&&
						(mostTopR<objectPosition.Y+objectHeight||mostTopL<objectPosition.Y+objectHeight))
					{return true;}
				}
			}
		}
	}
	return false;
};
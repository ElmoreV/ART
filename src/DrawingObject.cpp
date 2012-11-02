#include "DrawingObject.h"
#include "Rectangle.h"

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

DrawingObject::DrawingObject(float width, float height, float x, float y):_canvas(width,height)
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
//Return the heighest point on the line to collide with the object rectangle. With no collision, it return false
float DrawingObject::CheckCollision(Rectangle ObjectRect)
{
	Point2D objectPosition(ObjectRect.X,ObjectRect.Y);
	objectPosition-=_canvas.GetOffset();
	Point2D objectPositionMax(objectPosition.X+ObjectRect.W,objectPosition.Y+ObjectRect.H);
	
	//First check if the object is touching the surface
	float x1 = Maximum(objectPosition.X, 0);
	float y1 = Maximum(objectPosition.Y, 0);
	float x2 = Minimum(objectPositionMax.X, _canvas.GetWidth());
	float y2 = Minimum(objectPositionMax.Y, _canvas.GetHeight());
	float width = x2 - x1; 
	float height = y2 - y1;
	if(width >= 0.0f && height >= 0.0f) 
	{
		float highestPoint=objectPositionMax.Y;
		for (unsigned int i=1;i<_canvas.GetSize();i++)
		{
			Point2D point1=_canvas.GetPoint(i-1);
			Point2D point2=_canvas.GetPoint(i);
			if (!FloatEq(point1.X,-0xFFFF)&&!FloatEq(point2.X,-0xFFFF))
			{
				//Make a box of the 2 points, and check if the object box is inside the 'point box'
				float l=Minimum(point1.X,point2.X);
				float t=Minimum(point1.Y,point2.Y);
				float r=Maximum(point1.X,point2.X);
				float b=Maximum(point1.Y,point2.Y);
				
				float mostLeft=Maximum(objectPosition.X, l);
				float mostTop=Maximum(objectPosition.Y, t);
				float mostRight=Minimum(objectPositionMax.X, r);
				float mostBot=Minimum(objectPositionMax.Y, b);
				float width = mostRight -mostLeft; 
				float height =mostBot - mostTop;
				if(width >= 0.0f && height >=0.0f) 
				{
					//Check if the line actually hits the object
					float mostTopL,mostTopR;
					if (!FloatEq(point1.X,point2.X))
					{
						//Calculate the Y-values for the left collision edge and the right collision edge
						mostTopL=GetYForXBetweenPoints(mostLeft,point1.X,point1.Y,point2.X,point2.Y);
						mostTopR=GetYForXBetweenPoints(mostRight,point1.X,point1.Y,point2.X,point2.Y);
					}else
					{
						//Otherwise, it's just the highest collision point
						mostTopL=mostTop;
						mostTopR=mostTop;
					}
					if ((mostTopR>=objectPosition.Y||mostTopL>=objectPosition.Y)&&
						(mostTopR<objectPositionMax.Y||mostTopL<objectPositionMax.Y))
					{
						//Calculate the heighest point on the line hitting the player, and store it.
						float tempHighestPoint=Minimum(mostTopR,mostTopL);
						if (tempHighestPoint<highestPoint)
						{highestPoint=tempHighestPoint;}
					}
				}
			}
		}
		if (!FloatEq(highestPoint,objectPositionMax.Y)&&highestPoint>=0.0f)
		{
			return objectPositionMax.Y-highestPoint;
		}
	}
	return false;
};
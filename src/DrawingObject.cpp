#include "DrawingObject.h"
#include "Rectangle.h"
#include <math.h>
inline bool FloatEq(float a,float b){return (a-b>-0.0001&&a-b<0.0001);}

Point2D GetIntersectionRayCircle(Point2D point1, Point2D point2, Point2D center, float radius)
{
	Point2D result(point1);
	Point2D directionVec=point2-point1;
	Point2D supportVec=point1-center;
	float a=directionVec.X*directionVec.X+directionVec.Y*directionVec.Y;
	float b=2*(directionVec.X*supportVec.X+directionVec.Y*supportVec.Y);
	float c=supportVec.X*supportVec.X+supportVec.Y*supportVec.Y-radius*radius;
	float discriminant=b*b-4*a*c;
	if (discriminant<0)
	{return result;}
	float disqrt=sqrt(discriminant);
	float t1=(-b+disqrt)/(2*a);
	float t2=(-b-disqrt)/(2*a);
	if (t1>=0.0f&&t1<=1.0f)
	{
		result.X=point1.X+t1*directionVec.X;
		result.Y=point1.Y+t1*directionVec.Y;
	}
	if (t2>=0.0f&&t2<=1.0f)
	{
		result.X=point1.X+t2*directionVec.X;
		result.Y=point1.Y+t2*directionVec.Y;
	}
	return result;
}

inline float GetYForXBetweenPoints(float X, float x1, float y1, float x2, float y2)
{
	//y=(x-x1)*(y2-y1)/(x2-x1)+y1
	return (X-x1)*(y2-y1)/(x2-x1)+y1;
}
float CheckCollisionForTwoPoints(Point2D point1, Point2D point2, Point2D objectPosition, Point2D objectPositionMax)
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
			return tempHighestPoint;
		}
	}
	return objectPositionMax.Y;
}
DrawingObject::DrawingObject(float width, float height, float x, float y):_canvas(width,height)
{
	_cursorPressed=false;
	_cursorOnPlayer=false;
	_cursorOutOfRange=false;
	_canvas.SetDrawMode(false);
	_canvas.SetOffset(x,y);
};
void DrawingObject::HandleEvent(SDL_Event sEvent,Rectangle playerBound)
{
	Point2D playerMiddle(playerBound.X+0.5f*playerBound.W, playerBound.Y+0.5f*playerBound.H);
	float outsideDrawRadius=100;
	Point2D cursor(sEvent.button.x,sEvent.button.y);
	Point2D cursorRelPlayer=playerMiddle;
	cursorRelPlayer-=cursor;
	Point2D lastPoint(-0xFFFF,-0xFFFF);
	if (_canvas.GetSize())
		lastPoint=_canvas.GetPoint(_canvas.GetSize()-1);
	Point2D relativePlayer=Point2D(playerBound.X,playerBound.Y);
	Point2D relativeMouse=cursor;
	relativeMouse-=_canvas.GetOffset();
	relativePlayer-=_canvas.GetOffset();
	Point2D relativePlayerMax=Point2D(relativePlayer.X+playerBound.W,relativePlayer.Y+playerBound.H);
	//cursor button down
	if (sEvent.type==SDL_MOUSEBUTTONDOWN)
	{
		Point2D newPoint(sEvent.button.x,sEvent.button.y);
		//Check if the cursor is outside the draw range
		if ((cursorRelPlayer.X)*(cursorRelPlayer.X)+(cursorRelPlayer.Y)*(cursorRelPlayer.Y)>outsideDrawRadius*outsideDrawRadius)
		{
			_cursorOutOfRange=true;

		}else
		{
			_cursorOutOfRange=false;
		}
		//Or if it's inside the minimal draw range
		if (cursor.X>playerBound.X-2&&cursor.Y>playerBound.Y-2&&cursor.X<playerBound.X+playerBound.W+2&&cursor.Y<playerBound.Y+playerBound.H)
		{_cursorOnPlayer=true;}

		if (lastPoint.X!=-0xFFFF)
		{
			if (!FloatEq(CheckCollisionForTwoPoints(lastPoint,relativeMouse,
				relativePlayer,relativePlayerMax),relativePlayerMax.Y))
			{_cursorOnPlayer=true;}
		}

		if (!_cursorOnPlayer&&!_cursorOutOfRange)
		{
			_canvas.SetDrawMode(true);
		}
		_canvas.SetNewPoint(newPoint.X,newPoint.Y);
		_cursorPressed=true;

	}else if (sEvent.type==SDL_MOUSEMOTION)
	{
		Point2D newPoint(sEvent.button.x,sEvent.button.y);
		bool cursorIsOnPlayer=false;
		bool cursorIsOutOfRange=false;
		//Check if it's outside of the draw range. Stop drawing if so. Restart drawing if it WAS true, but not anymore
		if ((cursorRelPlayer.X)*(cursorRelPlayer.X)+(cursorRelPlayer.Y)*(cursorRelPlayer.Y)>outsideDrawRadius*outsideDrawRadius)
		{cursorIsOutOfRange=true;}

		//Check if it's inside of the minimal draw range
		if (cursor.X>playerBound.X-2&&cursor.Y>playerBound.Y-2&&cursor.X<playerBound.X+playerBound.W+2&&cursor.Y<playerBound.Y+playerBound.H)
		{cursorIsOnPlayer=true;}

		//Check if you're not drawing through the character
		if (!FloatEq(lastPoint.X,-0xFFFF))
		{
			if (!FloatEq(CheckCollisionForTwoPoints(lastPoint,relativeMouse,
				relativePlayer,relativePlayerMax),relativePlayerMax.Y))
			{cursorIsOnPlayer=true;}
		}else
		{
			Point2D referenceOutside=_canvas.GetOutsideReferencePoint();
			if (referenceOutside.X!=-0xFFFF)
			{
				if (!FloatEq(CheckCollisionForTwoPoints(referenceOutside,relativeMouse,	relativePlayer,relativePlayerMax),relativePlayerMax.Y))
				{cursorIsOnPlayer=true;}
			}
		}

		if (cursorIsOutOfRange)
		{
			if (_cursorPressed)
			{
				_cursorOutOfRange=true;
				if (!cursorIsOnPlayer)
				{
					if (lastPoint.X!=-0xFFFF)
					{
						newPoint=GetIntersectionRayCircle(lastPoint+_canvas.GetOffset(),newPoint,playerMiddle,outsideDrawRadius);
						_canvas.SetNewPoint(newPoint.X,newPoint.Y);
					}else
					{
						Point2D lastReferencePoint=_canvas.GetOutsideReferencePoint();
						if (lastReferencePoint.X!=-0xFFFF)
						{
							newPoint=GetIntersectionRayCircle(lastReferencePoint+_canvas.GetOffset(),newPoint,playerMiddle,outsideDrawRadius);
							_canvas.SetNewPoint(newPoint.X,newPoint.Y);
						}
					}
				}
				_canvas.SetDrawMode(false);
			}
		}
		else
		{
			if (_cursorOutOfRange)
			{
				_cursorOutOfRange=false;
				if (!cursorIsOnPlayer)
				{_canvas.SetDrawMode(true);}
			}
		}
		//If the line to be drawn goes on/through the player
		//Stop drawing
		//If it was true, but not anymore
		//Restart drawing
		if (cursorIsOnPlayer)
		{
			if (_cursorPressed)
			{
				_cursorOnPlayer=true;
				_canvas.SetDrawMode(false);
			}
		}else
		{
			if (_cursorOnPlayer)
			{
				if (!cursorIsOutOfRange)
				{_canvas.SetDrawMode(true);}
				_cursorOnPlayer=false;
			}
		}
		//Only add new points when the cursor is pressed
		if (_cursorPressed)
		{
			_canvas.SetNewPoint(newPoint.X,newPoint.Y);
		}
	}else if (sEvent.type==SDL_MOUSEBUTTONUP)
	{
		_cursorOnPlayer=false;
		_cursorOutOfRange=false;
		_cursorPressed=false;
		_canvas.SetDrawMode(false);

	}else if (sEvent.type==SDL_KEYDOWN)
	{

		if (sEvent.key.keysym.sym==SDLK_RETURN)
			_canvas.Optimize(0.5f);
		if (sEvent.key.keysym.sym==SDLK_DELETE)
		{	_canvas.Clear();
		_cursorOnPlayer=false;
		_cursorOutOfRange=false;
		_cursorPressed=false;
		_canvas.SetDrawMode(false);
		}
		if (sEvent.key.keysym.sym==SDLK_INSERT)
		{
			_canvas.SetDrawMode();
			_canvas.SetNewPoint(0,3*36+4);
			_canvas.SetNewPoint(200,3*36+4);
			_canvas.SetDrawMode(false);
		}
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
				float tempHighestPoint=CheckCollisionForTwoPoints(point1,point2,objectPosition,objectPositionMax);
				if (tempHighestPoint<highestPoint)
						{highestPoint=tempHighestPoint;}
			}
		}
		if (!FloatEq(highestPoint,objectPositionMax.Y)&&highestPoint>=0.0f)
		{
			return objectPositionMax.Y-highestPoint;
		}
	}
	return false;
};
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
void DrawingObject::Reset()
{
	_cursorPressed=false;
	_cursorOnPlayer=false;
	_cursorOutOfRange=false;
	_canvas.SetDrawMode(false);
	_canvas.Clear();
	_prevCursor.X=0;
	_prevCursor.Y=0;
}
void DrawingObject::HandleEvent(SDL_Event sEvent)
{
	if (sEvent.type==SDL_MOUSEBUTTONDOWN)
	{
		_prevCursor=Point2D(-0xffff,-0xffff);
		_newCursor=Point2D(sEvent.button.x,sEvent.button.y);
		_cursorPressed=true;
	}else if (sEvent.type==SDL_MOUSEMOTION)
	{
		_prevCursor=_newCursor;
		_newCursor=Point2D(sEvent.button.x,sEvent.button.y);
	}else if(sEvent.type==SDL_MOUSEBUTTONUP)
	{
		_prevCursor=Point2D(-0xffff,-0xffff);
		_newCursor=Point2D(-0xffff,-0xffff);
		_cursorPressed=false;
		_cursorOutOfRange=false;
		_cursorOnPlayer=false;
	}else if (sEvent.type==SDL_KEYDOWN)
	{

		if (sEvent.key.keysym.sym==SDLK_RETURN)
			_canvas.Optimize(0.5f);
		if (sEvent.key.keysym.sym==SDLK_DELETE)
		{	Reset();
		}
		if (sEvent.key.keysym.sym==SDLK_INSERT)
		{
			_canvas.SetDrawMode();
			//Can be used for quick line testing
			_canvas.SetDrawMode(false);
		}
	}
}
void DrawingObject::SetDrawMode(bool drawing)
{
	_drawing=drawing;
};
void DrawingObject::Update(Rectangle playerBound)
{
	Point2D playerMiddle(playerBound.X+0.5f*playerBound.W, playerBound.Y+0.5f*playerBound.H);
	float outsideDrawRadius=100;
	bool newCursorIsOutOfRange=false;
	bool newCursorIsOnPlayer=false;
	
	if (_cursorPressed)
	{
		//Check if it's outside the maximum range
		if ((_newCursor.X-playerMiddle.X)*(_newCursor.X-playerMiddle.X)+(_newCursor.Y-playerMiddle.Y)*(_newCursor.Y-playerMiddle.Y)>outsideDrawRadius*outsideDrawRadius)
		{newCursorIsOutOfRange=true;}
		//Or if it's inside the minimal draw range
		if (_newCursor.X>playerBound.X-2&&_newCursor.Y>playerBound.Y-2&&_newCursor.X<playerBound.X+playerBound.W+2&&_newCursor.Y<playerBound.Y+playerBound.H)
		{newCursorIsOnPlayer=true;}
		//Check if the line is drawn through the player
		if (!newCursorIsOnPlayer&&_prevCursor.X!=-0xFFFF)
		{
			if (!FloatEq(CheckCollisionForTwoPoints(_prevCursor,_newCursor,
				Point2D(playerBound.X,playerBound.Y),
				Point2D(playerBound.X+playerBound.W,playerBound.Y+playerBound.H)),playerBound.Y+playerBound.H))
			{newCursorIsOnPlayer=true;}
		}

		if (_cursorOutOfRange)
		{
			if(!newCursorIsOutOfRange)
			{
				//The cursor went from outside to inside
				_cursorOutOfRange=false;
				if (!newCursorIsOnPlayer&&_drawing)
				{
					_canvas.SetDrawMode(true);
					if (_prevCursor.X!=-0xFFFF)
					{
						Point2D intersect=GetIntersectionRayCircle(_prevCursor,_newCursor,playerMiddle,outsideDrawRadius);
						_canvas.SetNewPoint(intersect.X, intersect.Y);
					}				
				}
			}else
			{
				//From outside to outside
				if (_prevCursor.X!=-0xFFFF&&!newCursorIsOnPlayer&&_drawing)
				{
					Point2D intersect1=GetIntersectionRayCircle(_prevCursor,_newCursor,playerMiddle,outsideDrawRadius);
					if (!FloatEq(intersect1.X,_prevCursor.X)&&!FloatEq(intersect1.Y,_prevCursor.Y))
					{
						//It actually goes through the circle and goes outside again
						intersect1.X*=0.99f;
						intersect1.X+=0.01f*_newCursor.X;
						intersect1.Y*=0.99f;
						intersect1.Y+=0.01f*_newCursor.X;
						Point2D intersect2=GetIntersectionRayCircle(intersect1,_newCursor,playerMiddle,outsideDrawRadius);
						
						_canvas.SetDrawMode(true);
						_canvas.SetNewPoint(intersect1.X,intersect1.Y);
						_canvas.SetNewPoint(intersect2.X,intersect2.Y);
					}
				}
				_canvas.SetDrawMode(false);
			}
		}else
		{
			if (newCursorIsOutOfRange)
			{
				//The cursor went from inside to outside
				_cursorOutOfRange=true;
				
				if (!newCursorIsOnPlayer)
				{
					if (_prevCursor.X!=-0xFFFF)
					{
						Point2D intersect=GetIntersectionRayCircle(_prevCursor,_newCursor,playerMiddle,outsideDrawRadius);
						_canvas.SetNewPoint(intersect.X, intersect.Y);
					}
				}
				_canvas.SetDrawMode(false);	
			}else
			{
				//Still inside
			}
		}
		

		if (newCursorIsOnPlayer)
		{
			if (_cursorOnPlayer)
			{
				//Still on player
			}else
			{
				//Went outside the player on the player
				_cursorOnPlayer=true;
				_canvas.SetDrawMode(false);
			}
		}else
		{
			if (_cursorOnPlayer)
			{
				//Was on player, not anymore
				_cursorOnPlayer=false;
				if (!newCursorIsOutOfRange&&_drawing)
				{_canvas.SetDrawMode(true);}
			}else
			{
				// Not on player or through
			}
		}
		if (!_cursorOnPlayer&&!_cursorOutOfRange&&_drawing)
		{
			_canvas.SetDrawMode(true);
		}
		if (!_drawing)
		{
			_canvas.SetDrawMode(false);
		}
		_canvas.SetNewPoint(_newCursor.X,_newCursor.Y);
	}else
	{
		//The cursor isn't pressed
		_canvas.SetDrawMode(false);
	}

};
void DrawingObject::Draw(WindowSurface sfScreen)
{
	int vectorSize=_canvas.GetSize();
	for (int i=1;i<vectorSize;i++)
	{
		Point2D point=_canvas.GetPoint(i);
		Point2D point2=_canvas.GetPoint(i-1);
		if (point.X==-0xffff||point2.X==-0xffff||
			(FloatEq(point.X,point2.X)&&FloatEq(point.Y,point2.Y))
			)
		{
		}else
		{
			Point2D off(0,0);
			off-=_canvas.GetOffset();
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
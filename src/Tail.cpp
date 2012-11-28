#include "Tail.h"

enum HorizontalDirection
{
	HDirLeft,
	HDirRight,
	HDirNone
};


Point2D GetIntersectionRayCircle2(Point2D point1, Point2D point2, Point2D center, float radius)
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
Tail::Tail():_tip(0.0,0.0),_base(0.0,0.0){}

void Tail::Draw(WindowSurface screen)
{
	//Draw the different point, giving some thickness near the base
	for (int i=1;i<=100;i++)
	{
		if (i>50)
		{
			screen.DrawLine(_bezierCurve[i-1].X+1,_bezierCurve[i-1].Y+1,_bezierCurve[i].X+1,_bezierCurve[i].Y+1,200,100,100);
		}
		if (i>75)
		{
			screen.DrawLine(_bezierCurve[i-1].X+1,_bezierCurve[i-1].Y-1,_bezierCurve[i].X+1,_bezierCurve[i].Y-1,155,75,75);
			screen.DrawLine(_bezierCurve[i-1].X+1,_bezierCurve[i-1].Y+2,_bezierCurve[i].X+1,_bezierCurve[i].Y+2,155,75,75);
		}
		screen.DrawLine(_bezierCurve[i-1].X,_bezierCurve[i-1].Y,_bezierCurve[i].X,_bezierCurve[i].Y,255,155,155);
	}
}
void Tail::Update(Rectangle playerRect,HorizontalDirection playerDirection)
{
	//1. Get the base of the tail (need the player bound for that)
	_base.Y=playerRect.Y+24;
	_base.X=playerDirection==HDirLeft?playerRect.X+playerRect.W:playerRect.X;
	//2. Determine the tip of the tail (using the cursor input, and the maximum range)
	Point2D playerCenter(playerRect.X+playerRect.W*0.5,playerRect.Y+playerRect.H*0.5);
	Point2D cursorRelPlayer=playerCenter-_lastCursor;
	float outsideRadius=100;
	bool cursorOutOfRange=false;
	////2b. Check if it interferes with the circle. If so, make it touch the circle wall.
	if (cursorRelPlayer.X*cursorRelPlayer.X+cursorRelPlayer.Y*cursorRelPlayer.Y>outsideRadius*outsideRadius)
	{
		cursorOutOfRange=true;
		_tip=GetIntersectionRayCircle2(_base,_lastCursor,playerCenter,outsideRadius);
	}else
	{
		_tip=_lastCursor;
	}
	//Calculate a Bezier curve, translating 70 pixels away from the mouse
	Point2D middlePoint(0.5*(_base.X+_tip.X),0.5*(_base.Y+_tip.Y));//();
	if (playerDirection==HDirLeft)
	{middlePoint.X=_base.X+70;}
	else if (playerDirection==HDirRight)
	{middlePoint.X=_base.X-70;}
	
	
	for (int i=0;i<=100;i++)
	{
		float a=(float)i/100.0f;
		_bezierCurve[i].X=
			a*a*_base.X+
			2*(1-a)*a*middlePoint.X+
			(1-a)*(1-a)*_tip.X;
		_bezierCurve[i].Y=
			(a)*(a)*_base.Y+
			2*(1-a)*a*middlePoint.Y+
			(1-a)*(1-a)*_tip.Y;
	}


}
void Tail::HandleEvent(SDL_Event sEvent)
{
	if (sEvent.type==SDL_MOUSEMOTION||sEvent.type==SDL_MOUSEBUTTONDOWN)
	{
		////2a. Get the mouse input
		
		_lastCursor.X=sEvent.button.x;
		_lastCursor.Y=sEvent.button.y;
	}
	
}
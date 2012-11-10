#include "Drawing.h"

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
inline float GetGradient(float x1, float y1, float x2, float y2)
{
	return (y2-y1)/(x2-x1);
}

inline Point2D ResolveEdgePointWhenOutside(Point2D outside,Point2D point2, float width, float height)
{
	float nextX=0,nextY=0;
	float tempX=outside.X, tempY=outside.Y;
	if (outside.X<0.0||outside.X>=width)
	{
		//X=0, or X=_width, depending on which boundary is crossed by surfaceX
		nextX=outside.X<0.0?0.0f:width-0.001f;
		nextY=::GetYForXBetweenPoints(nextX,outside.X,outside.Y,point2.X,point2.Y);
		tempX=nextX; tempY=nextY;
	}
	if (tempY<0.0||tempY>=height)
	{
		//y=0 or y=_height, depending on crossing top or bottom
		nextY=tempY<0.0?0.0f:height-0.001f;
		nextX=GetXForYBetweenPoints(nextY,tempX,tempY,point2.X,point2.Y);
	}
	return Point2D(nextX,nextY);
}


VectorDraw::VectorDraw(float width, float height):_width(width),_height(height),_lastPoint(-0xFFFF,-0xFFFF),_drawing(false)
	,_recurseChecker(0)
{
	Point2D start(-0xFFFF,-0xFFFF);
	_points.push_back(start);
}
//Sets a new point when inside the square, and remembers the last point outside the boundaries for 
void VectorDraw::SetNewPoint(float x, float y)
{
	//If you weren't drawing, disregard this
	if (!_drawing)
	{return;}

	if (_recurseChecker++>3)
	{_recurseChecker=0;
	return;}

	//if you draw at screenX=300, and the surface is turned 100px to the left (-100)
	//You actually draw at X=300- -100  =400
	float surfaceX=x-_offset.X;
	float surfaceY=y-_offset.Y;
	//Check if it's inside the boundaries
	if (surfaceX<0||surfaceY<0||surfaceX>=_width||surfaceY>=_height)
	{
		//The new point is outside the surface

		if (FloatEq(_lastPoint.X,-0xFFFF))//if the last point was undefined (because it was inside)
		{
			//Find the last point inside the surface on the line between this point and the other point
			//Search in every quadrant
			//with insidePoint (a,b)
			//and outsidePoint(c,d)
			Point2D inPoint=_points[_points.size()-1];

			//You needed to be already drawing (so no 'stop' point).
			if (!FloatEq(inPoint.X,-0xFFFF))
			{
				Point2D edgePoint=ResolveEdgePointWhenOutside(Point2D(surfaceX,surfaceY),inPoint,_width,_height);
				SetNewPoint(edgePoint.X+_offset.X,edgePoint.Y+_offset.Y);
			}

		}else// If both points were outside
		{
			//If any of the two points are on the same side, do nothing
			if (!((surfaceX<0.0&&_lastPoint.X<0.0)
				||(surfaceY<0.0&&_lastPoint.Y<0.0)
				||(surfaceX>=_width&&_lastPoint.X>=_width)
				||(surfaceY>=_height&&_lastPoint.Y>=_height)))
			{
				Point2D edgePoint=ResolveEdgePointWhenOutside(Point2D(surfaceX,surfaceY),_lastPoint,_width,_height);
				SetNewPoint(edgePoint.X+_offset.X,edgePoint.Y+_offset.Y);
			}
		}
		//This point needs to be remembered for when it goes inside again
		_lastPoint.X=surfaceX;
		_lastPoint.Y=surfaceY;

		//The new point is outside, so we indicate it with an 'stop' marker, if it's not already there
		if (_points[_points.size()-1].X!=-0xFFFF)
		{
			Point2D stopPoint(-0xFFFF,-0xFFFF);
			_points.push_back(stopPoint);
		}
	}else
	{
		//The new point is inside the surface
		//The last point was defined (so outside the surface), get the line, and set the last point to undefined
		if (_lastPoint.X!=-0xFFFF)
		{
			Point2D edgePoint=ResolveEdgePointWhenOutside(_lastPoint,Point2D(surfaceX,surfaceY),_width,_height);
			if (edgePoint.X>=0&&edgePoint.Y>=0&&edgePoint.X<_width&&edgePoint.Y<_height)
			{
				_lastPoint.X=-0xFFFF;
				_lastPoint.Y=-0xFFFF;
			}
			SetNewPoint(edgePoint.X+_offset.X,edgePoint.Y+_offset.Y);
		}
		//It's inside, so just plot the point
		Point2D newPoint(surfaceX,surfaceY);
		_points.push_back(newPoint);

	}
	_recurseChecker=0;
}
void VectorDraw::SetOffset(float x, float y)
{
	_offset.X=x;
	_offset.Y=y;
	if (_drawing)
	{
		SetDrawMode(false);
	}
};
void VectorDraw::ChangeOffset(float relativeX, float relativeY)
{
	//Change the offset by adding the relativeX/Y (which in this case IS the surface velocity)
	_offset.X+=relativeX;
	_offset.Y+=relativeY;
	//When you are drawing, also set a new point
	if (_drawing)
	{
		//Get last point as reference
		Point2D endPoint=_points[_points.size()-1];
		//If last point is stop point, get the 'outside'pointer for it.
		if (endPoint.X==-0xFFFF)
		{endPoint=_lastPoint;}
		//if (endPoint.X)
		SetNewPoint(endPoint.X+_offset.X-relativeX,endPoint.Y+_offset.Y-relativeY);
	}
}
void VectorDraw::SetDrawMode(bool startDrawing)
{
	//If you stop drawing, indicate it!
	if (!startDrawing)
	{
		//Also, delete the 'last point'reference
		_lastPoint.X=-0xFFFF;
		_lastPoint.Y=-0xFFFF;
		//If it isn't already there, put a stop indicator
		if (_points[_points.size()-1].X!=-0xFFFF)
		{
			Point2D stopPoint(-0xFFFF,-0xFFFF);
			_points.push_back(stopPoint);
		}
	}
	_drawing=startDrawing;
}
void VectorDraw::Optimize(float precision)
{
	unsigned int vectorSize=_points.size();
	if (vectorSize<2){return;}
	Point2D beginPoint;
	Point2D middlePoint;
	Point2D endPoint;

	for (unsigned int i=0;i+2<vectorSize;i++)
	{
		beginPoint=_points[i];
		middlePoint=_points[i+1];
		endPoint=_points[i+2];
		//Check if the last two are the same, cause that would be silly
		if (endPoint.X==middlePoint.X&&endPoint.Y==middlePoint.Y)
		{
			_points.erase(_points.begin()+i+2);//the endPoint
			vectorSize--;
			//Check this one again
			i--;
		}
		//Check if all three are valid
		else if (beginPoint.X!=-0xffff&&middlePoint.X!=-0xffff&&endPoint.X!=-0xffff)
		{
			//Check if the middle can be put out between
			//zDiff1=middle-begin; zDiff2=end-middle
			float xDiff1=middlePoint.X-beginPoint.X;
			float yDiff1=middlePoint.Y-beginPoint.Y;
			float xDiff2=endPoint.X-middlePoint.X;
			float yDiff2=endPoint.Y-middlePoint.Y;
			if (xDiff1*yDiff2==0)
			{
				if (xDiff1==0&&xDiff2==0)
				{
					//If the drawing doesn't suddenly go the other direction
					if (!((yDiff1>0&&yDiff2<0)||(yDiff2>0&&yDiff1<0)))
					{
						_points.erase(_points.begin()+i+1);//the middlePoint
						vectorSize--;
						//Check this one again
						i--;
					}
				}else if (yDiff1==0&&yDiff2==0)
				{
					if (!((xDiff1>0&&xDiff2<0)||(xDiff2>0&&xDiff1<0)))
					{
						_points.erase(_points.begin()+i+1);//the middlePoint
						vectorSize--;
						//Check this one again
						i--;
					}
				}
			}else
			{
				float equalSteepness=(yDiff1*xDiff2)/(yDiff2*xDiff1);
				if (equalSteepness>=precision&&equalSteepness<=1/precision)
				{
					_points.erase(_points.begin()+i+1);//the middlePoint
					vectorSize=_points.size();
					//Check this one again
					i--;
				}
			}
		}else
		{
			//Check if it could be just a dot in nowhere...
			if (beginPoint.X==-0xffff&&middlePoint.X!=-0xffff&&endPoint.X==-0xffff)
			{
				_points.erase(_points.begin()+i+2);//the endpoint
				_points.erase(_points.begin()+i+1);//the middlepoint
				vectorSize-=2;
				i--;
			}
		}

	};
}
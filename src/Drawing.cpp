#include "Drawing.h"

VectorDraw::VectorDraw(int width, int height):_width(width),_height(height),_lastPoint(-0xFFFF,-0xFFFF),_drawing(false)
		,_recurseChecker(0)
	{
		Point2D start(-0xFFFF,-0xFFFF);
		_points.push_back(start);
	}
//Sets a new point when inside the square, and remembers the last point outside the boundaries for 
void VectorDraw::SetNewPoint(float x, float y)
	{
		if (_recurseChecker++>3)
		{_recurseChecker=0;return;}
		//If you weren't drawing, you sure are now!
		if (!_drawing){_drawing=true;}
		//if you draw at screenX=300, and the surface is turned 100x to the left (-100)
		//You actually draw at X=400
		float offX=x-_offset.X;
		float offY=y-_offset.Y;
		//Check if it's inside the boundaries
		if (offX<0||offY<0||offX>=_width||offY>=_height)
		{
			//It's outside
			//If it wasn't already outside
			if (_lastPoint.X==-0xFFFF)
			{
				//Find the last point inside the surface on the line between this point and the other point
				//Search in every quadrant
				//with insidePoint (a,b)
				//and outsidePoint(c,d)
				Point2D inPoint=_points[_points.size()-1];
				float nextX=0,nextY=0;
				if (inPoint.X!=-0xFFFF)//If you were actually already drawing
				{
					if (offX<0.0||offX>=_width)
					{
						//X=0, or X=_width, depending on which boundary is crossed by offX
						nextX=offX<0.0?0:_width-1;
						//y=(X-c)*(b-d)/(a-c)+d
						nextY=(nextX-offX)*(inPoint.Y-offY)/(inPoint.X-offX)+offY;

					}
					else if (offY<0||offY>=_height)
					{
						//y=0 or y=_height, depending on crossing top or bottom
						nextY=offY<0.0?0:_height-1;
						//x=(x-c)*(a-c)/(b-d)+c
						nextX=(nextY-offY)*(inPoint.X-offX)/(inPoint.Y-offY)+offX;
					}
					SetNewPoint(nextX+_offset.X,nextY+_offset.Y);
				}

			}
			//Anyway, just update the values
			_lastPoint.X=offX;
			_lastPoint.Y=offY;
			//Oh, and add a point indicating not to continue drawing this shizzle, if it's not already done
			if (_points[_points.size()-1].X!=-0xFFFF)
			{
				Point2D stopPoint(-0xFFFF,-0xFFFF);
				_points.push_back(stopPoint);
			}
		}else
		{
			//If it was outside before, get the line, and set the last point to undefined
			if (_lastPoint.X!=-0xFFFF)
			{
				int nextX=0, nextY=0;
				if (_lastPoint.X<0.0||_lastPoint.X>=_width)
				{
					//Point insidePoint=_points[_points.size()-1];
					//X=0, or X=_width, depending on which boundary is crossed by offX
					nextX=_lastPoint.X<0.0?0:_width-1;
					//y=(X-c)*(b-d)/(a-c)+d
					nextY=(nextX-offX)*(_lastPoint.Y-offY)/(_lastPoint.X-offX)+offY;
					
				}
				else if (_lastPoint.Y<0||_lastPoint.Y>=_height)
				{
					//Point insidePoint=_points[_points.size()-1];
					//y=0 or y=_height, depending on crossing top or bottom
					nextY=_lastPoint.Y<0.0?0:_height-1;
					//x=(x-c)*(a-c)/(b-d)+c
					nextX=(nextY-offY)*(_lastPoint.X-offX)/(_lastPoint.Y-offY)+offX;
				}
				_lastPoint.X=-0xFFFF;
				_lastPoint.Y=-0xFFFF;
				SetNewPoint(nextX+_offset.X,nextY+_offset.Y);
				
			}
			//It's inside, so just plot the point
			Point2D newPoint(offX,offY);
			_points.push_back(newPoint);
			
		}
		_recurseChecker=0;
	}
void VectorDraw::ChangeOffset(int relativeX, int relativeY)
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
			if (endPoint.X==-0xFFFF){endPoint=_lastPoint;}
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
			//Font font;
			//font.OpenFont("Another.ttf");
			//font.SetColor(0xff,0xff,0xff);
			//std::stringstream integer; integer<<beginPoint.X<<","<<beginPoint.Y<<" "
			//	<<middlePoint.X<<","<<middlePoint.Y<<" "
			//	<<endPoint.X<<","<<endPoint.Y;
			//std::string str; str=integer.str();
			//bamboozle.RenderText(font,str.c_str());
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
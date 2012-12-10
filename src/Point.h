#ifndef POINT2D_H
#define POINT2D_H

class Point2D {
public:
	float X, Y; //The x and y coordinates in a certain initial reference frame
	//Creates a point with x and y coordinates
	Point2D(float x=0, float y=0);
	//Add the x and the y of the point to add to this point
	inline Point2D& operator +=(Point2D pointToAdd){X+=pointToAdd.X;Y+=pointToAdd.Y;return *this;}
	//Substract the x and the y of the point to substract to this point
	inline Point2D& operator -=(Point2D pointToSubstract){X-=pointToSubstract.X;Y-=pointToSubstract.Y; return *this;}
	//Add the x and the y of two points together and return it to continue the expression
	inline Point2D operator +(Point2D pointToAdd){return Point2D(X+pointToAdd.X,Y+pointToAdd.Y);}
	//Substract the x and the y of two points from each other and return it to continue the expression
	inline Point2D operator -(Point2D pointToSubstract){return Point2D(X-pointToSubstract.X,Y-pointToSubstract.Y);}
	//Multiply the x and the y of two points and return it to continue the expression
	inline Point2D operator *(Point2D point){return Point2D(X*point.X,Y*point.Y);}
	//Divide the x and the y of this point by the other and return it to continue the expression
	inline Point2D operator /(Point2D point){return Point2D(X/point.X,Y/point.Y);}
};
#endif
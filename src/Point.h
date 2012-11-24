#ifndef POINT2D_H
#define POINT2D_H

class Point2D {
public:
	float X, Y;
	//Point2D();
	Point2D(float x=0, float y=0);
	inline Point2D& operator +=(Point2D pointToAdd){X+=pointToAdd.X;Y+=pointToAdd.Y;return *this;}
	inline Point2D& operator -=(Point2D pointToSubstract){X-=pointToSubstract.X;Y-=pointToSubstract.Y; return *this;}
	inline Point2D operator +(Point2D pointToAdd){return Point2D(X+pointToAdd.X,Y+pointToAdd.Y);}
	inline Point2D operator -(Point2D pointToSubstract){return Point2D(X-pointToSubstract.X,Y-pointToSubstract.Y);}
};
#endif
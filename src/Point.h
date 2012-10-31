#ifndef POINT2D_H
#define POINT2D_H

class Point2D {
public:
	float X, Y;
	//Point2D();
	Point2D(float x=0, float y=0);
	inline void operator +=(Point2D pointToAdd){X+=pointToAdd.X;Y+=pointToAdd.Y;}
	inline void operator -=(Point2D pointToSubstract){X-=pointToSubstract.X;Y-=pointToSubstract.Y;}
};
#endif
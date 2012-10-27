#ifndef DRAWING_H
#define DRAWING_H
#include <vector>
#include "Point.h"
#include "SDLFramework.h"
class VectorDraw
{
public:
	VectorDraw(int width, int height);
	void SetNewPoint(float x, float y);
	void SetOffset(float X, float Y);
	void ChangeOffset(float relativeX, float relativeY);
	void SetDrawMode(bool startDrawing=true);
	void Optimize(float precision);
	inline Point2D GetPoint(int index){return _points[index];};
	int GetSize(){return _points.size();}
	Point2D GetOffset(){return _offset;};
	void Clear(){_points.clear();}
private:
	unsigned int _recurseChecker;
	bool _drawing;
	int _width, _height;
	Point2D _offset;
	Point2D _lastPoint;
	std::vector<Point2D> _points;
};

#endif

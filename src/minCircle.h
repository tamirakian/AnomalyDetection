#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle {
public:
	Point center;
	float radius;
	Circle(Point c, float r) :center(c), radius(r) {}
};
// --------------------------------------

float getRadius(const Point& p1, const Point& p2);

float getRadiusFrom3Points(const Point& center, const Point& boundryPoint);

Point getCenterFrom3Points(vector<Point>& circleBoundry);

bool isPointInsideCircle(const Point& p, const Circle& c);

bool isCircle(const Circle& c, const vector<Point>& points, int pointsSize);

Circle getCircleFrom2Points(const Point& p1, const Point& p2);

Circle circleCalc(vector<Point>& circleBoundry, int boundrySize);

Circle welzlAlg(vector<Point>& points, size_t pointsSize, vector<Point> circleBoundry, int boundrySize);

Circle findMinCircle(Point** points, size_t size);

#endif /* MINCIRCLE_H_ */
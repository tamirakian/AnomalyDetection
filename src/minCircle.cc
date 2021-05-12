#include "minCircle.h"
#include <iostream>
#include <vector>
#include <math.h>
#define EPSILON 0.000001

using namespace std;

// getting the radius by dividing by 2 the diameter
float getRadius(const Point& p1, const Point& p2)
{
    //distance formula for computing the diameter.
    return (sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2))) / 2;
}

// returns the distance from the center to one of the point on the boundry (AKA the radius).
float getRadiusFrom3Points(const Point& center, const Point& boundryPoint)
{
    //distance formula.
    return (sqrt(pow((center.x - boundryPoint.x), 2) + pow((center.y - boundryPoint.y), 2)));
}

// computing the center by the equation of circle enclosing a triangle
Point getCenterFrom3Points(vector<Point>& circleBoundry)
{
    //initial calculations
    Point p1 = circleBoundry.at(0);
    float x1 = p1.x;
    float y1 = p1.y;
    Point p2 = circleBoundry.at(1);
    float x2 = p2.x;
    float y2 = p2.y;
    Point p3 = circleBoundry.at(2);
    float x3 = p3.x;
    float y3 = p3.y;
    // getting the slope of 1 line of the triangle
    float mP1P2 = (y2 - y1) / (x2 - x1);
    // getting the mid point of the first line
    Point midP1P2((x1 + x2) / 2, (y1 + y2) / 2);
    // getting the perpendicular line slope of the first line
    float perpMP1P2 = (-1) / mP1P2;
    // getting the constant variable of the first perpendicular line
    float perpNP1P2 = midP1P2.y - perpMP1P2 * midP1P2.x;
    //creating the first perpendicular line
    Line perpP1P2(perpMP1P2, perpNP1P2);
    // same idea for the second line
    float mP1P3 = (y3 - y1) / (x3 - x1);
    Point midP1P3((x1 + x3) / 2, (y1 + y3) / 2);
    float perpMP1P3 = (-1) / mP1P3;
    float perpNP1P3 = midP1P3.y - perpMP1P3 * midP1P3.x;
    Line perpP1P3(perpMP1P3, perpNP1P3);
    // getting the center by interssction of the lines
    float centerX = (perpNP1P3 - perpNP1P2) / (perpMP1P2 - perpMP1P3);
    // putting the center's x value inside the formula of the first line
    float centerY = perpP1P2.f(centerX);
    return Point(centerX, centerY);
}

// checking if the point is inside the circle
bool isPointInsideCircle(const Point& p, const Circle& c)
{
    // calculating by the circle equation.
    float circleEqval = pow((p.x - c.center.x), 2) + pow((p.y - c.center.y), 2);
    // if its inside the circle.
    if ((circleEqval - pow(c.radius, 2)) <= EPSILON)
    {
        return true;
    }
    return false;
}

// checking if all of the points is inside the circle
bool isCircle(const Circle& c, const vector<Point>& points, int pointsSize)
{
    for (int i = 0; i < pointsSize; i++)
    {
        if (!isPointInsideCircle(points.at(i), c))
        {
            return false;
        }
    }
    return true;
}

// getting a circle from 2 points on its boundry
Circle getCircleFrom2Points(const Point& p1, const Point& p2)
{
    float radius = getRadius(p1, p2);
    // middle point equation. the 2 points must be the diameter of the circle.
    float centerXval = (p1.x + p2.x) / 2;
    float centerYval = (p1.y + p2.y) / 2;
    Point center(centerXval, centerYval);
    return Circle(center, radius);
}

// calculating a circle with 2 or 3 points on boundry
Circle circleCalc(vector<Point>& circleBoundry, int boundrySize)
{
    // initializing a starting circle
    if (boundrySize == 0)
    {
        return Circle(Point(0, 0), 0);
    }
    // reurning only the center with radius=0
    if (boundrySize == 1)
    {
        return Circle(circleBoundry.at(0), 0);
    }
    // in case there are 2 points on he boundry
    if (boundrySize == 2)
    {
        return getCircleFrom2Points(circleBoundry.at(0), circleBoundry.at(1));
    }
    //if (boundrySize == 3), so first we will check if there is a possibility that we can create the circle with only 2 points
    for (int i = 0; i < 3; i++)
    {
        for (int j = i + 1; j < 3; j++)
        {
            // we will check for every set of points on the boundry if its possible
            Circle c = getCircleFrom2Points(circleBoundry.at(i), circleBoundry.at(j));
            if (isCircle(c, circleBoundry, boundrySize))
            {
                // we need at least 1 couple of points that are fullfiling it
                return c;
            }
        }
    }
    // else we will return the circle from 3 points on its boundry
    Point center = getCenterFrom3Points(circleBoundry);
    float radius = getRadiusFrom3Points(center, circleBoundry.at(0));
    return Circle(center, radius);
}

// using welzl algorithm for finding the minimal enclosing circle
Circle welzlAlg(vector<Point>& points, size_t pointsSize, vector<Point> circleBoundry, int boundrySize)
{
    // if there are 3 points on the boundry, we can calculate the circle
    // if there are no more points, so we have 2 points on the boundry
    if (boundrySize == 3 || pointsSize == 0)
    {
        return circleCalc(circleBoundry, boundrySize);
    }
    //extracting the last random point from the points vector
    Point randomPoint = points.at(pointsSize - 1);
    // calculating the circle with the new points (without the last point), and the same boundry
    Circle circ = welzlAlg(points, pointsSize - 1, circleBoundry, boundrySize);
    if (!isPointInsideCircle(randomPoint, circ))
    {
        // so the point must be on the boundry
        circleBoundry.push_back(randomPoint);
        // calculating the circle with the new points (without the last point), and the new boundry
        circ = welzlAlg(points, pointsSize - 1, circleBoundry, boundrySize + 1);

    }
    return circ;
}

//finding the minimal enclosing circle
Circle findMinCircle(Point** points, size_t size)
{
    // if there is only 1 point - this is no a real circle
    if (size == 1)
    {
        return Circle(*points[0], 0);
    }
    // a vecor containig the points on the boundry
    vector<Point> circleBoundry;
    int circleBoundrySize = 0;
    // switch the representation of points from array to vector
    vector<Point> pointsVector;
    for (int i = 0; i < size; i++)
    {
        pointsVector.push_back(*points[i]);
    }
    return welzlAlg(pointsVector, size, circleBoundry, circleBoundrySize);
}
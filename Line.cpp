//
// Created by Chlebus, Grzegorz on 25.08.17.
// Copyright (c) Chlebus, Grzegorz. All rights reserved.
//

#include "Line.h"
#include "geometry.h"

using namespace cv;

Line Line::fromRhoTheta(float rho, float theta)
{
  double a = cos(theta), b = sin(theta);
  double x0 = a * rho, y0 = b * rho;
  Point2f p1((x0 + 2000 * (-b)),
    (y0 + 2000 * (a)));
  Point2f p2((x0 - 2000 * (-b)),
    (y0 - 2000 * (a)));
  return Line::fromTwoPoints(p1, p2);
}


Line Line::fromTwoPoints(cv::Point2f p1, cv::Point2f p2)
{
  Point2f vec = p2 - p1;
  return Line(p1, vec);
}

Line::Line(cv::Point2f point, cv::Point2f vector)
  : u(point)
  , v(normalize(vector))
{

}

cv::Point2f Line::getPoint() const
{
  return u;
}


cv::Point2f Line::getVector() const
{
  return v;
}

float Line::getDistance(const cv::Point2f& point) const
{
  Point2f pointOnLine = getPointOnLineClosestTo(point);
  return distance(point, pointOnLine);
}

cv::Point2f Line::getPointOnLineClosestTo(const cv::Point2f point) const
{
  Point2f n;
  float c;
  toImplicit(n, c);
  float q = c - n.dot(point);
  return point - q*n;
}

bool Line::isDuplicate(const Line& otherLine) const
{
  Point2f n1, n2;
  float c1, c2;
  toImplicit(n1, c1);
  otherLine.toImplicit(n2, c2);
  float dot = fabs(n1.dot(n2));
  double dotThreshold = cos(1*CV_PI/180);
  if ((dot > dotThreshold) && fabs(fabs(c1)-fabs(c2)) < 10)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void Line::toImplicit(cv::Point2f& n, float& c) const
{
  n = perpendicular(v);
  c = n.dot(u);
}


bool Line::isVertical() const
{
  Point2f n;
  float c;
  toImplicit(n, c);

  return (fabs(atan2(n.y, n.x)) < 65 * CV_PI / 180) || (fabs(atan2(-n.y, -n.x)) < 65 * CV_PI / 180);
}

bool Line::computeIntersectionPoint(const Line& line, cv::Point2f& intersectionPoint) const
{
  Point2f x = line.getPoint() - u;
  Point2f d1 = v;
  Point2f d2 = line.getVector();
  float cross = d1.x * d2.y - d1.y * d2.x;
  if (abs(cross) < /*EPS*/1e-8)
    return false;
  double t1 = (x.x * d2.y - x.y * d2.x) / cross;
  intersectionPoint = u + d1 * t1;
  return true;
}

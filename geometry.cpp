//
// Created by Chlebus, Grzegorz on 26.08.17.
// Copyright (c) Chlebus, Grzegorz. All rights reserved.
//
#include "geometry.h"

float length(const cv::Point2f& v)
{
  return sqrt(v.x*v.x + v.y*v.y);
}

float distance(const cv::Point2f& p1, const cv::Point2f& p2)
{
  float dx = p1.x - p2.x;
  float dy = p1.y - p2.y;
  return sqrt(dx*dx + dy*dy);
}


cv::Point2f perpendicular(const cv::Point2f& v)
{
  return cv::Point2f(-v.y, v.x);
}

cv::Point2f normalize(const cv::Point2f& v)
{
  float l = length(v);
  return cv::Point2f(v.x/l, v.y/l);
}

class LineComparator
{
public:
  LineComparator(cv::Point2f point) : p(point) { }

  bool operator()(const Line& lineA, const Line& lineB)
  {
    return lineA.getDistance(p) < lineB.getDistance(p);
  }

private:
  cv::Point2f p;
};

void sortLinesByDistanceToPoint(std::vector<Line>& lines, const cv::Point2f& point)
{
  std::sort(lines.begin(), lines.end(), LineComparator(point));
}
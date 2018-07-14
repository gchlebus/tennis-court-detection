//
// Created by Chlebus, Grzegorz on 26.08.17.
// Copyright (c) Chlebus, Grzegorz. All rights reserved.
//
#include "DebugHelpers.h"

using namespace std;
using namespace cv;

void printVideoInfo(const cv::VideoCapture vc)
{
  int frameCount = int(vc.get(CV_CAP_PROP_FRAME_COUNT));
  int width = int(vc.get(CV_CAP_PROP_FRAME_WIDTH));
  int height = int(vc.get(CV_CAP_PROP_FRAME_HEIGHT));
  int fps = int(vc.get(CV_CAP_PROP_FPS));
  std::cout << "Video properties (frames: " << frameCount << ", w: " << width << ", h: " << height
    << ", fps: " << fps << ")" << std::endl;
}

void displayImage(const string& windowName, const Mat& image, int delay)
{
  namedWindow(windowName, CV_WINDOW_AUTOSIZE);
  imshow(windowName, image);
  waitKey(delay);
}


void drawLines(std::vector<Line>& lines, cv::Mat& image, cv::Scalar color, int thickness)
{
  for (auto& line: lines)
  {
    drawLine(line, image, color, thickness);
  }
}

void drawLine(const Line& line, cv::Mat& image, Scalar color, int thickness)
{
  Line topBorder(Point2f(0, 0), Point2f(1, 0));
  Line leftBorder(Point2f(0, 0), Point2f(0, 1));
  Line bottomBorder(Point2f(image.cols, image.rows), Point2f(1, 0));
  Line rightBorder(Point2f(image.cols, image.rows), Point2f(0, 1));

  Point2f p1, p2;
  if (line.computeIntersectionPoint(topBorder, p1) && line.computeIntersectionPoint(bottomBorder, p2))
  {
    cv::line(image, p1, p2, color, thickness, 8);
  }
  else if (line.computeIntersectionPoint(leftBorder, p1) && line.computeIntersectionPoint(rightBorder, p2))
  {
    cv::line(image, p1, p2, color, thickness, 8);
  }
  else
  {
    throw std::runtime_error("No intersections found!");
  }
}


void drawLine(const cv::Point2f start, const cv::Point2f end, cv::Mat& image, Scalar color, int thickness)
{
  cv::line(image, start, end, color, thickness, 8);
}

void drawPoints(std::vector<cv::Point2f> points, cv::Mat& image, Scalar color)
{
  for (auto& point: points)
  {
    drawPoint(point, image, color);
  }
}


void drawPoint(cv::Point2f point, cv::Mat& image, Scalar color)
{
  circle(image, point, 3, color, -1);
}

void printInfo(const cv::Mat& matrix, const std::string& name)
{
  std::cout << name <<
    " rows " << matrix.rows <<
    ", cols " << matrix.cols <<
    ", type " << matrix.type() << std::endl;
}

void printInfo(const cv::Point2f& point, const std::string& name)
{
  std::cout << name << "(" << point.x << ", " << point.y << ")" << std::endl;
}

void printInfo(const Line& line, const std::string& name)
{
  std::cout << name << ": " << std::endl;
  printInfo(line.getPoint(), "Point");
  printInfo(line.getVector(), "Vector");
}

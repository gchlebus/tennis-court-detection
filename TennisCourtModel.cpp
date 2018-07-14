//
// Created by Chlebus, Grzegorz on 28.08.17.
// Copyright (c) Chlebus, Grzegorz. All rights reserved.
//

#include "TennisCourtModel.h"
#include "GlobalParameters.h"
#include "DebugHelpers.h"
#include "geometry.h"
#include "TimeMeasurement.h"

using namespace cv;

TennisCourtModel::TennisCourtModel()
{
  Point2f hVector(1, 0);
  const Line upperBaseLine = Line(Point2f(0, 0), hVector);
  const Line upperServiceLine = Line(Point2f(0, 5.49), hVector);
  const Line netLine = Line(Point2f(0, 11.89), hVector);
  const Line lowerServiceLine = Line(Point2f(0, 18.29), hVector);
  const Line lowerBaseLine = Line(Point2f(0, 23.78), hVector);
  hLines = {
    upperBaseLine, upperServiceLine, netLine, lowerServiceLine, lowerBaseLine
  };

  Point2f vVector(0, 1);
  const Line leftSideLine = Line(Point2f(0, 0), vVector);
  const Line leftSinglesLine = Line(Point2f(1.37, 0), vVector);
  const Line centreServiceLine = Line(Point2f(5.485, 0), vVector);
  const Line rightSinglesLine = Line(Point2f(9.6, 0), vVector);
  const Line rightSideLine = Line(Point2f(10.97, 0), vVector);
  vLines = {
    leftSideLine, leftSinglesLine, centreServiceLine, rightSinglesLine, rightSideLine
  };

  // TODO do not contrain to only these lines
//  hLinePairs = getPossibleLinePairs(hLines);
//  vLinePairs = getPossibleLinePairs(vLines);
  hLinePairs.push_back(std::make_pair(hLines[0], hLines[4]));
  hLinePairs.push_back(std::make_pair(hLines[0], hLines[3]));
  hLinePairs.push_back(std::make_pair(hLines[1], hLines[3]));
  hLinePairs.push_back(std::make_pair(hLines[1], hLines[4]));


  vLinePairs.push_back(std::make_pair(vLines[0], vLines[4]));
  vLinePairs.push_back(std::make_pair(vLines[0], vLines[3]));
  vLinePairs.push_back(std::make_pair(vLines[1], vLines[4]));
  vLinePairs.push_back(std::make_pair(vLines[1], vLines[3]));

  Point2f point;
  if (upperBaseLine.computeIntersectionPoint(leftSideLine, point))
  {
    courtPoints.push_back(point); // P1
  }
  if (lowerBaseLine.computeIntersectionPoint(leftSideLine, point))
  {
    courtPoints.push_back(point); // P2
  }
  if (lowerBaseLine.computeIntersectionPoint(rightSideLine, point))
  {
    courtPoints.push_back(point); // P3
  }
  if (upperBaseLine.computeIntersectionPoint(rightSideLine, point))
  {
    courtPoints.push_back(point);  // P4
  }
  if (upperBaseLine.computeIntersectionPoint(leftSinglesLine, point))
  {
    courtPoints.push_back(point);  // P5
  }
  if (lowerBaseLine.computeIntersectionPoint(leftSinglesLine, point))
  {
    courtPoints.push_back(point);  // P6
  }
  if (lowerBaseLine.computeIntersectionPoint(rightSinglesLine, point))
  {
    courtPoints.push_back(point);  // P7
  }
  if (upperBaseLine.computeIntersectionPoint(rightSinglesLine, point))
  {
    courtPoints.push_back(point);  // P8
  }
  if (leftSinglesLine.computeIntersectionPoint(upperServiceLine, point))
  {
    courtPoints.push_back(point);  // P9
  }
  if (rightSinglesLine.computeIntersectionPoint(upperServiceLine, point))
  {
    courtPoints.push_back(point);  // P10
  }
  if (leftSinglesLine.computeIntersectionPoint(lowerServiceLine, point))
  {
    courtPoints.push_back(point);  // P11
  }
  if (rightSinglesLine.computeIntersectionPoint(lowerServiceLine, point))
  {
    courtPoints.push_back(point);  // P12
  }
  if (upperServiceLine.computeIntersectionPoint(centreServiceLine, point))
  {
    courtPoints.push_back(point);  // P13
  }
  if (lowerServiceLine.computeIntersectionPoint(centreServiceLine, point))
  {
    courtPoints.push_back(point);  // P14
  }
  if (leftSideLine.computeIntersectionPoint(netLine, point))
  {
    courtPoints.push_back(point);  // P15
  }
  if (rightSideLine.computeIntersectionPoint(netLine, point))
  {
    courtPoints.push_back(point);  // P16
  }

  assert(courtPoints.size() == 16);
}

TennisCourtModel::TennisCourtModel(const TennisCourtModel& o)
  : transformationMatrix(o.transformationMatrix)
{
  courtPoints = o.courtPoints;
  hLinePairs = o.hLinePairs;
  vLinePairs = o.vLinePairs;
  hLines = o.hLines;
  vLines = o.vLines;
}

TennisCourtModel& TennisCourtModel::operator=(const TennisCourtModel& o)
{
  transformationMatrix = o.transformationMatrix;
  return *this;
}

float TennisCourtModel::fit(const LinePair& hLinePair, const LinePair& vLinePair,
  const cv::Mat& binaryImage, const cv::Mat& rgbImage)
{
  float bestScore = GlobalParameters().initialFitScore;
  std::vector<Point2f> points = getIntersectionPoints(hLinePair, vLinePair);
  //TODO Check whether the intersection points make sense

  for (auto& modelHLinePair: hLinePairs)
  {
    for (auto& modelVLinePair: vLinePairs)
    {
      std::vector<Point2f> modelPoints = getIntersectionPoints(modelHLinePair, modelVLinePair);
      Mat matrix = getPerspectiveTransform(modelPoints, points);
      std::vector<Point2f> transformedModelPoints(16);
      perspectiveTransform(courtPoints, transformedModelPoints, matrix);
      float score = evaluateModel(transformedModelPoints, binaryImage);
      if (score > bestScore)
      {
        bestScore = score;
        transformationMatrix = matrix;
      }
//      Mat image = rgbImage.clone();
//      drawModel(transformedModelPoints, image);
//      displayImage("TennisCourtModel", image, 0);
    }
  }
  return bestScore;
}


std::vector<cv::Point2f> TennisCourtModel::getIntersectionPoints(const LinePair& hLinePair,
  const LinePair& vLinePair)
{
  std::vector<Point2f> v;
  Point2f point;

  if (hLinePair.first.computeIntersectionPoint(vLinePair.first, point))
  {
    v.push_back(point);
  }
  if (hLinePair.first.computeIntersectionPoint(vLinePair.second, point))
  {
    v.push_back(point);
  }
  if (hLinePair.second.computeIntersectionPoint(vLinePair.first, point))
  {
    v.push_back(point);
  }
  if (hLinePair.second.computeIntersectionPoint(vLinePair.second, point))
  {
    v.push_back(point);
  }

  assert(v.size() == 4);

  return v;
}

std::vector<LinePair> TennisCourtModel::getPossibleLinePairs(std::vector<Line>& lines)
{
  std::vector<LinePair> linePairs;
  for (size_t first = 0; first < lines.size(); ++first)
//  for (size_t first = 0; first < 1; ++first)
  {
    for (size_t second = first + 1; second < lines.size(); ++second)
    {
      linePairs.push_back(std::make_pair(lines[first], lines[second]));
    }
  }
  return linePairs;
}


void TennisCourtModel::drawModel(cv::Mat& image, Scalar color)
{
  std::vector<Point2f> transformedModelPoints(16);
  perspectiveTransform(courtPoints, transformedModelPoints, transformationMatrix);
  drawModel(transformedModelPoints, image, color);
}

void TennisCourtModel::drawModel(std::vector<Point2f>& courtPoints, Mat& image, Scalar color)
{
  drawLine(courtPoints[0], courtPoints[1], image, color);
  drawLine(courtPoints[1], courtPoints[2], image, color);
  drawLine(courtPoints[2], courtPoints[3], image, color);
  drawLine(courtPoints[3], courtPoints[0], image, color);

  drawLine(courtPoints[4], courtPoints[5], image, color);
  drawLine(courtPoints[6], courtPoints[7], image, color);

  drawLine(courtPoints[8], courtPoints[9], image, color);
  drawLine(courtPoints[10], courtPoints[11], image, color);

  drawLine(courtPoints[12], courtPoints[13], image, color);
  drawLine(courtPoints[14], courtPoints[15], image, color);
}


float TennisCourtModel::evaluateModel(const std::vector<cv::Point2f>& courtPoints, const cv::Mat& binaryImage)
{
  float score = 0;

  // TODO: heuritic to see whether the model makes sense
  float d1 = distance(courtPoints[0], courtPoints[1]);
  float d2 = distance(courtPoints[1], courtPoints[2]);
  float d3 = distance(courtPoints[2], courtPoints[3]);
  float d4 = distance(courtPoints[3], courtPoints[0]);
  float t = 30;
  if (d1 < t || d2 < t || d3 < t || d4 < t)
  {
    return GlobalParameters().initialFitScore;
  }

  score += computeScoreForLineSegment(courtPoints[0], courtPoints[1], binaryImage);
  score += computeScoreForLineSegment(courtPoints[1], courtPoints[2], binaryImage);
  score += computeScoreForLineSegment(courtPoints[2], courtPoints[3], binaryImage);
  score += computeScoreForLineSegment(courtPoints[3], courtPoints[0], binaryImage);
  score += computeScoreForLineSegment(courtPoints[4], courtPoints[5], binaryImage);
  score += computeScoreForLineSegment(courtPoints[6], courtPoints[7], binaryImage);
  score += computeScoreForLineSegment(courtPoints[8], courtPoints[9], binaryImage);
  score += computeScoreForLineSegment(courtPoints[10], courtPoints[11], binaryImage);
  score += computeScoreForLineSegment(courtPoints[12], courtPoints[13], binaryImage);
//  score += computeScoreForLineSegment(courtPoints[14], courtPoints[14], binaryImage);

//  std::cout << "Score = " << score << std::endl;

  return score;
}

float TennisCourtModel::computeScoreForLineSegment(const cv::Point2f& start, const cv::Point2f& end,
  const cv::Mat& binaryImage)
{
  float score = 0;
  float fgScore = 1;
  float bgScore = -0.5;
  int length = round(distance(start, end));

  Point2f vec = normalize(end-start);

  for (int i = 0; i < length; ++i)
  {
    Point2f p = start + i*vec;
    int x = round(p.x);
    int y = round(p.y);
    if (isInsideTheImage(x, y, binaryImage))
    {
      uchar imageValue = binaryImage.at<uchar>(y,x);
      if (imageValue == GlobalParameters().fgValue)
      {
        score += fgScore;
      }
      else
      {
        score += bgScore;
      }
    }
  }
  return score;
}


bool TennisCourtModel::isInsideTheImage(float x, float y, const cv::Mat& image)
{
  return (x >= 0 && x < image.cols) && (y >= 0 && y < image.rows);
}

void TennisCourtModel::writeToFile(const std::string& filename)
{
  std::vector<Point2f> transformedModelPoints(16);
  perspectiveTransform(courtPoints, transformedModelPoints, transformationMatrix);

  std::ofstream outFile(filename);
  if (!outFile.is_open())
  {
    throw std::runtime_error("Unable to open file: " + filename);
  }
  for (auto& point: transformedModelPoints)
  {
    outFile << point.x << ";" << point.y << std::endl;
  }
}

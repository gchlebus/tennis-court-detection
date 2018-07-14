//
// Created by Chlebus, Grzegorz on 27.08.17.
// Copyright (c) Chlebus, Grzegorz. All rights reserved.
//

#include "CourtLineCandidateDetector.h"
#include "GlobalParameters.h"
#include "TimeMeasurement.h"
#include "DebugHelpers.h"

using namespace cv;

bool CourtLineCandidateDetector::debug = false;
const std::string CourtLineCandidateDetector::windowName = "CourtLineCandidateDetector";

CourtLineCandidateDetector::Parameters::Parameters()
{
  houghThreshold = 120;
  distanceThreshold = 8;
  refinementIterations = 5;
}

CourtLineCandidateDetector::CourtLineCandidateDetector()
  : CourtLineCandidateDetector(Parameters())
{

}


CourtLineCandidateDetector::CourtLineCandidateDetector(CourtLineCandidateDetector::Parameters p)
  : parameters(p)
{

}


std::vector<Line> CourtLineCandidateDetector::run(const cv::Mat& binaryImage, const cv::Mat& rgbImage)
{
  TimeMeasurement::start("CourtLineCandidateDetector::run");

  std::vector<Line> lines;
  TimeMeasurement::start("\textractLines");
  lines = extractLines(binaryImage, rgbImage);
  TimeMeasurement::start("\textractLines");

  for (int i = 0; i < parameters.refinementIterations; ++i)
  {
    TimeMeasurement::start("\tgetRefinedParameters");
    refineLineParameters(lines, binaryImage, rgbImage);
    TimeMeasurement::stop("\tgetRefinedParameters");

    TimeMeasurement::start("\tremoveDuplicateLines");
    removeDuplicateLines(lines, rgbImage);
    TimeMeasurement::stop("\tremoveDuplicateLines");
  }

  TimeMeasurement::stop("CourtLineCandidateDetector::run");
  return lines;
}

std::vector<Line> CourtLineCandidateDetector::extractLines(const cv::Mat& binaryImage,
  const cv::Mat& rgbImage)
{
  std::vector<cv::Point2f> tmpLines;

  HoughLines(binaryImage, tmpLines, 1, CV_PI / 180, parameters.houghThreshold);

  std::vector<Line> lines;
  for (size_t i = 0; i < tmpLines.size(); ++i)
  {
    lines.push_back(Line::fromRhoTheta(tmpLines[i].x, tmpLines[i].y));
  }

  if (debug)
  {
    std::cout << "CourtLineCandidateDetector::extractLines line count = " << lines.size() << std::endl;
    Mat image = rgbImage.clone();
    drawLines(lines, image);
    displayImage(windowName, image);
  }

  return lines;
}


void CourtLineCandidateDetector::refineLineParameters(std::vector<Line>& lines,
  const Mat& binaryImage, const Mat& rgbImage)
{
  for (auto& line: lines)
  {
    line = getRefinedParameters(line, binaryImage, rgbImage);
  }
  if (debug)
  {
    Mat image = rgbImage.clone();
    drawLines(lines, image);
    displayImage(windowName, image);
  }
}

bool lineEqual(const Line& a, const Line& b)
{
  return a.isDuplicate(b);
}


bool CourtLineCandidateDetector::operator()(const Line& a, const Line& b)
{
  Mat tmpImage = image.clone();
  drawLine(a, tmpImage);
  drawLine(b, tmpImage);
  displayImage(windowName, tmpImage, 1);
  return a.isDuplicate(b);
}

void CourtLineCandidateDetector::removeDuplicateLines(std::vector<Line>& lines, const cv::Mat& rgbImage)
{
  image = rgbImage.clone();
  auto it = std::unique(lines.begin(), lines.end(), lineEqual);
  lines.erase(it, lines.end());
  if (debug)
  {
    std::cout << "CourtLineCandidateDetector::removeDuplicateLines line count =  " << lines.size() << std::endl;
    Mat image = rgbImage.clone();
    drawLines(lines, image);
    displayImage(windowName, image);
  }
}

Line CourtLineCandidateDetector::getRefinedParameters(Line line, const Mat& binaryImage,
  const cv::Mat& rgbImage)
{
  Mat A = getClosePointsMatrix(line, binaryImage, rgbImage);
  Mat X = Mat::zeros(1, 4, CV_32F);
  fitLine(A, X, DIST_L2, 0, 0.01, 0.01);
  Point2f v(X.at<float>(0,0), X.at<float>(0,1));
  Point2f p(X.at<float>(0,2), X.at<float>(0,3));
  return Line(p, v);
}

Mat CourtLineCandidateDetector::getClosePointsMatrix(Line line, const Mat& binaryImage,
  const cv::Mat& rgbImage)
{
  Mat M = Mat::zeros(0, 2, CV_32F);

  Mat image = rgbImage.clone(); // debug

  for (unsigned int x = 0; x < binaryImage.cols; ++x)
  {
    for (unsigned int y = 0; y < binaryImage.rows; ++y)
    {
      if (binaryImage.at<uchar>(y, x) == GlobalParameters().fgValue)
      {
        float distance = line.getDistance(Point2f(x, y));
        if (distance < parameters.distanceThreshold)
        {
//          drawPoint(Point2f(x, y), image, Scalar(255,0,0));
          Mat point = Mat::zeros(1, 2, CV_32F);
          point.at<float>(0, 0) = x;
          point.at<float>(0, 1) = y;
          M.push_back(point);
        }
      }
    }
  }

  if (false)
  {
    drawLine(line, image);
    displayImage(windowName, image);
  }

  return M;
}

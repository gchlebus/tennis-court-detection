//
// Created by Chlebus, Grzegorz on 27.08.17.
// Copyright (c) Chlebus, Grzegorz. All rights reserved.
//
#pragma once

#include "Line.h"
#include <opencv2/opencv.hpp>

class CourtLineCandidateDetector
{
public:
  struct Parameters
  {
    Parameters();
    int houghThreshold;
    int distanceThreshold; // in pixels
    int refinementIterations;
  };

  CourtLineCandidateDetector();

  CourtLineCandidateDetector(Parameters p);

  bool operator()(const Line& a, const Line& b);

  std::vector<Line> run(const cv::Mat& binaryImage, const cv::Mat& rgbImage);

  static bool debug;
  static const std::string windowName;

private:
  std::vector<Line> extractLines(const cv::Mat& binaryImage, const cv::Mat& rgbImage);

  void refineLineParameters(std::vector<Line>& lines, const cv::Mat& binaryImage,
    const cv::Mat& rgbImage);

  void removeDuplicateLines(std::vector<Line>& lines, const cv::Mat& rgbImage);

  Line getRefinedParameters(Line line, const cv::Mat& binaryImage, const cv::Mat& rgbImage);

  cv::Mat getClosePointsMatrix(Line line, const cv::Mat& binaryImage, const cv::Mat& rgbImage);

  Parameters parameters;

  cv::Mat image;
};


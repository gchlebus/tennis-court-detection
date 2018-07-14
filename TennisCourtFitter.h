//
// Created by Chlebus, Grzegorz on 28.08.17.
// Copyright (c) Chlebus, Grzegorz. All rights reserved.
//
#pragma once

#include "Line.h"
#include <opencv2/opencv.hpp>
#include "TennisCourtModel.h"

class TennisCourtFitter
{
public:
  struct Parameters
  {
    Parameters();
  };

  TennisCourtFitter();

  TennisCourtFitter(Parameters p);

  TennisCourtModel run(const std::vector<Line>& lines, const cv::Mat& binaryImage, const cv::Mat& rgbImage);

  static bool debug;
  static const std::string windowName;

private:
  void getHorizontalAndVerticalLines(const std::vector<Line>& lines, std::vector<Line>& hLines,
    std::vector<Line>& vLines, const cv::Mat& rgbImage);

  void sortHorizontalLines(std::vector<Line>& hLines, const cv::Mat& rgbImage);

  void sortVerticalLines(std::vector<Line>& vLines, const cv::Mat& rgbImage);

  void findBestModelFit(const cv::Mat& binaryImage, const cv::Mat& rgbImage);

  Parameters parameters;
  std::vector<LinePair> hLinePairs;
  std::vector<LinePair> vLinePairs;
  TennisCourtModel bestModel;
};
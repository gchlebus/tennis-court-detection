//
// Created by Chlebus, Grzegorz on 26.08.17.
// Copyright (c) Chlebus, Grzegorz. All rights reserved.
//

#pragma once

#include <opencv2/opencv.hpp>

class CourtLinePixelDetector
{
public:
  struct Parameters
  {
    Parameters();
    uchar threshold;
    uchar diffThreshold;
    unsigned int t;
    int gradientKernelSize;
    int kernelSize;
  };

  CourtLinePixelDetector();

  CourtLinePixelDetector(Parameters p);

  cv::Mat run(const cv::Mat& frame);

  static bool debug;
  static const std::string windowName;

private:
  cv::Mat getLuminanceChannel(const cv::Mat& frame);

  cv::Mat detectLinePixels(const cv::Mat& image);

  cv::Mat filterLinePixels(const cv::Mat& binaryImage, const cv::Mat& luminanceImage);

  uchar isLinePixel(const cv::Mat& image, unsigned int x, unsigned int y);

  void computeStructureTensorElements(const cv::Mat& image, cv::Mat& dx2, cv::Mat& dxy, cv::Mat& dy2);

  Parameters parameters;
};


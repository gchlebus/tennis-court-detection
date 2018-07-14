//
// Created by Chlebus, Grzegorz on 26.08.17.
// Copyright (c) Chlebus, Grzegorz. All rights reserved.
//

#include "CourtLinePixelDetector.h"
#include "GlobalParameters.h"
#include "DebugHelpers.h"
#include "TimeMeasurement.h"

using namespace cv;

bool CourtLinePixelDetector::debug = false;
const std::string CourtLinePixelDetector::windowName = "CourtLinePixelDetector";

CourtLinePixelDetector::Parameters::Parameters()
{
  threshold = 80;
  diffThreshold = 20;
  t = 4;
  gradientKernelSize = 3;
  kernelSize = 41;
}

CourtLinePixelDetector::CourtLinePixelDetector()
  : CourtLinePixelDetector(Parameters())
{

}

CourtLinePixelDetector::CourtLinePixelDetector(CourtLinePixelDetector::Parameters p)
  : parameters(p)
{

}

Mat CourtLinePixelDetector::run(const Mat& frame)
{
  TimeMeasurement::start("CourtLinePixelDetector::run");

  TimeMeasurement::start("\tgetLuminanceChannel");
  Mat luminanceImage = getLuminanceChannel(frame);
  TimeMeasurement::stop("\tgetLuminanceChannel");

  TimeMeasurement::start("\tdetectLinePixels");
  Mat image = detectLinePixels(luminanceImage);
  TimeMeasurement::stop("\tdetectLinePixels");

  TimeMeasurement::start("\tfilterLinePixels");
  image = filterLinePixels(image, luminanceImage);
  TimeMeasurement::stop("\tfilterLinePixels");

  TimeMeasurement::stop("CourtLinePixelDetector::run");
  return image;
}

cv::Mat CourtLinePixelDetector::getLuminanceChannel(const cv::Mat& frame)
{
  Mat imgYCbCr;
  cvtColor(frame, imgYCbCr, CV_RGB2YCrCb);
  Mat luminanceChannel(frame.rows, frame.cols, CV_8UC1);
  const int from_to[2] = {0, 0};
  mixChannels(&frame, 1, &luminanceChannel, 1, from_to, 1);

  if (debug)
  {
    displayImage(windowName, frame);
    displayImage(windowName, imgYCbCr);
    displayImage(windowName, luminanceChannel);
  }

  return luminanceChannel;
}

Mat CourtLinePixelDetector::detectLinePixels(const cv::Mat& image)
{
  Mat pixelImage(image.rows, image.cols, CV_8UC1, Scalar(GlobalParameters().bgValue));

  for (unsigned int x = 0; x < image.cols; ++x)
  {
    for (unsigned int y = 0; y < image.rows; ++y)
    {
      pixelImage.at<uchar>(y,x) = isLinePixel(image, x, y);
    }
  }

  if (debug)
  {
    displayImage(windowName, pixelImage);
  }

  return pixelImage;
}


uchar CourtLinePixelDetector::isLinePixel(const Mat& image, unsigned int x, unsigned int y)
{
  if (x < parameters.t || image.cols - x <= parameters.t)
  {
    return GlobalParameters().bgValue;
  }
  if (y < parameters.t || image.rows - y <= parameters.t)
  {
    return GlobalParameters().bgValue;
  }

  uchar value = image.at<uchar>(y,x);
  if (value < parameters.threshold)
  {
    return GlobalParameters().bgValue;
  }

  uchar topValue = image.at<uchar>(y-parameters.t, x);
  uchar lowerValue = image.at<uchar>(y+parameters.t, x);
  uchar leftValue = image.at<uchar>(y,x-parameters.t);
  uchar rightValue = image.at<uchar>(y,x+parameters.t);

  if ((value - leftValue > parameters.diffThreshold) && (value - rightValue > parameters.diffThreshold))
  {
    return GlobalParameters().fgValue;
  }

  if ((value - topValue > parameters.diffThreshold) && (value - lowerValue > parameters.diffThreshold))
  {
    return GlobalParameters().fgValue;
  }

  return GlobalParameters().bgValue;
}


Mat CourtLinePixelDetector::filterLinePixels(const Mat& binaryImage, const Mat& luminanceImage)
{
  Mat dx2, dxy, dy2;
  computeStructureTensorElements(luminanceImage, dx2, dxy, dy2);
  Mat outputImage(binaryImage.rows, binaryImage.cols, CV_8UC1, Scalar(GlobalParameters().bgValue));
  for (unsigned int x = 0; x < binaryImage.cols; ++x)
  {
    for (unsigned int y = 0; y < binaryImage.rows; ++y)
    {
      uchar value = binaryImage.at<uchar>(y,x);
      if (value == GlobalParameters().fgValue)
      {
        Mat t(2, 2, CV_32F);
        t.at<float>(0, 0) = dx2.at<float>(y,x);
        t.at<float>(0, 1) = dxy.at<float>(y,x);
        t.at<float>(1, 0) = dxy.at<float>(y,x);
        t.at<float>(1, 1) = dy2.at<float>(y,x);
        Mat l;
        eigen(t, l);
        if (l.at<float>(0,0) > 4* l.at<float>(0,1))
        {
          outputImage.at<uchar>(y,x) = GlobalParameters().fgValue;
        }
      }
    }
  }

  if (debug)
  {
    displayImage(windowName, outputImage);
  }

  return outputImage;
}


void CourtLinePixelDetector::computeStructureTensorElements(const cv::Mat& image, cv::Mat& dx2, cv::Mat& dxy, cv::Mat& dy2)
{
  Mat floatImage, dx, dy;
  image.convertTo(floatImage, CV_32F);
  GaussianBlur(floatImage, floatImage, Size(5,5), 0);
  Sobel(floatImage, dx, CV_32F, 1, 0, parameters.gradientKernelSize);
  Sobel(floatImage, dy, CV_32F, 0, 1, parameters.gradientKernelSize);
  multiply(dx, dx, dx2);
  multiply(dx, dy, dxy);
  multiply(dy, dy, dy2);
  Mat kernel = Mat::ones(parameters.kernelSize, parameters.kernelSize, CV_32F);
  filter2D(dx2, dx2, -1, kernel);
  filter2D(dxy, dxy, -1, kernel);
  filter2D(dy2, dy2, -1, kernel);
}

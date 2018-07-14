//
// Created by Chlebus, Grzegorz on 26.08.17.
// Copyright (c) Chlebus, Grzegorz. All rights reserved.
//
#pragma once

#include <opencv2/opencv.hpp>
#include "Line.h"

void printVideoInfo(const cv::VideoCapture vc);

void displayImage(const std::string& windowName, const cv::Mat& image, int delay=0);

void drawLines(std::vector<Line>& lines, cv::Mat& image, cv::Scalar color=cv::Scalar(0,0,255), int thickness=2);

void drawLine(const Line& line, cv::Mat& image, cv::Scalar color=cv::Scalar(0,0,255), int thickness=2);

void drawLine(const cv::Point2f start, const cv::Point2f end,
  cv::Mat& image, cv::Scalar color=cv::Scalar(0,0,255), int thickness=2);

void drawPoints(std::vector<cv::Point2f> points, cv::Mat& image, cv::Scalar color=cv::Scalar(0,0,255));

void drawPoint(cv::Point2f point, cv::Mat& image, cv::Scalar color=cv::Scalar(0,0,255));

void printInfo(const cv::Mat& matrix, const std::string& name="Matrix");

void printInfo(const cv::Point2f& point, const std::string& name="Point");

void printInfo(const Line& line, const std::string& name="Line");


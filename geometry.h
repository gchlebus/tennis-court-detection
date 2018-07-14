//
// Created by Chlebus, Grzegorz on 26.08.17.
// Copyright (c) Chlebus, Grzegorz. All rights reserved.
//
#pragma once

#include "Line.h"
#include <opencv2/opencv.hpp>

float length(const cv::Point2f& v);

float distance(const cv::Point2f& p1, const cv::Point2f& p2);

cv::Point2f perpendicular(const cv::Point2f& v);

cv::Point2f normalize(const cv::Point2f& v);

void sortLinesByDistanceToPoint(std::vector<Line>& lines, const cv::Point2f& point);

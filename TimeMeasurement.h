//
// Created by Chlebus, Grzegorz on 25.08.17.
// Copyright (c) Chlebus, Grzegorz. All rights reserved.
//

#pragma once

#include <string>
#include <chrono>
#include <map>


class TimeMeasurement
{
public:
  typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;

  static bool enabled;

  static bool debug;

  static void start(const std::string& name);

  static float stop(const std::string& name);

private:
  static std::map<std::string, TimePoint> startPoints;
};


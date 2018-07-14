//
// Created by Chlebus, Grzegorz on 25.08.17.
// Copyright (c) Chlebus, Grzegorz. All rights reserved.
//

#include "TimeMeasurement.h"
#include <iostream>

using namespace std;


bool TimeMeasurement::enabled = false;

bool TimeMeasurement::debug = false;

map<string, TimeMeasurement::TimePoint> TimeMeasurement::startPoints = map<string, TimeMeasurement::TimePoint>();

void TimeMeasurement::start(const std::string& name)
{
  startPoints[name] = chrono::high_resolution_clock::now();
}


float TimeMeasurement::stop(const std::string& name)
{
  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double> elapsed_s = end - startPoints[name];
  if (debug)
  {
    cout << name << " " << 1000 * elapsed_s.count() << " ms" << endl;
  }
  return elapsed_s.count();
}

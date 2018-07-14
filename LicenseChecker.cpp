//
// Created by Chlebus, Grzegorz on 29.08.17.
// Copyright (c) Chlebus, Grzegorz. All rights reserved.
//

#include "LicenseChecker.h"
#include <sstream>
#include <iomanip>
#include <iostream>

LicenseChecker::LicenseChecker(const std::string& expiryDate,
  const std::string& expiryMessage, int returnCode)
{
  std::tm tm;
  std::istringstream ss(expiryDate);
  ss >> std::get_time(&tm, "%Y-%m-%d");
  std::time_t expiryTime = std::mktime(&tm);
  if (std::time(nullptr) > expiryTime)
  {
    std::cerr << expiryMessage << std::endl;
    exit(returnCode);
  }
}

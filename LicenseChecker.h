//
// Created by Chlebus, Grzegorz on 29.08.17.
// Copyright (c) Chlebus, Grzegorz. All rights reserved.
//
#pragma once

#include <iosfwd>
#include <ctime>

class LicenseChecker
{
public:
  LicenseChecker(const std::string& expiryDate, const std::string& expiryMessage,
    int returnCode);
};

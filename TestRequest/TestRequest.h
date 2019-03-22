#pragma once
/////////////////////////////////////////////////////////////////////
// TestRequest.h - Creates and parses Test Requests                //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  TestRequest class instances create and parse TestRequests.
*  Each TestRequest instance holds:
*  - name:    name of test request for display
*  - author:  author of test request
*  - date:    date test request was created
*  - one or more dll names
*
*  Required Files:
*  ---------------
*  TestRequest.h, TestRequest.cpp
*  Properties.h, Properties.cpp
*  DateTime.h, DateTime.cpp
*  StringUtilities.h, StringUtilities.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 23 Oct 2018
*  - first release
*/

#include <iostream>
#include <string>
#include <vector>
#include "../Utilities/Properties/Properties.h"
#include "../Utilities/DateTime/DateTime.h"
#include "../Utilities/StringUtilities/StringUtilities.h"

namespace Testing
{
  using Name = std::string;
  using Dll = std::string;
  using Request = std::vector<Dll>;
  using Author = std::string;
  using Date = std::string;
  using namespace Utilities;

  class TestRequest 
  {
  public:
    Property<Name> name;
    Property<Request> request;
    Property<Author> author;
    Property<Date> date;
    void addDll(const Dll& dll);
    std::string toString();
    static TestRequest fromString(const std::string& trStr);
  };
}
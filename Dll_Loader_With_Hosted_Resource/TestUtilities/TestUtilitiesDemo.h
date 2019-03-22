#pragma once
///////////////////////////////////////////////////////////////////////
// TestUtilitiesDemo.h - facilities for single-user test harness     //
// ver 3.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides three classes and a struct:
* - TestExecutor    Executes single test in the context of a try-catch block
* - TestExecutive   Uses TestExecutor to run a sequence of tests
* - TestHelpers     Provides methods to display file names and contents
* - Cosmetic        Displays newline when instance goes out of scope
*
* Required Files:
* ---------------
*   TestUtilities.h
*
* Maintenance History:
* --------------------
* ver 3.0 : 07 Oct 2018
* - removed logger code to simplify demo and avoid giving too much help
* ver 2.2 : 02 Oct 2018
* - added logger code
* - move Cosmetic struct to CodeUtilities.h
* ver 2.1 : 04 Sep 2018
* - added TestExecutive::clearTests()
* - minor changes to display layout
* ver 2.0 : 28 Aug 2018
* - major change from tests defined by lambdas
*   to tests defined by classes derived from ITest interface
* ver 1.0 : 12 Jan 2018
* - first release
* - taken from Utilities/TestUtilities package
*
* Notes:
* ------
* - Designed to provide all functionality in header file.
* - Implementation file only needed for test and demo.
*
*/

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <memory>
#include "../TestUtilities/FileSystem.h"
#include "../ITests.h"

namespace Utilities
{
  using Path = std::string;
  using Message = std::string;
  using Line = size_t;
  using File = std::string;
  using Files = std::vector<File>;

  /////////////////////////////////////////////////////////////////////
  // TestExecutor class
  // - supports execution of callable objects for testing in the
  //   context of a try-catch block.

  class TestExecutor
  {
  public:
    bool execute(std::shared_ptr<ITest> pTest, const std::string& name);
  private:
    void check(bool result);
  };
  //----< execute tests in the context of a try-catch block >----------

  bool TestExecutor::execute(std::shared_ptr<ITest> pTest, const std::string& name)
  {
    bool result = false;
    try
    {
      result = pTest->test();  // execute test which may throw an exception
      check(result);
      std::string msg = " -- \"" + name + "\"\n";
      std::cout << "\n  " << msg;
    }
    catch (std::exception& ex)
    {
      check(false);
      std::string msg = " -- \"" + name + "\"\n";
      std::cout << "\n  " << msg;;
      std::cout << "\n  " << ex.what();
    }
    return result;
  }
  //----< display test results >---------------------------------------

  void TestExecutor::check(bool result)
  {
    if (result)
      std::cout << "\n  passed";
    else
      std::cout << "\n  failed";
  }

  ///////////////////////////////////////////////////////////////////////
  // TestExecutive class
  // - executes a sequence of tests with the help of TestExecutor

  class TestExecutive
  {
  public:
    using TestItem = struct {
      std::shared_ptr<ITest> pTest;
      std::string testName;
    };
    using Tests = std::vector<TestItem>;

    bool doTests();
    void registerTest(std::shared_ptr<ITest> pTest, const std::string& testName);
    void registerTest(const TestItem& ts);
    void clearTests();
  private:
    Tests tests_;  // collection of test items
  };
  //----< store test in test collection for later execution >--------

  inline void TestExecutive::registerTest(std::shared_ptr<ITest> pTest, const std::string& testName)
  {
    TestItem ts{ pTest, testName };
    tests_.push_back(ts);
    // push_back copies the test item
    // - that makes a new shared_ptr sharing the same test as the original
    // - it also makes a copy of the test name string
  }

  inline void TestExecutive::registerTest(const TestItem& ts)
  {
    tests_.push_back(ts);
    // push_back copies the test item
    // - that makes a new shared_ptr sharing the same test as the original
    // - it also makes a copy of the test name string
  }

  inline void TestExecutive::clearTests()
  {
    tests_.clear();
  }

  //----< execute each stored test >---------------------------------

  inline bool TestExecutive::doTests()
  {
    TestExecutor tester;
    bool result = true;
    for (auto item : tests_)
    {
      bool tResult = tester.execute(item.pTest, item.testName);
      if (tResult == false)
        result = false;
    }
    return result;
  }
}


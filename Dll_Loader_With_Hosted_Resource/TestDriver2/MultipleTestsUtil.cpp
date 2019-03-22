////////////////////////////////////////////////////////////////////////////
// MultipleTestsUtil.cpp : multiple test classes for CodeUtilities pkg    //
// ver 1.0                                                                //
//                                                                        //
// Application : OOD F18 Project 2 Help                                   //
// Platform    : VS17 Community - Windows 10 Professional x64             //
// Author      : Jim Fawcett, CSE687 - OOD, Fall 2018                     //
// Source      : Ammar Salman, EECS Department, Syracuse University       //
//               313/788-4694, hoplite.90@hotmail.com                     //
////////////////////////////////////////////////////////////////////////////
/*
*  Package description:
* ======================
*  This is a 'multiple tests' test driver DLL package. It follows the same
*  protocol defined in ITest.h package. For DllLoader, it doesn't care
*  if the TestDriver contains one or many Test Classes as it will attempt
*  to get the collection of ITest*.
*
*  Required files:
* =================
*  MultipleTestsUtil.cpp
*  ITests.h
*  CodeUtilities.h
*
*  Maintainence History:
* =======================
*  ver 2.0 : 07 Oct 2018
*  - replaced all tests
*  - Ammar's infrastructure has not been modified
*  ver 1.0 - first release
*/

#define IN_DLL
#define ARG_SIZE 256

#include "../ITests.h"
#include "../CodeUtilities/CodeUtilities.h"
#include "../../Utilities/DateTime/DateTime.h"

#include <iostream>
#include <string>
#include <string.h>

using namespace Utilities;
using namespace std;

//////////////////////////////////////////////////////////////////
// Test class Demo1

class Demo1 : public ITest {
public:
  DLL_DECL bool test()
  {
    if (pRes_ == nullptr)
    {
      std::cout << "\n  no logger available";
      return false;
    }
    pRes_->setTerminator("");
    pRes_->write("\n  Testing Tester Package");
    pRes_->write("\n ========================");

    pRes_->write("\n  Test:   " + name());
    pRes_->write("\n  Author: " + author());
    pRes_->write("\n  " + Utilities::DateTime().now());

    pRes_->write("\n");
    pRes_->write("\n  Testing Demo1 class");
    pRes_->write("\n ---------------------");

    pRes_->write("\n  Test:   " + name());
    pRes_->write("\n  Author: " + author());
    pRes_->write("\n--Test always passes");
    return true;
  }
  DLL_DECL std::string name()
  {
    return "Demo1";
  }
  DLL_DECL std::string author()
  {
    return "Fawcett";
  }
  DLL_DECL void acceptHostedResource(ILog* pRes)
  {
    pRes_ = pRes;
  }
private:
  ILog* pRes_ = nullptr;
};

//////////////////////////////////////////////////////////////////
// Test class Demo2

class Demo2 : public ITest {
public:
  DLL_DECL bool test()
  {
    if (pRes_ == nullptr)
    {
      std::cout << "\n  no logger available";
      return false;
    }

    pRes_->write("\n");
    pRes_->write("\n  Testing Demo2 class");
    pRes_->write("\n ---------------------");

    pRes_->write("\n  Test:   " + name());
    pRes_->write("\n  Author: " + author());
    pRes_->write("\n--test always fails");
    return false;
  }
  DLL_DECL std::string name()
  {
    return "Demo2";
  }
  DLL_DECL std::string author()
  {
    return "Fawcett";
  }
  DLL_DECL void acceptHostedResource(ILog* pRes)
  {
    pRes_ = pRes;
  }
private:
  ILog* pRes_ = nullptr;
};
//////////////////////////////////////////////////////////////////
// Test class Demo3

class Demo3 : public ITest {
public:
  DLL_DECL bool test()
  {
    if (pRes_ == nullptr)
    {
      std::cout << "\n  no logger available";
      return false;
    }

    pRes_->write("\n");
    pRes_->write("\n  Testing Demo3 class");
    pRes_->write("\n ---------------------");

    pRes_->write("\n  Test:   " + name());
    pRes_->write("\n  Author: " + author());
    pRes_->write("\n--Test always throws");
    std::exception ex("\n  exception was thrown");
    throw ex;
    return true; // won't get here
  }
  DLL_DECL std::string name()
  {
    return "Demo3";
  }
  DLL_DECL std::string author()
  {
    return "Fawcett";
  }
  DLL_DECL void acceptHostedResource(ILog* pRes)
  {
    pRes_ = pRes;
  }
private:
  ILog* pRes_ = nullptr;
};

//////////////////////////////////////////////////////////////////
// Container that's responsible for returning a collection of the
// previous 2 test classes. It will return a collection of:
// 1) ITest* that points to an instance of TestProcessCmdArgs
// 2) ITest* that points to an instance of TestConverter

class TestCollection : public ITests {
	DLL_DECL std::vector<ITest*> tests();
};

// this is where each test class is instantiated and added to collection
DLL_DECL std::vector<ITest*> TestCollection::tests()
{
	std::vector<ITest*> tests_vec;
	tests_vec.push_back(new Demo1); 
  tests_vec.push_back(new Demo2);
  tests_vec.push_back(new Demo3);

	return tests_vec;
}

//////////////////////////////////////////////////////////////////////////////
// this section is where each Test Driver DLL completely differs from other
// test drivers. Although the same name can be used in all TestDrivers, the 
// actual instance of TestCollection is different in the way it returns 
// different collection of ITest*.

DLL_DECL ITests* get_ITests()
{
	return new TestCollection;
}



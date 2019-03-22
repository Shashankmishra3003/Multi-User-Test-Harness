////////////////////////////////////////////////////////////////////////////
// SingleTestUtil.cpp : single test class for CodeUtilities pkg           //
// ver 1.1                                                                //
//                                                                        //
// Application : OOD F18 Project 2 Help                                   //
// Platform    : VS17 Community - Windows 10 Professional x64             //
// Modified    : Jim Fawcett, CSE687 - OOD, Fall 2018
// Author      : Ammar Salman, EECS Department, Syracuse University       //
//               313/788-4694, hoplite.90@hotmail.com                     //
////////////////////////////////////////////////////////////////////////////
/*
*  Package description:
* ======================
*  This is a 'single test' test driver DLL package. It follows the same
*  protocol defined in ITest.h package. For DllLoader, it doesn't care
*  if the TestDriver contains one or many Test Classes as it will attempt
*  to get the collection of ITest*.
*
*  Required files:
* =================
*  SingleTestUtil.cpp 
*  ITest.h
*  CodeUtilities.h
*
*  Maintainence History:
* =======================
*  ver 1.1 : 07 Oct 2018
*  - added Hosted Resource infrastructure
*  - Tests use host functionality via passed pointer to
*    IHostedResource interface
*  ver 1.0 - first release
*/

#define IN_DLL
#define ARG_SIZE 256  // for the manufactured cmd args

#include "../ITests.h"
#include "../CodeUtilities/CodeUtilities.h"
#include "../../Utilities/DateTime/DateTime.h"

#include <iostream>
#include <string>
#include <string.h>

using namespace Utilities;
using namespace std;


// Concrete Test class that implements ITest.
// It performs tests on ProcessCmdArgs and Convereter classes in a single test class.

class SingleTestUtilities : public ITest {

public:
	DLL_DECL bool test();
	DLL_DECL std::string name();
	DLL_DECL std::string author();

  // get access to Hosted Resource
  DLL_DECL void acceptHostedResource(ILog* pRes)
  {
    pRes_ = pRes;
  }

private:
  ILog* pRes_ = nullptr;
};

DLL_DECL bool SingleTestUtilities::test()
{
  if (pRes_ == nullptr)
  {
    std::cout << "\n  no logger available";
    return false;
  }
  pRes_->setTerminator("");
  pRes_->write("\n");
  pRes_->write("\n  Testing CodeUtilities Package");
  pRes_->write("\n ===============================");

  pRes_->write("\n  Test:   " + name());
  pRes_->write("\n  Author: " + author());
  pRes_->write("\n  " + Utilities::DateTime().now());

  pRes_->write("\n\n  Testing ProcessCmdLine class");
  pRes_->write("\n ------------------------------");

  // create arguments for testing
	int argc = 5;
	char ** argv = new char*[argc];
	for (int i = 0; i < argc; ++i) {
		argv[i] = new char[ARG_SIZE]; 
	}

	strcpy_s(argv[0], ARG_SIZE, "dummy.exe");
	strcpy_s(argv[1], ARG_SIZE, "../");
	strcpy_s(argv[2], ARG_SIZE, "/s");
  strcpy_s(argv[3], ARG_SIZE, "\"*.h\"");
  strcpy_s(argv[4], ARG_SIZE, "\"*.cpp\"");

  pRes_->write("\n  Arguments:");
	for (int i = 0; i < argc; i++) {
	  pRes_->write("\n    arg #" + Utilities::Converter<int>::toString(i) + ":\t" + argv[i]);
	}

	ProcessCmdLine pcl(argc, argv);

  pRes_->write("\n\n  Extracted information:");

  pRes_->write("\n    Path:\t" + pcl.path());
  pRes_->write("\n    Options:\t");
  for (auto opt : pcl.options())
  {
    char temp = opt;
    std::string tmpStr;
    tmpStr += temp;
    pRes_->write(tmpStr + " ");
  }
  pRes_->write("\n    Patterns:\t");
	for (auto pat : pcl.patterns())
    pRes_->write(pat + " ");

	for (int i = 0; i < argc; i++)
		delete[] argv[i];
	delete[] argv;
  pRes_->write("\n");

  pRes_->write("\n  Testing Converter class");
  pRes_->write("\n -------------------------");

  pRes_->write("\n  Converting from string \"10\" to integer");
  int rslt = Converter<int>::toValue("10");
  if (rslt == 10)
    pRes_->write("\n  Converted integer is 10");
  else
    pRes_->write("\n  Converted interger is not 10");

  pRes_->write("\n  Converting from integer 10 to string");
  pRes_->write("\n  Converted string: \"" + Converter<int>::toString(10) + "\"");
  pRes_->write("\n  ");

	return true;
}

DLL_DECL std::string SingleTestUtilities::name()
{
	return string("CodeUtilities single Test DLL");
}

DLL_DECL std::string SingleTestUtilities::author()
{
	return string("Ammar Salman, modified by Jim Fawcett");
}

//////////////////////////////////////////////////////////////////////////////
// test collection 

class TestCollection : public ITests {
	DLL_DECL std::vector<ITest*> tests();
};

DLL_DECL std::vector<ITest*> TestCollection::tests()
{
	std::vector<ITest*> tests_vec;
	tests_vec.push_back(new SingleTestUtilities);

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


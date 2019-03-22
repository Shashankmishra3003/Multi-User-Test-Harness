/////////////////////////////////////////////////////////////////////////////////
// Executie.cpp : single test class for demostrating the project3 requirements //
// ver 1.1                                                                     //
//                                                                            //
// Application : OOD F18 Project 4                                            //
// Platform    : VS17 Community - Windows 8.1                                //
// Modified    : Shashank Mishra, CSE687 - OOD, Fall 2018                    //
// Author      : Ammar Salman, EECS Department, Syracuse University          //
//               313/788-4694, hoplite.90@hotmail.com                       //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package description:
* ======================
*  This is a 'Executive' test driver DLL package. It follows the same
*  protocol defined in ITest.h package. For DllLoader, it doesn't care
*  if the TestDriver contains one or many Test Classes as it will attempt
*  to get the collection of ITest*.
*
*  Required files:
* =================
*  StringUtilities.h, FileUtilities
*  ITest.h
**  Maintenance History:
*  --------------------
*  ver 1.0 : 05 Dec 2018
*  - first release
* */

#define IN_DLL
#define ARG_SIZE 256  // for the manufactured cmd args

#include "ITests.h"
#include <iostream>
#include<string.h>
#include"../Utilities/StringUtilities/StringUtilities.h"
#include"../Utilities/FileUtilities/FileUtilities.h"

using namespace std;
using Path = std::string;
using Message = std::string;
using Line = size_t;
using File = std::string;
using Files = std::vector<File>;

class DemoRequirements : public ITest
{

public:
	DLL_DECL bool test();
	DLL_DECL std::string name();
	DLL_DECL std::string author();
	DLL_DECL void requirement1();
	DLL_DECL void requirement2();
	DLL_DECL void requirement3a();
	DLL_DECL void requirement3b();
	DLL_DECL void requirement3c();
	DLL_DECL void requirement4();
	DLL_DECL void requirement5A();
	DLL_DECL void requirement5B();
	DLL_DECL void requirement6();
	DLL_DECL void requirement7();
	DLL_DECL void requirement8();
	DLL_DECL void requirement9();
	DLL_DECL void requirement10();

	// get access to Hosted Resource
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}

private:
	ILog* pRes_ = nullptr;
};

DLL_DECL void DemoRequirements::requirement1()
{
	Utilities::Title("Demonstrating Project #4 Requirements");
	Message msg = "Requirement #1 - Demonstrates the use of C++";
	Path path ="../Executive";
	Utilities::showDirContents(path, msg);
}

DLL_DECL void DemoRequirements::requirement2()
{
	Message msg = "Requirement #2 - Demonstrates the use of Windows Presentation Foundation for user Display";
	Path path ="../ClientGui";
	Utilities::showDirContents(path, msg);
}

DLL_DECL void DemoRequirements::requirement3a()
{
	Message msg = "Requirement #3A - Showing the Line of code which Demonstrates the Assembly of Project 1,2 and 3";
	Path path = "../Tester/Tester.cpp";
	Utilities::showFileLines(path, 111, 123, msg);
}
DLL_DECL void DemoRequirements::requirement3b()
{
	Message msg = "Requirement #3B - Showing the Line of code which Demonstrates Client-Server configuration, where Test Harness Accepts Test Requests from the client and Spawns Child Process to execute the Test Request";
	Path path = "../TestHarnessCore/TestHarness.cpp";
	Utilities::showFileLines(path, 139, 159, msg);
}

DLL_DECL void DemoRequirements::requirement3c()
{
	Utilities::Title("Requirement #3C - Shows that the Client need not wait for a reply for a TestRequest from the TestHarness before sending additional TestRequests, We can continuesly send test requests from the Client GUI without waiting for response");
	Utilities::Title("Result for Every Test Request is Stored in the TestLogClient Directory of the Client");

}

DLL_DECL void DemoRequirements::requirement4()
{
	Utilities::Title("The Client GUI interface demontrates that we have the Facility to select test libraries and send the selected test Files for testing");
	Utilities::Title("The Test Result Tab shows the result file for every Test Request. Clicking on the file will display the content inside");
}

DLL_DECL void DemoRequirements::requirement5A()
{
	Message msg = "Requirement #5A - Showing the Line of code which Demonstrates Message design at Client end ";
	Path path = "../ClientGui/MainWindow.xaml.cs";
	Utilities::showFileLines(path, 256, 265, msg);
}

DLL_DECL void DemoRequirements::requirement5B()
{
	Message msg = "Requirement #5B - Showing the Line of code which Demonstrates Message design at Test Harness End ";
	Path path = "../TestHarnessCore/TestHarness.cpp";
	Utilities::showFileLines(path, 125, 135, msg);
}

DLL_DECL void DemoRequirements::requirement6()
{
	Message msg = "Requirement #6 - Showing the Line of code which Demonstrates that message is being sent from client to Test Harness. The File from TestLibraries directory is sent to ";
	Path path = "../ClientGui/MainWindow.xaml.cs";
	Utilities::showFileLines(path, 228, 251, msg);
}

DLL_DECL void DemoRequirements::requirement7()
{
	Message msg = "Requirement #7 - Demonstrates the line of code which shows that each TestRequest running in its own Process Pool child process";
	Utilities::Title("Each Test request is executed Correctly as a sequesnce of Tests");
	Path path = "../TestHarnessCore/TestHarness.cpp";
	Utilities::showFileLines(path, 153, 172, msg);
}

DLL_DECL void DemoRequirements::requirement8()
{
	Message msg = "Requirement #8 - Demonstrates the line of code which shows that test results are sent via a result message back to the client";
	Path path = "../Tester/Tester.cpp";
	Utilities::showFileLines(path, 153, 166, msg);
}

DLL_DECL void DemoRequirements::requirement9()
{
	Utilities::Title("Requirement #9 - Demonstrates taht two concurrent clients are running");
	Utilities::Title("We can see that there are two Client windows and each Client can send test request to the the Test Harness.");
	Utilities::Title("The test Harness Executes the tests and returns the result cak to client.");
	Utilities::Title("The test result can be fount at TestLogFromChildProcess, which is the directory at Client end");
}

DLL_DECL void DemoRequirements::requirement10()
{
	Message msg = "Requirement #10 - Demonstrates the Automated Test Suite, which shows the lines of code from the DLL which is executed by One of the Child Process Console which shows all the requirements.";
	Utilities::Title("Each Child process Sends the test result in a Log file back to the Client, this log file is at Directory, /TestLogFromChildProcess");
	Utilities::Title("This shows the completion of Execution Cycle,Client sends Test request through UI and child Process sends the result back to the Client");
	Path path = "../Executive/Executive.cpp";
	Utilities::showFileLines(path, 76, 89, msg);
}
DLL_DECL bool DemoRequirements::test()
{
	
	DemoRequirements::requirement1();
	DemoRequirements::requirement2();
	DemoRequirements::requirement3a();
	DemoRequirements::requirement3b();
	DemoRequirements::requirement3c();
	DemoRequirements::requirement4();
	DemoRequirements::requirement5A();
	DemoRequirements::requirement5B();
	DemoRequirements::requirement6();
	DemoRequirements::requirement7();
	DemoRequirements::requirement8();
	DemoRequirements::requirement9();
	DemoRequirements::requirement10();

	return true;
}

DLL_DECL std::string DemoRequirements::name()
{
	return string("Demonstrating project4 Requirements");
}

DLL_DECL std::string DemoRequirements::author()
{
	return string("Shashank Mishra");
}

//////////////////////////////////////////////////////////////////////////////
// test collection 

class TestCollection : public ITests {
	DLL_DECL std::vector<ITest*> tests();
};

DLL_DECL std::vector<ITest*> TestCollection::tests()
{
	std::vector<ITest*> tests_vec;
	tests_vec.push_back(new DemoRequirements);

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

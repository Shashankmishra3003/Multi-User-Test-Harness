///////////////////////////////////////////////////////////////////////////////
// Tester.h - Loads Test Requests from DLL and executes the test            //
// ver 1.1                                                                 //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018        //
// Author:Shashank Mishra, Syracuse University			            	 //
//		  smishr06@syr.edu                                              //
/////////////////////////////////////////////////////////////////////////
/*  Package description :
*======================
* -Loads Test Request
* -Runs the Loaded test in exception safe code block
*
*  Required files :
*================ =
*  StringUtilities.h, FileUtilities
*  ITest.h, TestUtilities.h
*  Maintenance History :
*--------------------
*  ver 1.0 : 14 Nov 2018
* -first release
*/

#include "Tester.h"
#include "../Dll_Loader_With_Hosted_Resource/ITests.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../Utilities/FileSystem/FileSystem.h"
#include "../Utilities/FileUtilities/FileUtilities.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <memory>
#include <vector>

namespace Testing
{
	//----< initialize paths and logger >------------------------------

	Tester::Tester(EndPoint from, EndPoint to)
		:comm_(from, "Tester"), from_(from), to_(to)
	{
		comm_.start();
		comm_.setSendFilePath("../TestResultByChildProcess");

		requestPath("../TestRequests");
		libPath("../TestHarness_Directory");
		pLog(Utilities::Logger<0, Utilities::Lock>::getInstance());
		pLog()->setTerminator("");
	}
	//----< deallocate logger >----------------------------------------

	Tester::~Tester()
	{
		delete pLog();
	}
	//----< load Test Requests >-----------------------------------------
	/*
	* - In Project #4 serialized TestRequests, sent by client, will be
	*   extracted from Comm channel.
	*/
	void Tester::loadTestRequests()
	{
		std::vector<std::string> files = FileSystem::Directory::getFiles(requestPath());
		std::ifstream in;
		for (auto file : files)
		{
			if (!Utilities::openReadFile(requestPath() + "\\" + file, in))
				continue;
			std::ostringstream sstr;
			sstr << in.rdbuf();
			TestRequest tr = TestRequest::fromString(sstr.str());
			requests.valueRef().push_back(tr);
			pLog()->write("\n  Loaded TestRequests for testing: \"" + tr.name() + "\"");
			pLog()->write("\n  " + tr.toString() + "\n");
		}
	}
	//----< load libraries >---------------------------------------------
	/*
	* - Loads all libraries specified in a TestRequest from libPath.
	* - Uses DllLoader member, dllLoader_, to load dlls and extract
	*   their tests.
	*/
	void Tester::loadLibraries(TestRequest& tr)
	{
		std::cout << tr.toString();
		std::string path = FileSystem::Path::getFullFileSpec(libPath());
		pLog()->write("\n  Loading from: \"" + path + "\"");

		std::vector<std::string> files;
		for (Dll dll : tr.request.valueRef())
		{
			std::cout << "\nDLL" << dll;
			files.push_back(dll);
		}

		for (auto file : files)
		{
			pLog()->write("\n    " + file);
			std::string fileSpec = path + "\\" + file;
			dllLoader_.loadAndExtract(fileSpec);
		}
		pLog()->write("\n");
	}
	//----< executes tests >--------------------------
	/*
	* - Executes tests held by DllLoader instance,
	*   using TestUtilities infrastructure.
	*/
	bool Tester::doTests()
	{
		using sPtr = std::shared_ptr<ITest>;
		Utilities::TestExecutive te;
		for (ITest* test : dllLoader_.tests()) {
			// Host passes logger resource pointer to test
			// - test will use logger via this pointer
			test->acceptHostedResource(pLog.valueRef());
			sPtr pTest(test);
			te.registerTest(pTest, pTest->name());
		}
		bool result = te.doTests();
		return result;
	}

	//----< Load tests and create Log file >--------------------------
	/*
	* - Create Log file for every Test request
	*   using TestUtilities infrastructure.
	  - Send the Log file back to the client who
		created the test request.
	*/
	void Tester::runTests(TestRequest tr, MsgPassingCommunication::Message ready)
	{
		while (true)
		{
			comm_.postMessage(ready);
			MsgPassingCommunication::Message msg = comm_.getMessage();

			if (msg.command() == "testReq")
			{
				std::string fileSpec = "..\\TestResultByChildProcess\\" + msg.value("FileName") + "LogFile.txt";
				std::ofstream outfileStrm(fileSpec, std::ios::out);
				if (outfileStrm.good())
					pLog()->addStream(&outfileStrm);

				tr = TestRequest::fromString(msg.value("Request"));
				loadLibraries(tr);
				doTests();
				clear();
				pLog()->removeStream(&outfileStrm);
				outfileStrm.close();
				//////////////////////////////////////////////////////////////////////
				/*- sending the file back to the client
					the file has the name same as the test request with Current Time Stamp
				*/
				MsgPassingCommunication::Message logMessage;
				logMessage.to(msg.from());
				logMessage.from(msg.to());
				logMessage.value("sendingFile");
				logMessage.attribute("FileName", msg.value("FileName"));
				logMessage.command("sendingFile");
				logMessage.name("ResultFile");
				logMessage.attribute("sendingFile",msg.value("FileName") + "LogFile.txt");
				comm_.postMessage(logMessage);
				std::cout << "\nLog File in HTTP style Sent to Client: " << logMessage.toString();


			}
		}
	}
}

using namespace Testing;

int main(int argc, char* argv[])
{
	std::cout << Utilities::sTitle("\nTesting the Tester using Dll Loader and Logger: ");

	int port1 = std::stoi(argv[0]);
	int port2 = std::stoi(argv[1]);

	EndPoint endPoint3("localhost", port1);
	EndPoint endPoint4("localhost", port2);
	std::cout << "\nInitializing the Tester: ";

	Tester tester(endPoint3, endPoint4);
	TestRequest tr;

	MsgPassingCommunication::Message ready;
	ready.from(tester.from_);
	ready.to(tester.to_);
	ready.command("ready");
	std::cout << "\nTester Sending Message: " + ready.command() +"\n";

	tester.recv = std::thread([&]() { tester.runTests(tr, ready); });
	tester.recv.join();

	Utilities::putline(2);
	return 0;

}

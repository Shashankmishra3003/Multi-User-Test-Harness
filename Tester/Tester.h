#pragma once
///////////////////////////////////////////////////////////////////////////////
// Tester.h - Loads Test Requests from DLL and executes the test            //
// ver 1.1                                                                 //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018        //
// Author:Shashank Mishra, Syracuse University			            	 //
//		  smishr06@syr.edu                                              //
/////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package defines one class, Tester:
*  - uses DllLoader to load libraries and extract tests.
*  - provides pointer to its member logger to each test, then
*    executes it, using the infrastructure from TestUtilities.
*
*  Required Files:
*  ---------------
*  Tester.h,DllLoader.h, DllLoader.cpp
*  ISingletonLogger.h, SingletonLogger.h
*  Properties.h,
*  Comm.h,Cpp11-BlockingQueue.h
*  
*  Maintenance History:
--------------------------
   ver 1.1 : 14 Nov 2018
   -Moved it to a different Package
*  --------------------
*  ver 1.0 : 21 Oct 2018
*  - first release
*
*/
#include "../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include "../TestRequest/TestRequest.h"
#include "../Utilities/SingletonLogger/ISingletonLogger.h"
#include "../Utilities/SingletonLogger/SingletonLogger.h"
#include "../Utilities/Properties/Properties.h"
#include "../CppCommWithFileXfer/Message/Message.h"
#include "../CppCommWithFileXfer/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Dll_Loader_With_Hosted_Resource/DllLoader/DllLoader.h"
#include <vector>
#include <string>

namespace Testing
{
	using namespace Utilities;
	using namespace MsgPassingCommunication;

	///////////////////////////////////////////////////////////////////
	// Tester class
	// - Loads TestRequests from requestPath.
	//   Each TestRequest provides names of Dlls to load and test
	// - Loads Test Dynamic Link Libraries (Dlls) from libPath.
	// - Executes tests defined in each Dll.
	// - Each tests logs ouputs using Tester class's logger

	using Requests = std::vector<TestRequest>;

	class Tester
	{
	public:
		Tester(EndPoint from, EndPoint to);
		~Tester();

		Property<Requests> requests;
		Property<std::string> requestPath;
		Property<std::string> libPath;
		Property<ILog*> pLog;
		EndPoint to_;
		EndPoint from_;
		Comm comm_;
		std::thread recv;

		void loadTestRequests();
		void loadLibraries(TestRequest& tr);
		bool doTests();
		void runTests(TestRequest tr, MsgPassingCommunication::Message ready);

		void setLibraryPath(std::string& path)
		{
			libPath(path);
		}
		void clear()
		{
			dllLoader_.clear();
		}
	private:
		DllLoader dllLoader_;
	};

}

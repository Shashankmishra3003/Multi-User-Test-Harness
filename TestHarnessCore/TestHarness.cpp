///////////////////////////////////////////////////////////////////////
// TestHarness.h - Proveides the Core functionality                   //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Project3  CSE687 - Object Oriented Design            //
// Author:		Shashank Mishra, Syracuse University				 //
//				smishr06@syr.edu									 //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
*--------------------
*-Starts the Test Harness,
*-stops the test Harness, receivs message and sends message
*-using the Comm chanell
*-receives message from the Client
*-Creates specified number of child processses and respawns a
*-new process
 -
* Required File
*---------------------------------
*-TestHarness.h,Process.h,
*
* Maintenance History:
*---------------------------------
* ver 1.1 : 04 Dec 2018
*-Added Call back functionalit to respawn Child Processes
* ver 1.0 : 14 Nov 2018
 * - first release
*/


#include "TestHarness.h"
#include "../Process/Process.h"

TestHarness::TestHarness(EndPoint from, EndPoint to) 
	:comm_(from, "recvr"), to_(to), from_(from)
{
	comm_.start();
	comm_.setSaveFilePath("../TestHarness_Directory");
}
/*
-Prints the Request Queue and the Ready Queue size when stopping
*/
TestHarness::~TestHarness()
{
	std::cout << "\n  TestHarness Comm shutting down :";
	std::cout << "\n  RequestQ size = " + Utilities::Converter<size_t>::toString(requestQ_.size());
	std::cout << "\n  ReadyQ size   = " + Utilities::Converter<size_t>::toString(readyQ_.size());
	comm_.stop();
}
/*
-Starts the receive thread and Dispatch Thread of test harness
*/
void TestHarness::start()
{
	std::cout << "\n  Starting TestHarness Receive thread, listening on: " << from_.toString();
	recvr = std::thread([&]() { recvMessages(); });
	recvr.detach();

	std::cout << "\n  Starting TestHarness Dispatch thread: ";
	dspat = std::thread([&]() { dispatchMessages(); });
	dspat.detach();
}

////////////////////////////////////////////////////////////////////
/*
-Stops the Test Harness
*/
void TestHarness::stop()
{
	Message stop;
	stop.command("\n TestHarness Stops: ");
	stop.to(from_);
	stop.from(from_);
	comm_.postMessage(stop);
}

//----< waits for TestHarness to shutdown >------------------------

void TestHarness::wait()
{
	if (recvr.joinable())
		recvr.join();
	if (dspat.joinable())
		dspat.join();
}

//----< receive and post messages to appropriate queues >----------

void TestHarness::recvMessages()
{
	while(true)
	{
		Message msg = comm_.getMessage();
		std::cout << "\n  TestHarness received message: " + msg.command();
		if (msg.command() == "ready")
		{
			std::cout << "\n  TestHarness posting message: " + msg.command();
			readyQ_.enQ(msg);
		}
		else if (msg.command() == "testReq")
		{
			std::cout << "\n  TestHarness posting message: " + msg.command();
			requestQ_.enQ(msg);
		}
		else if (msg.command() == "sendingFile")
		{
			std::cout << "\n-------SendingFile----------------\n";
		}
		else
		{
			std::cout << "\n Invalid message: ";
		}
	}
	
}

//----< deQ messages and send to testers >-------------------------

void TestHarness::dispatchMessages()
{
	while (true)
	{
		Message trMsg = requestQ_.deQ();
		std::cout << "\n  TestHarness deQ Test Request Message form the Blocking Queue: " + trMsg.command();

		Message rdyMsg = readyQ_.deQ();
		std::cout << "\n  TestHarness deQ Ready Message form the Blocking Queue: " + rdyMsg.command();

		trMsg.to(rdyMsg.from());  // send request to ready child
		std::cout << "\n    TestHarness sending Message: " + trMsg.command();
		comm_.postMessage(trMsg);
	}
}

int main()
{
	//Ports for sending and Test request 
	MsgPassingCommunication::EndPoint ep1("localhost", 8081);
	MsgPassingCommunication::EndPoint ep2("localhost", 8082);

	TestHarness testHarness(ep2, ep1);
	testHarness.start();
	/*
	-Creates the Chile Processes for Executing the Tests from the Client
	-Number of child processes can be changes here
	*/
	const int noOfChildProc = 2;
	int i = 0;
	CBP callback;
	Process childProcess[noOfChildProc],newChildProc;
	for (int i = 0; i < noOfChildProc; i++)
	{
		childProcess[i].title("Child Process"+std::to_string(i));
		childProcess[i].application("../Debug/Tester.exe");
		childProcess[i].commandLine(std::to_string(8085+i)+" 8082");
		childProcess[i].create();

	//----<Spawns a new child Process whenever a running child process is Destroyed>-----
		callback = [&]()
		{ 
			newChildProc.title("New Child Process");
			newChildProc.application("../Debug/Tester.exe");
			newChildProc.commandLine(std::to_string(i += 1) + " 8082");
			newChildProc.create();
			newChildProc.setCallBackProcessing(callback);
			newChildProc.registerCallback();
		};
		childProcess[i].setCallBackProcessing(callback);
		childProcess[i].registerCallback();
	}
	i+= 8085;
	testHarness.wait();
	getchar();
}
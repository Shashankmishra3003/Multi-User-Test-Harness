////////////////////////////////////////////////////////////////////////////////
// Client.cpp - Creates TestRequest and stores in requestPath                //
// ver 1.0                                                                  //
// Source:-Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
// Author:-Shashank Mishra                                                //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  Client creates test request and stores in requestPath.
*  - This simulates what a Project #4 client does, e.g., creates
*    TestRequests and sends to TestHarness via a Comm channel.
*
*  Required Files:
*  ---------------
*  Client.cpp
*  TestRequest.h, TestRequest.cpp
*  DateTime.h, DateTime.cpp
   Comm.h, FileUtilities.h, Message.h
*
*  Maintenance History:
*  --------------------
   ver 1.1 14 Nov 2018 
   - Added function for sending Test requests using Comm
   --------------------
*  ver 1.0 : 23 Oct 2018
*  - first release
*/
#include "../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include "../TestRequest/TestRequest.h"
#include "../Utilities/FileUtilities/FileUtilities.h"
#include "../CppCommWithFileXfer/Message/Message.h"
#include <iostream>


namespace Testing
{
	
	std::string mock_Test(int i)
	{	
		TestRequest tr1;

		if (i == 0)
		{
			tr1.name("tr1");
			tr1.request.valueRef().push_back("Executive.dll");

		}
		if (i == 1)
		{
			tr1.name("tr2");
			tr1.request.valueRef().push_back("TestDriver1.dll");

		}
		if (i == 2)
		{
			tr1.name("tr3");
			tr1.request.valueRef().push_back("TestDriver2.dll");

		}
		tr1.author("Shashank");
		tr1.date(DateTime().now());

		Utilities::title("/n  Created test request: ");
		std::cout << "\n  " << tr1.toString();
		
		return tr1.toString();
	}
}

using namespace MsgPassingCommunication;
class Client
{
public:
	Client(EndPoint from, EndPoint to);
	~Client();
	void start();
	void wait();
	void sendMessages();
	void getLogs();

private:
	std::thread recv;
	std::thread recvLog;
	Comm comm_;
	EndPoint to_;
	EndPoint from_;
	int requestCounter = 0;
};
//----< initializes endpoints and starts comm >--------------------

Client::Client(EndPoint from, EndPoint to) : comm_(from, "sender"), to_(to), from_(from)
{
	comm_.start();
	comm_.setSendFilePath("../TestLibraries");
	comm_.setSaveFilePath("../TestLogFromChildProcess");
}
//----< stops comm and detaches thread if needed >-----------------

Client::~Client()
{
	std::cout << "\n  Client Comm shutting down: ";
	comm_.stop();
	if (recv.joinable())
		recv.detach();
}

//----< creates thread to send test request messages >-------------

void Client::start()
{
	std::cout << "\n  Starting Client's send thread listening on: " + from_.toString();
	recv = std::thread([&]() { sendMessages(); });
	if (recv.joinable())
		recv.join();

	recvLog = std::thread([&]() {getLogs(); });
	if (recvLog.joinable())
		recvLog.join();
}
//----< waits for all messages to be sent >------------------------

void Client::wait()
{
	if (recv.joinable())
		recv.join();
	if (recvLog.joinable())
		recvLog.join();
}

// Gets the Test Result for Client's Test Request
void Client::getLogs()
{
	while (true)
	{
		MsgPassingCommunication::Message msg = comm_.getMessage();
		if (msg.command() == "sendingFile")
		{
			std::cout << "\nTest Result for Client`s Test Request:" << msg.value("FileName") << "received";
			requestCounter--;
			std::cout << "\n Waiting to Process " << requestCounter << " more Test Requests Pending ";
		}
		if (requestCounter == 0)
		{
			std::cout << "\n  Received the results for all the Test requests, Client Shutting Down: ";
			exit(0);
		}
			
	}
}
//----< send request messages >------------------------------------
/*
*  - send thread will shut down when this method completes
*  - sleeps just for display so everything doesn't happen at once
*/
void Client::sendMessages()
{
	//Sending Test Suit DLL files to be tested
	
	MsgPassingCommunication::Message fileMsg;
	fileMsg.to(to_);
	fileMsg.from(from_);
	fileMsg.value("sendingFile");
	fileMsg.command("sendingFile");
	fileMsg.name("Executive");
	fileMsg.file("Executive.dll");
	comm_.postMessage(fileMsg);

	MsgPassingCommunication::Message testReq;
	testReq.name("Test Suite");
	testReq.attribute("Request", Testing::mock_Test(0));
	testReq.attribute("FileName", "Test Suite");
	testReq.to(to_);
	testReq.from(from_);
	testReq.command("testReq");
	std::cout << "\nTest Suite: " << testReq.toString();
	comm_.postMessage(testReq);
	requestCounter++;
	std::this_thread::sleep_for(std::chrono::milliseconds(30000));
	//Waits for 10 Seconds to demonstrate the Test Suit.

	//Sending the Remaining Test Requests
	MsgPassingCommunication::Message fileMsg1;
	fileMsg1.to(to_);
	fileMsg1.from(from_);
	fileMsg1.value("sendingFile");
	fileMsg1.command("sendingFile");
	fileMsg1.name("TestDLL1");
	fileMsg1.file("TestDriver1.dll");
	comm_.postMessage(fileMsg1);

	fileMsg1.name("TestDLL2");
	fileMsg1.file("TestDriver2.dll");
	comm_.postMessage(fileMsg1);
	std::this_thread::sleep_for(std::chrono::milliseconds(30000));

	MsgPassingCommunication::Message testReq1;
	testReq1.name("Test Request1");
	testReq1.attribute("Request", Testing::mock_Test(1));
	testReq1.attribute("FileName", "TestReq1");
	testReq1.to(to_);
	testReq1.from(from_);
	testReq1.command("testReq");
	std::cout << "\nTestReq1: ";
	comm_.postMessage(testReq1);
	requestCounter++;

	testReq1.attribute("Request", Testing::mock_Test(2));
	testReq1.attribute("FileName", "TestReq2");
	comm_.postMessage(testReq1);
	requestCounter++;

}


using namespace Testing;
int main()
{
	Utilities::Title("Demonstrating Client: ");
	std::this_thread::sleep_for(std::chrono::milliseconds(4000));

	MsgPassingCommunication::EndPoint ep1("localhost", 8081);
	MsgPassingCommunication::EndPoint ep2("localhost", 8082);

	Client client(ep1, ep2);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	client.start();
	client.wait();
	
	std::cout << "\n";
	return 0;
}
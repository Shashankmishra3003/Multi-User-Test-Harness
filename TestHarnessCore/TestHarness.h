#pragma once

///////////////////////////////////////////////////////////////////////
// TestHarness.h - Proveides theCore functionality                   //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Project3       CSE687 - Object Oriented Design       //
// Author:		Shashank Mishra, Syracuse University				 //
//				smishr06@syr.edu									 //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
*--------------------
*-Provides Methods for starting the Test Harness,
*-stoping the test Harness, receiving message and sending message
*-using the Comm chanell
 -
* Required File
*--------------------
*-Comm.h,Cpp11-BlockingQueue.h,
*-Message.h
*
*/

#include"../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include "../CppCommWithFileXfer/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../CppCommWithFileXfer/Message/Message.h"

using namespace MsgPassingCommunication;

class TestHarness
{
public:
	TestHarness(EndPoint from, EndPoint to);
	~TestHarness();
	void start();
	void wait();
	void stop();
	void recvMessages();
	void dispatchMessages();
private:
	BlockingQueue<Message> readyQ_;
	BlockingQueue<Message> requestQ_;
	std::thread recvr;
	std::thread dspat;
	Comm comm_;
	EndPoint to_;
	EndPoint from_;
};

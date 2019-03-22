#pragma once
////////////////////////////////////////////////////////////////////////////
// DllLoader.h : Test-Driver-DLL loader that uses ITests.h as protocole   //
// ver 2.0                                                                //
// Application : OOD F18 Project 2 Help                                   //
// Platform    : VS17 Community - Windows 10 Professional x64             //
// Modified    : Jim Fawcett, CSE687 - OOD, Fall 2018
// Author      : Ammar Salman, EECS Department, Syracuse University       //
//               313/788-4694, hoplite.90@hotmail.com                     //
////////////////////////////////////////////////////////////////////////////
/*
*  Package description:
* ======================
*  This package loads Test-Driver DLLs that follow the ITests protocol
*  defined in ITests.h 
*
*  The protocol forces all TestDrivers to have an implementation of ITests
*  interface which holds a collection of ITest pointers. This package will
*  attempt to load the DLL, get an instance of ITests* and retrieve the
*  std::vector<ITest*> contained within the DLL.
*
*  Required files:
* =================
*  ITests.h DllLoader.h DllLoader.cpp
*  Note: nothing else is needed since this will load the DLLs at RUNTIME.
*
*  Maintainence History:
* =======================
*  ver 2.0 : 07 Oct 2018
*  - converted testing to use TestUtilitiesDemo code
*  - added Hosted Resource infrastructure
*  - removed commented options for strings
*  ver 1.1 : 27 Sep 2018
*  - demonstrate some options for converting strings
*  ver 1.0 : 24 Sep 2018 : Ammar Salman
*  - first release
*/


#include "../ITests.h"
#include <string>
#include <vector>
#include <iostream>
#include <windows.h>

/////////////////////////////////////////////////////////////////////
// HostedResource class
// - supports tests using resources provided by Host
// - In this demo, we allow tests to use Host's display function, say()
// - For project #2 you will replace this with your logger facility

class DllLoader
{
public:
	DllLoader();
	~DllLoader();
	bool loadAndExtract(const std::string& dll); 
	const std::vector<ITest*>& tests() const; 
	void clear();  // unloads the DLL (if loaded) and clears all extracted tests

private:
	std::vector<ITest*> extractedTests_;  // contains the set of tests extracted from the TestDriver DLL
	HMODULE hmod;  // handle to the loaded DLL
};


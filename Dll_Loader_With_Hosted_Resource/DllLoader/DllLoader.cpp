////////////////////////////////////////////////////////////////////////////
// DllLoader.cpp : implementation and testing for DllLoader.h             //
// ver 2.0                                                                //
//                                                                        //
// Application : OOD F18 Project 2 Help                                   //
// Platform    : VS17 Community - Windows 10 Professional x64             //
// Author      : Ammar Salman, EECS Department, Syracuse University       //
//               313/788-4694, hoplite.90@hotmail.com                     //
////////////////////////////////////////////////////////////////////////////

#include "DllLoader.h"
#include <iostream>

using namespace std;

//----< string converter using std C++ >-----------------------------

std::wstring toWstring(const std::string& s)
{
  std::wstring wstr;
  for (auto ch : s)
  {
    wstr += (wchar_t)ch;
  }
  return wstr;  // will move
}

DllLoader::DllLoader(){}

DllLoader::~DllLoader()
{
	clear(); // to make sure the DLL is unloaded
}

// accepts a DLL path and attempts to load it and extract its tests

bool DllLoader::loadAndExtract(const std::string & dll)
{
  hmod = LoadLibrary(toWstring(dll).c_str()); // load DLL
  if (!hmod) return false;  // check if DLL was properly loaded

	// check if the DLL contains get_ITests() function ...
	// in other words, check if the DLL complies with the protocol

  using gITs = ITests * (*)();  // using instead of typedef

	gITs get_ITests = (gITs)GetProcAddress(hmod, "get_ITests");
	if (!get_ITests) return false;

	// execute get_ITests() to get the collection of tests
	ITests* tests = get_ITests();
	if (tests) { // check if tests is nullptr before attempting to get its tests collection
		std::vector<ITest*> extTsts = tests->tests(); 
		extractedTests_.insert(extractedTests_.begin(), extTsts.begin(), extTsts.end());
		return true;
	}

	// this will only be reached if get_ITests() returned nullptr in which case the 
	// TestDriver DLL contains no tests to execute.
	return false;
}

const std::vector<ITest*>& DllLoader::tests() const
{
	return extractedTests_;
}

void DllLoader::clear()
{
	if (hmod) FreeLibrary(hmod);
	extractedTests_.clear();
}

#ifdef TEST_DLL_LOADER

#include "../TestUtilities/TestUtilitiesDemo.h"
#include <iostream>
#include <iomanip>
#include <memory>

//----< uses TestUtilities Infrastructure >--------------------------

void doTests(DllLoader& loader, HostedResource* pRes)
{
  using sPtr = std::shared_ptr<ITest>;
  Utilities::TestExecutive te;
  for (ITest* test : loader.tests()) {
    // Host passes resource pointer to test - test will use hr via this pointer
    test->acceptHostedResource(pRes);
    sPtr pTest(test);
    te.registerTest(pTest, pTest->name());
  }
  te.doTests();
}

int main() {
	cout << "\n  Testing DLL Loader";
	cout << "\n ====================";

	DllLoader loader;

	cout << "\n  Loading '../TestLibraries/TestDriver1.dll' which is a single-test TestDriver...";

	bool loaded = loader.loadAndExtract("../Debug/TestDriver1.dll");
	if (!loaded) {
		cout << " failed to load dll or failed to load tests.\n    Terminating...\n";
		return -1; 
	}

	cout << " successfully loaded";
	cout << "\n    Extracted tests:\n";

  HostedResource hr1("\n  Hosted Resource Message: TestDriver1");
  doTests(loader, &hr1);

	cout << "\n  Loading '../TestLibraries/TestDriver2.dll' which is a multiple-tests TestDriver ...";
	loader.clear();

	loaded = loader.loadAndExtract("../Debug/TestDriver2.dll");
	if (!loaded) {
		cout << " failed to load dll or failed to load tests.\n    Terminating...\n";
		return -1;
	}

	cout << " successfully loaded";
	cout << "\n    Extracted tests:\n";

  HostedResource hr2("\n  HostedResource Message: TestDriver2");
  doTests(loader, &hr2);

	cout << "\n\n";
	return 0;
}

#endif
#pragma once
/////////////////////////////////////////////////////////////////////////
// TestExecutive.h - Demonstrates requirements done in project 4       //
//                                                                     //
//                                                                     //
// Platform:    MacBook Pro, Windows 10(dual boot)                     //
// Application: Project #4, CSE687 - Object Oriented Design, S2017     //
// Author:      Suruchi Singh, Syracuse University                     //
//              ssingh11@syr.edu				                       //
/////////////////////////////////////////////////////////////////////////
#include "TestExecutive.h"
/*
Module Operations:
==================
This class defines demonstrateReqs for demonstration purpose.

Public Interface:
=================
void executive();


Required files
No required Files

Maintenance History:
====================
ver 1.0 
- first release
*/

//#ifdef  Test

using namespace std;
void TestExecutive::demonstrate_req()
{
	cout << "\n==================== \nREQUIREMENT 2 \n====================" << endl;
	cout << "Provided the use the C++ standard library's streams for all console I/O and new and delete for all heap-based memory management."<<endl;
	cout << "\n==================== \nREQUIREMENT 3 \n====================" << endl;
	cout << "Provided a Repository program that provides functionality to publish, as linked web pages, the contents of a set of C++ source code files."<<endl;
	cout << "\n==================== \nREQUIREMENT 4 \n====================" << endl;
	cout << "For the publishing process, satisfied the requirements of CodePublisher developed in Project #3." << endl;
	cout << "\n==================== \nREQUIREMENT 5 \n====================" << endl;
	cout << "Provided a Client program that can upload files, and view Repository contents in a folder called '*ServersRepository*', as described in the Purpose section, above."<<endl;
	cout << "\n==================== \nREQUIREMENT 6 \n====================" << endl;
	cout << "Provides a message - passing communication system, based on Sockets, used to access the Repository's functionality from another process or machine."<<endl;
	cout << "\n==================== \nREQUIREMENT 7 \n====================" << endl;
	cout << "The communication system provides support for passing HTTP style messages using either synchronous request / response or asynchronous one - way messaging."<<endl;
	cout << "\n==================== \nREQUIREMENT 8 \n====================" << endl;
	cout << "The communication system supports sending and receiving streams of bytes.Streams are established with an initial exchange of messages."<<endl;
	cout << "\n==================== \nREQUIREMENT 9 \n====================" << endl;
	cout << "Includes an automated unit test suite that demonstrates I meet all the requirements of this project4 including the transmission of files." << endl;

}


int main()
{
	TestExecutive te;
	te.demonstrate_req();
	return 0;
	
}

//#endif //  Test
#pragma once
/////////////////////////////////////////////////////////////////////
// Publish.h - Creates a Code Publisher			                   //
// ver 1.0                                                         //  
//								     							   //
// Language:    C++, Visual Studio 2015                            //
// Platform:    MacBook Pro, Windows 10(dual boot)                 //
// Application: Project #4, CSE687 - Object Oriented Design, S2017 //
// Author:      Suruchi Singh, Syracuse University                 //
//              ssingh11@syr.edu				                   //				   
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  ===================
*  Publishes all the files with extentions .cpp and .h and their dependent files on the browser.
*
*  Publish.h supports displays on the browser for:
*  - The contents of .cpp and .h files and the links to their dependent files and scope hiding and unhiding.
*
*
*
*  Public Interface Document:
*  ---------------------------
*  -void publishAllFiles();
*   =>creates html files obtained from the dependency table
*  -void toHtm(string pathof);
*    => generates html file of all the .h and .cpp files
*  -void createStartFile();
*    => creates an html file with all the links of the html files created
*
*  Maintanence History:
*  --------------------
*  ver 1.0
*  - first release
*
*/
#include <iostream>
#include <functional>
#include<unordered_map>
#include<vector>
#include<set>
#include <direct.h>
#include<fstream>
#include <iterator>
#include<string>

using namespace std;
//using namespace CodeAnalysis;

class PublishFiles
{
    private:
		string dirName = "../ServersRepository";
		vector<string> depfiles;
		vector<string> childvec;
		vector<string> filevector;
    public: 
	  using depTablevalues = std::unordered_map<std::string, std::set<std::string>>;
	  depTablevalues depTable;
	  PublishFiles(std::unordered_map<std::string, std::set<std::string>> getDepTable)
	  {
		  depTable = getDepTable;  
	  }
	  void createStyleSheet();  //create style sheet
	  void createStartFile();   //create a home page
	  void publishAllFiles();   //runs on dependency table to get files and their dependencies
	  void toHtm(string pathof); //creates html files
	  void createJavaScript();
};


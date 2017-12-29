#pragma once
/////////////////////////////////////////////////////////////////////
// DepTable.h - Creates a Dependency Table			               //
// ver 1.0                                                         //  
//																   //
// Language:    C++, Visual Studio 2015                            //
// Platform:    MacBook Pro, Windows 10(dual boot)                 //
// Application: Project #4, CSE687 - Object Oriented Design, S2017 //
// Author:      Suruchi Singh, Syracuse University                 //
//              ssingh11@syr.edu								   //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  ===================
*  DepTable uses TypeTable, FileSystem and AbstrSynTree packages to
*  provide create a dependency table such that all the files dependent
*  on other files are stored in the dependency table as well as the NoSqlDb.
*
*  It finds files for analysis using AbstrSynTree and, for each file found,
*  uses TypeTable to check whether the relationship exists
*  to build a Dependency Table
*
*  DepTable supports displays for:
*  - file name and a list of all .cpp and .h files that each file dependent on.
*  - the data when stored in NoSqlDb and when given an input to an XML file.
*
*
*  Required Files:
*  ---------------
*  - Parser.h, ActionsAndRules.h
*  - AbstrSynTree.h, AbstrSynTree.cpp
*  - IFileMgr.h, FileMgr.h, FileSystem.h
*  - DepAnal.h
*  - TypeTable.h
*  - NoSqlDb.h
*  - xmlpersist.h
*  - XMLDocument.h
*
*  Public Interface Document:
*  ---------------------------
*  - DepTable();     
*	 => Constructor to the class which initialises the ASTRef object.
*  - unordered_map<std::string, std::string> &getdepTable() { return dependencyTable; } 
*	 => returns the created dependency table
*  - vector<string> allfiles(); 
*	 => traverses all components of a given filePath*
*  - NoSqlDb<string> getdb();  
*	 => get the NoSqlDb
*  - void printdatabase();   
*	 => print the generated NoSqlDb
*  - void writexml();    
*	 => write generated NoSqlDb to an Xml File
*  - void getTypetable(unordered_map<std::string, std::pair<std::string, std::string>> typetable_);  
*	 => get the created Type Table
*  - std::unordered_map<std::string, std::set<std::string>> returndeptable();  
*	 => returns the Dependency Table 
*  - void traverseFilePath();   
*	 => get the root node of the FilePath and by recursive call, get each component of the FilePath
*  - void createDependencyTable(); 
*	 => calls a recursive function to add all dependencies into the dependency table of a given file
*  - void DFS(ASTNode* pNode); 
*	 => get all the components of the FilePath
*  - void makedeptable(string filespec);
*	 => recursive call that adds files and its dependencies into the dependency table
*
*  Maintanence History:
*  --------------------
*  ver 1.0
*  - first release
*
*/
#pragma warning (disable : 4101)
#ifndef DEPT_H
#define DEPT_H

#include <iostream>
#include <functional>
#include<unordered_map>
#include<vector>
#include<set>


#include"../Analyzer/DepAnal.h"
#include "../FileSystem/FileSystem.h"
#include "../FileMgr/FileMgr.h"
#include "../FileMgr/IFileMgr.h"


using namespace std;
using StrData = std::string;


using namespace CodeAnalysis;  

//class for Dependency Analysis
class DepTable
{
private:
	std::set<std::string> pathVector; //store FilePath
	using depTablevalues = std::unordered_map<std::string, std::set<std::string>>; 
	depTablevalues depTable;
	unordered_map<std::string, std::pair<std::string, std::string>> typetable; //store generated typetable 
	AbstrSynTree& ASTref_;

public:
	
	DepTable(); //Constructor to the class which initialises the ASTRef object.
	vector<string> allfiles(); //traverses all components of a given filePath
	void getTypetable(unordered_map<std::string, std::pair<std::string, std::string>> typetable_); //get the created Type Table
	std::unordered_map<std::string, std::set<std::string>> returndeptable(); //returns the Dependency Table 
	void traverseFilePath(); //get the root node of the FilePath and by recursive call, get each component of the FilePath
	void createDependencyTable(); //calls a recursive function to add all dependencies into the dependency table of a given file
	void DFS(ASTNode* pNode); //get all the components of the FilePath
	void makedeptable(string filespec); //recursive call that adds files and its dependencies into the dependency table

};



#endif
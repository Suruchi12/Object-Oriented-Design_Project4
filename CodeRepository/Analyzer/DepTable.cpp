#pragma once
/////////////////////////////////////////////////////////////////////
// DepTable.cpp - Creates a Dependency Table			            //
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
*  - DepTable.h
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
#include "DepTable.h"
#include<iostream>
#include<unordered_map>
#include<vector>
using namespace std;


//constructor to initialize the AbstrSynTree object
DepTable::DepTable() :ASTref_(Repository::getInstance()->AST())
{
}
//receive populated typetable
 void DepTable::getTypetable(unordered_map<std::string, std::pair<std::string, std::string>> typetable_)
{
	typetable = typetable_;
}


//Checks for all files when a path is given by traversing the directory
 vector<string> DepTable::allfiles()
{
	FileSystem::Directory directory;
	FileSystem::Path path;
	vector<string> directoryVector, fileVector, filefound;
	string extention = "";
	string currentDirectory = directory.getCurrentDirectory();
	directoryVector = directory.getDirectories(currentDirectory);
	for (auto ite = directoryVector.begin(); ite != directoryVector.end(); ite++)
	{
		fileVector = directory.getFiles(*ite); auto it = fileVector.begin();
		for (auto it = fileVector.begin(); it != fileVector.end(); it++)
		{
			extention = path.getExt(*it);
			if (extention == "cpp" || extention == "h")
			{
				string filename = "";
				filename.append(*ite);
				filename.append("/");
				filename.append(*it);
				filefound.push_back(filename);
			}
		}
	}
	return filefound;
}

//returns the created dependency table
 std::unordered_map<std::string, std::set<std::string>> DepTable::returndeptable()
{
	return depTable;
}

//get the root node of the FilePath and by recursive call, get each component
 void DepTable::traverseFilePath()
{
	ASTNode* pRoot = ASTref_.root();
	DFS(pRoot);
}

//recursive call to get each component(directory,files) of the FilePath provided
 void DepTable::DFS(ASTNode* pNode)
{
	static std::string path = "";
	if (pNode->path_ != path)
	{
		path = pNode->path_;
	}
	pathVector.insert(pNode->path_);
	for (auto pChild : pNode->children_)
		DFS(pChild);
}

//create a dependency table by calling a recursive function
 void DepTable::createDependencyTable()
{
	cout << "\n****************** \nDEPENDENCY TABLE\n******************\n" << "--------------------------------------------------------------\n";
	cout << std::left << std::setw(30) << "FileName" << "Dependent FileName \n" << "--------------------------------------------------------------\n";
   auto iter = pathVector.begin();
   while(iter!=pathVector.end())
	{
		makedeptable(*iter);
		iter++;

	}
}

//recursive function to store all dependencies of a file
 void DepTable::makedeptable(string filespec)
{
	string fname = "";
	if (filespec != "") {
		static std::string path = "";
		try {
			std::ifstream file(filespec);
			if (file.good()) {
				Scanner::Toker toker;
				vector<string> children;
				std::set<std::string> depVector;
				toker.returnComments();	toker.attach(&file);
				do {
					std::string tok = toker.getTok(); 
					for (auto tl : typetable) {
						if (tok == tl.first)  {
							string packagenamae = tl.second.second;   //get each dependency of the filename
							depVector.insert(packagenamae); //store dependencies in a vector 
						}
					}
				} while (file.good());
				depTable.insert(std::make_pair(filespec, depVector));
			}
			else {
				cout << "Cannot open the file!";
				exit(0);
			} std::cout << std::endl;
			for (auto depTableit = depTable[filespec].begin(); depTableit != depTable[filespec].end(); ++depTableit) {
				cout << std::left << filespec << "\n \t \t" << std::setw(30) << *depTableit << endl;  //print the dependency table
				cout << "\n \n";
			}
		}
		catch (std::logic_error& ex) {
			std::cout << "\n  " << ex.what();
		}
	}
}



//--------------------Dependency Table TEST STUB
#ifdef TEST_DEPTABLE
#define TEST_DEPTABLE
int main()
{
	DepTable dt;
	unordered_map<std::string, std::pair<std::string, std::string>> typetable;
	std::unordered_map<std::string, std::set<std::string>> temp;
	
	/*string Key="",Key1="";
	cout << "HI from dep table!"<<endl;
	std::pair<std::string, std::string> value,value1;
	Key1 = "Key1";
	value1.first = "value.first1";
	value1.second = "value.second1";
	typetable.insert(std::make_pair(Key1, value1));
	Key = "Key2";
	value.first = "value.first2";
	value.second = "value.second2";
	typetable.insert(std::make_pair(Key, value));
*/
	/*for (auto it : typetable)
		cout << it.first<<it.second.first<<it.second.second;*/
	
	TypeAnal ta; //create an object of TypeTable
	typetable = ta.doTypeAnal();  //return populated type table 
	//create an object for dependency table
	 dt.getTypetable(typetable);  //get the populated typetable in the dependency class
	 dt.traverseFilePath();
	 dt.createDependencyTable();
	temp = dt.returndeptable();  //get the populated dependency table
	NoSqlDb<string> db1 = dt.getdb();			//gets the dependency database


	return 0;
}

#endif TEST_DEPTABLE

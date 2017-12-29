#pragma once
/////////////////////////////////////////////////////////////////////
// TypeTable.h - Creates a Type Table			                   //
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
*  TypeTable uses AbstrSynTree package to build the type table.
*
*  TypeTable supports displays for:
*  - a list of the names,types and filenames of all classes,structs,using,typedefs and enums.
*
*
*  Required Files:
*  ---------------
*  - Parser.h, ActionsAndRules.h
*  - AbstrSynTree.h
*
*  Public Interface Document:
*  ---------------------------
*  - void insertintable(CodeAnalysis::ASTNode *pNode);  
*    =>inserts the name and type and filename associated with each class,struct,enum and using
*  - void display();  
*    =>displays the type table
*  - string getName(){return name1;} 
*    =>getter function for Name of class/struct/enum/using
*  - string  TypeTable::getFileName() { return filename1; }
*    =>getter function for FileName
*  - string  TypeTable::getTypeName() {return typename1;}
*    =>getter function for TypeName
*  - unordered_map<std::string, std::pair<std::string, std::string>> &getTypeTable() 
*    =>returns populated TypeTable that can be used by non class members
*
*  Maintanence History:
*  --------------------
*  ver 1.0
*  - first release
*
*/
#include "../Parser/ActionsAndRules.h"
#include"../AbstractSyntaxTree/AbstrSynTree.h"
#include <iostream>
#include <functional>
#include<unordered_map>
#include<vector>
#include<iomanip>
#pragma warning (disable : 4101) 
using namespace std;


class TypeTable
{
private :
	std::unordered_map<std::string, std::pair<std::string,std::string>> typetable_;
	unordered_map<string, vector<string>> typetable1;
	std::pair<std::string,std::string> value;
	using FileName = std::string;
	using TypeName = std::string;
	using Namestr = std::string;
	using Namespace = std::string;
	Namestr name1;
	FileName filename1;
	TypeName typename1;
	vector<string> vec1;
public:
	TypeTable() {  }

	void insertintable(CodeAnalysis::ASTNode *pNode); //inserts into the type table
	void display();   //displays the type table
	string getName(){return name1;}   //getter method for name
	string  TypeTable::getFileName() { return filename1; } //getter method for filename
	string  TypeTable::getTypeName() {return typename1;}   //getter method for typename
	//function to return the typetable created
	unordered_map<std::string, std::pair<std::string, std::string>> &getTypeTable()
	{
		return typetable_;
	}

};


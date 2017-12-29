#pragma once
/////////////////////////////////////////////////////////////////////
// TypeTable.cpp - Creates a Type Table			                   //
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
*  - TypeTable.h
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
#include "../AbstractSyntaxTree/AbstrSynTree.h"
#include"TypeTable.h"
#include "../Parser/ActionsAndRules.h"
#include <iostream>
#include <functional>

//-----------------------Type Table Test Stub
void  TypeTable::insertintable(CodeAnalysis::ASTNode *pNode)
{
	value.first = pNode->type_;
	value.second = pNode->path_;
	string Key = pNode->name_;
	if (pNode->type_ == "class" || pNode->type_ == "struct" || pNode->type_ == "typedef" || pNode->type_ == "using" || pNode->type_ == "enum" || pNode->type_ == "namespace")
		typetable_.insert(std::make_pair(Key, value)); //create typetable
}
//--------------------------------------------------------------------------------
//Displays the type table
void TypeTable::display()
{
	cout << "\n************\nTYPE TABLE\n************" << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	cout << left << setw(30) << "NAME:";
	cout << std::left << setw(30) << "TYPE:";
	cout << std::left << setw(30) << "FILENAME:" << "\n";
	cout << "-----------------------------------------------------------------------------------------\n" << endl;

	for (auto it = typetable_.begin(); it != typetable_.end(); it++)
	{
		cout << left << setw(30) << it->first;
		cout << left << setw(30) << it->second.first << std::right << setw(35);
		cout << left << setw(30) << it->second.second << "\n";
	}
}

#ifdef TEST_TYPETABLE
#define TEST_TYPETABLE
int main()
{
	CodeAnalysis::ASTNode* pNode=new CodeAnalysis::ASTNode();
	CodeAnalysis::ASTNode* aNode = new CodeAnalysis::ASTNode();
	CodeAnalysis::ASTNode* bNode = new CodeAnalysis::ASTNode();

	pNode->name_ = "Name1";
	pNode->package_ = "Package1";
	pNode->type_ = "Type1";

	aNode->name_ = "Name2";
	aNode->package_ = "Package2";
	aNode->type_ = "Type2";

	bNode->name_ = "Name3";
	bNode->package_ = "Package3";
	bNode->type_ = "Type3";

	TypeTable tt;
	tt.insertintable(pNode);
	tt.insertintable(aNode);
	tt.insertintable(bNode);
	tt.display();
}

#endif // TEST_TYPETABLE
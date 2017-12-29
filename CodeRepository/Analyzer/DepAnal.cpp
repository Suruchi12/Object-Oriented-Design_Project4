//// DepAnal.cpp
//
/////////////////////////////////////////////////////////////////////
//DepAnal.cpp                                                      //
// ver 1.0                                                         //  
//								     							   //
// Language:    C++, Visual Studio 2015                            //
// Platform:    MacBook Pro, Windows 10(dual boot)                 //
// Application: Project #4, CSE687 - Object Oriented Design, S2017 //
// Author:      Suruchi Singh, Syracuse University                 //
//              ssingh11@syr.edu				                   //				   
/////////////////////////////////////////////////////////////////////
/*
*  Required Files:
*  ---------------
*  - TyepTable.h
*
*  Package Operations:
*  ===================
*  Traverses the path recursively using AST to create the type table
*
*  Public Interface Document:
*  ---------------------------
*   TypeAnal();
*    -calls the DFS recursively to create the type table
*   DFS
*    -traverses the path from the root directory to the file to create the type table
*
*
*  Maintanence History :
*--------------------
*  ver 1.0
*  -first release
*
*/
#include "DepAnal.h"

#ifdef TEST_DEPANAL

int main()
{
	TypeAnal ta;                          //create an object of TypeTable
	exec.typetable = ta.doTypeAnal();
}
#endif

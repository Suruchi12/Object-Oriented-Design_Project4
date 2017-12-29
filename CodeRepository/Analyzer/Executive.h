#pragma once
/////////////////////////////////////////////////////////////////////
// Executive.h - Organizes and Directs Code Analysis               //
// ver 1.0                                                        //
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
// Language:    C++, Visual Studio 2015                            //
// Platform:    MacBook Pro, Windows 10(dual boot)                 //
// Application: Project #4, CSE687 - Object Oriented Design, S2015 //
// Author:      Suruchi Singh, Syracuse University                 //
//              ssingh11@syr.edu	                               //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  ===================
*  Executive uses Parser, ScopeStack, and AbstrSynTree packages to
*  provide static code analysis for C++ and C# files contained in a
*  specified directory tree.  
*
*  It finds files for analysis using FileMgr and, for each file found,
*  uses parser rules to detect:
*  - namespace scopes
*  - class scopes
*  - struct scopes
*  - control scopes
*  - function definition scopes
*  - data declarations
*  and uses rule actions to build an Abstract Sytax Tree (AST) 
*  representing the entire code set's static structure.  Each detected
*  scope becomes a node in the AST which spans all of the files analyzed, 
*  rooted in a top-level Global Namespace scope.
*
*  Executive supports displays for:
*  - a list of all files processed with their source lines of code count.
*  - an indented representation of the AST.
*  - a list of all functions that are defined, organized by package, along
*    with their starting lines, size in lines of code, and complexity as
*    measured by the number of their descendent nodes.
*  - a list of all functions which exceed specified function size and/or
*    complexity.
*
*  Because much of the important static structure information is contained
*  in the AST, it is relatively easy to extend the application to evaluate
*  additional information, such as class relationships, dependency network,
*  and static design flaws.
*
*  Required Files:
*  ---------------
*  - Executive.h, Executive.cpp
*  - Parser.h, Parser.cpp, ActionsAndRules.h, ActionsAndRules.cpp
*  - ConfigureParser.h, ConfigureParser.cpp
*  - ScopeStack.h, ScopeStack.cpp, AbstrSynTree.h, AbstrSynTree.cpp
*  - ITokenCollection.h, SemiExp.h, SemiExp.cpp, Tokenizer.h, Tokenizer.cpp
*  - IFileMgr.h, FileMgr.h, FileMgr.cpp, FileSystem.h, FileSystem.cpp
*  - Logger.h, Logger.cpp, Utilities.h, Utilities.cpp
*
*  Maintanence History:
*  --------------------
*  
*  ver 1.0 
*  - first release
*
*/
#include <string>
#include <vector>
#include <unordered_map>
#include <iosfwd>

#include "../Parser/Parser.h"
#include "../FileMgr/FileMgr.h"
#include "../Parser/ConfigureParser.h"
#include "../Utilities/Utilities.h"
#include "TypeTable.h"
#include"../Publisher/PublishFile.h"
namespace CodeAnalysis
{
  using Utils = Utilities::StringHelper;

  ///////////////////////////////////////////////////////////////////
  // AnalFileMgr class derives from FileManager::FileMgr
  // - provides application specific handling of file and dir events

  class AnalFileMgr : public FileManager::FileMgr
  {
  public:
    using Path = std::string;
    using File = std::string;
    using Files = std::vector<File>;
    using Pattern = std::string;
    using Ext = std::string;
    using FileMap = std::unordered_map<Pattern, Files>;

    AnalFileMgr(const Path& path, FileMap& fileMap);
    virtual void file(const File& f);
    virtual void dir(const Dir& d);
    virtual void done();
    size_t numFiles();
    size_t numDirs();
  private:
    Path d_;
    FileMap& fileMap_;
    bool display_;
    size_t numFiles_;
    size_t numDirs_;
  };

  ///////////////////////////////////////////////////////////////////
  // CodeAnalysisExecutive class directs C++ and C# code analysis

  class CodeAnalysisExecutive
  {
  public:
    using Path = std::string;
    using Pattern = std::string;
    using Patterns = std::vector<Pattern>;
    using File = std::string;
    using Files = std::vector<File>;
    using Ext = std::string;
    using Options = std::vector<char>;
    using FileMap = std::unordered_map<Pattern, Files>;
    using FileNodes = std::vector<std::pair<File, ASTNode*>>;
    using Slocs = size_t;
    using SlocMap = std::unordered_map<File, Slocs>;

    CodeAnalysisExecutive();
    virtual ~CodeAnalysisExecutive();

    CodeAnalysisExecutive(const CodeAnalysisExecutive&) = delete;
    CodeAnalysisExecutive& operator=(const CodeAnalysisExecutive&) = delete;

    void showCommandLineArguments(int argc, char* argv[]);
    bool ProcessCommandLine(int argc, char* argv[]);
    FileMap& getFileMap();
    std::string getAnalysisPath();
    virtual void getSourceFiles();
    virtual void processSourceCode(bool showActivity);
    void complexityAnalysis();
    std::vector<File>& cppHeaderFiles();
    std::vector<File>& cppImplemFiles();
    std::vector<File>& csharpFiles();
    Slocs fileSLOCs(const File& file);
    size_t numFiles();
    size_t numDirs();
    std::string systemTime();
    virtual void displayMetrics(ASTNode* root);
    virtual void displayMetrics();
    virtual void displayMetricSummary(size_t sMax, size_t cMax);
    virtual void displaySlocs();
    virtual void displayAST();
    virtual void dispatchOptionalDisplays();
    virtual void setDisplayModes();
    void startLogger(std::ostream& out);
    void flushLogger();
    void stopLogger();
    void setLogFile(const File& file);
	
	void req8();
	int myExec(int argc, char* argv[]);
	std::unordered_map<std::string, std::set<std::string>> temp;
	unordered_map<std::string, std::pair<std::string, std::string>> typetable;

  private:
    void setLanguage(const File& file);
    void showActivity(const File& file);
    void clearActivity();
    virtual void displayHeader();
    virtual void displayMetricsLine(const File& file, ASTNode* pNode);
    virtual void displayDataLines(ASTNode* pNode, bool isSummary = false);
    std::string showData(const Scanner::ITokCollection* ptc);
    Parser* pParser_;
    ConfigParseForCodeAnal configure_;
    Repository* pRepo_;
    Path path_;
    Patterns patterns_;
    Options options_;
    FileMap fileMap_;
    FileNodes fileNodes_;
    std::vector<File> cppHeaderFiles_;
    std::vector<File> cppImplemFiles_;
    std::vector<File> csharpFiles_;
    size_t numFiles_;
    size_t numDirs_;
    SlocMap slocMap_;
    bool displayProc_ = false;
    std::ofstream* pLogStrm_ = nullptr;
  };
}

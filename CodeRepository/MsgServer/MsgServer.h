#pragma once
/////////////////////////////////////////////////////////////////////////
// MsgServer.h - Demonstrates simple one-way HTTP style messaging    //
//                 and file transfer                                   //
//                                                                     //
// Platform:    MacBook Pro, Windows 10(dual boot)                     //
// Application: Project #4, CSE687 - Object Oriented Design, S2017     //
// Author:      Suruchi Singh, Syracuse University                     //
//              ssingh11@syr.edu				                       //
/////////////////////////////////////////////////////////////////////////
/* Package Operations :
*  == == == == == == == == == =
* This package implements a server that receives HTTP style messages and
* files from multiple concurrent clients and simply displays the messages
* and stores files.
*
* It's purpose is to provide a very simple illustration of how to use
* the Socket Package provided for Project #4.
*/
/*
* Required Files:
* 
*   HttpMessage.h, HttpMessage.cpp
*   Cpp11-BlockingQueue.h
*   Sockets.h, Sockets.cpp
*   FileSystem.h, FileSystem.cpp
*   Logger.h, Logger.cpp
*   Utilities.h, Utilities.cpp
*   Analyzer/Executive.h
*
*  Public Interface Document:
*  ---------------------------
*   void execute(const size_t TimeBetweenMessages, const size_t NumMessages);
*	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
*	void sendMessage(HttpMessage& msg, Socket& socket);
*	bool sendFile(const std::string& fqname, Socket& socket);
*	void operator()(Socket socket);
*	void argfunc(int argc, char* argv[]);
*	void Download();
*	void ServerPublish();
*
*  Maintanence History :
*--------------------
*  ver 1.0
*  -first release
*/

#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../Analyzer/Executive.h"


#include <string>
#include <iostream>
#include <direct.h>

using namespace Logging;
using namespace Async;
using namespace CodeAnalysis;
using Show = StaticLogger<1>;
using namespace Utilities;

/////////////////////////////////////////////////////////////////////
// ClientHandler class
/////////////////////////////////////////////////////////////////////
// - instances of this class are passed by reference to a SocketListener
// - when the listener returns from Accept with a socket it creates an 
//   instance of this class to manage communication with the client.

//Reveives from the client
class ClientHandler
{
public:
	ClientHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);
	void argfunc();
	void Download();
	void ServerPublish();
private:
	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);  //HttpMessage is the return type
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	BlockingQueue<HttpMessage>& msgQ_;

	std::unordered_map<std::string, std::set<std::string>> temp;
	unordered_map<std::string, std::pair<std::string, std::string>> typetable;
	int newargc = 0;
	char *newargv[1000];

};

/*
***************************
SENDS TO THE CLIENT
***************************
*/

class ServerHandler
{
public:
	using EndPoint = std::string;
	void execute(size_t TimeBetweenMessages, const size_t NumMessages);
private:
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const std::string& fqname, Socket& socket);
};






#pragma once
#pragma warning (disable : 4367)
/////////////////////////////////////////////////////////////////////////
// MsgServer.cpp - Demonstrates simple HTTP style messaging            //
//                 and file transfer                                   //
//                                                                     //
// Platform:    MacBook Pro, Windows 10(dual boot)                     //
// Application: Project #4, CSE687 - Object Oriented Design, S2017     //
// Author:      Suruchi Singh, Syracuse University                     //
//              ssingh11@syr.edu				                       //
/////////////////////////////////////////////////////////////////////////
/* Package Operations :
* ===================
*  Performs all operations sent from the Client like Upload, download and publish.
* This package implements a server that receives HTTP style messages and
* files from multiple concurrent clients and simply displays the messages
* and stores files.
*
* It's purpose is to provide a very simple illustration of how to use
* the Socket Package provided for Project #4.
*
*
* Required Files:
* ------------------------------
*   MsgClient.cpp, MsgServer.cpp
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


#include"MsgServer.h"

#include <string>
#include <iostream>
#include <direct.h>

//client wants to download the files 
void ClientHandler::Download()
{
	Show::title("\nDownload Files\n");
	Show::title("Server is sending files to the client");
	ServerHandler c1;
	std::thread t1(
		[&]() { c1.execute(100, 1); } // 20 messages 100 millisec apart
	);
	t1.join();
}
//client wants to publish the files
void ClientHandler::ServerPublish()
{
	Show::title("\nPublish the Files\n");
	CodeAnalysisExecutive Cae;
	int a = Cae.myExec(newargc, newargv);
}

//read messages received
HttpMessage ClientHandler::readMessage(Socket& socket){
   connectionClosed_ = false; HttpMessage msg;
  while (true){  std::string attribString = socket.recvString('\n'); 
    if (attribString.size() > 1){
      HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);  //trim white spaces using colon
      msg.addAttribute(attrib);  }
    else     break;}
  if (msg.attributes().size() == 0) {
    connectionClosed_ = true;   //close the connection
    return msg;
  }
  if (msg.attributes()[0].first == "POST"){  	  
	  std::string filename = msg.findValue("file");
	  if (msg.attributes()[0].second == "UPLOAD"){//upload to the server
		   if (filename != ""){
			  size_t contentSize;
			  std::string sizeString = msg.findValue("content-length");
			  if (sizeString != "")
				  contentSize = Converter<size_t>::toValue(sizeString);  //converting the string back to template
			  else
				  return msg;
			  readFile(filename, contentSize, socket);
		  }
		   if (filename != "") {
			   msg.removeAttribute("content-length");
			   std::string bodyString = "<file>" + filename + "</file>";
			   std::string sizeString = Converter<size_t>::toString(bodyString.size());
			   msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
			   msg.addBody(bodyString);
		   }
	  }
	  else if (msg.attributes()[0].second == "PUBLISH") {ServerPublish();}   //publish functionality called 
	  else if (msg.attributes()[0].second == "DOWNLOAD") {Download();}    //download function called
	  else if (msg.attributes()[0].second == "QUIT"){ Show::write("\n\n  Quitting");}  //quit called
	  if (filename == ""){
		  size_t numBytes = 0;
		  size_t pos = msg.findAttribute("content-length");
		  if (pos < msg.attributes().size()){
			  numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
			  Socket::byte* buffer = new Socket::byte[numBytes + 1];
			  socket.recv(numBytes, buffer);
			  buffer[numBytes] = '\0';
			  std::string msgBody(buffer);
			  msg.addBody(msgBody);
			  delete[] buffer;
		  }
	  }
  } return msg;
}
//----< read a binary file from socket and save >--------------------
/*
 * This function gets a file message, 
 * and when this function is running, continuosly send bytes until
 * fileSize bytes have been sent.
 */
bool ClientHandler::readFile(const std::string& filename, size_t fileSize, Socket& socket)
{
  std::string fqname = "../ServersRepository/" + filename;   //files are stored in ServersRepository
  FileSystem::File file(fqname);
  file.open(FileSystem::File::out, FileSystem::File::binary);
  if (!file.isGood())
  {
    Show::write("\n\n  can't open file " + fqname);
    return false;
  }
  const size_t BlockSize = 2048;
  Socket::byte buffer[BlockSize];
  size_t bytesToRead;
  while (true)
  {
    if (fileSize > BlockSize)
      bytesToRead = BlockSize;
    else
      bytesToRead = fileSize;
    socket.recv(bytesToRead, buffer);
    FileSystem::Block blk;
    for (size_t i = 0; i < bytesToRead; ++i)
      blk.push_back(buffer[i]);

    file.putBlock(blk);
    if (fileSize < BlockSize)
      break;
    fileSize -= BlockSize;
  }
  file.close();
  return true;
}
//----< receiver functionality is defined by this function >---------

void ClientHandler::operator()(Socket socket)
{
  while (true)
  {
    HttpMessage msg = readMessage(socket);
    if (connectionClosed_ || msg.bodyString() == "quit")
    {
      Show::write("\n\n  clienthandler thread is terminating");
      break;
    }
    msgQ_.enQ(msg);
  }
}


void ClientHandler::argfunc()
{
	for (int i = 0; i<100; i++)
		newargv[i] = { 0 };
	newargc = 5;
	newargv[1] = "..\\ServersRepository";
	newargv[2] = "*.h";
	newargv[3] = "*.cpp";
	newargv[4] = "..\\ServersRepository\\startfile.htm";

}


class ClientCounter
{
public:
	ClientCounter()
	{
		++clientCount;
	}
	size_t count()
	{
		return clientCount;
	}
private:
	static size_t clientCount;
};

size_t ClientCounter::clientCount = 0;
//----< factory for creating messages >------------------------------
HttpMessage ServerHandler::makeMessage(size_t n, const std::string& body, const EndPoint& ep)
{
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	EndPoint myEndPoint = "localhost:8080";  // ToDo: make this a member of the sender
											 // given to its constructor.
	switch (n)
	{
	case 1:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("GET", "DOWNLOAD"));
		break;
	case 2:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("GET", "QUIT"));
		break;
	default:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
		break;
	}

	msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
	msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
	msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));
	msg.addBody(body);    //pushes back bytes----> <msg>Message # from client #
	if (body.size() > 0)
	{
		attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
		msg.addAttribute(attrib);
	}

	return msg;
}
//----< send message using socket >----------------------------------

void ServerHandler::sendMessage(HttpMessage& msg, Socket& socket)
{
	std::string msgString = msg.toString();
	socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}
//----< send file using socket >-------------------------------------
/*
* - Sends a message to tell receiver a file is coming.
* - Then sends a stream of bytes until the entire file
*   has been sent.
* - Sends in binary mode which works for either text or binary.
*/
bool ServerHandler::sendFile(const std::string& filename, Socket& socket)
{
	// assumes that socket is connected
	std::string fqname = "../ServersRepository/" + filename;
	FileSystem::FileInfo fi(fqname);  //if the file is good
	size_t fileSize = fi.size();     //size of this particular file
	std::string sizeString = Converter<size_t>::toString(fileSize); //convering that size into a string
	FileSystem::File file(fqname);   //returns features of the file, if it is good or not
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
		return false;

	HttpMessage msg = makeMessage(1, "", "localhost::8081");
	msg.addAttribute(HttpMessage::Attribute("file", filename));
	msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
	sendMessage(msg, socket);    //sends file contents byte by byte into the message
	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];
	while (true)
	{
		FileSystem::Block blk = file.getBlock(BlockSize); //reads a blok of file and puts it in blk
		if (blk.size() == 0)
			break;
		for (size_t i = 0; i < blk.size(); ++i)
			buffer[i] = blk[i];                      //put each block into the buffer
		socket.send(blk.size(), buffer); //send this buffer and block byte by byte  this returns true or false
		if (!file.isGood())
			break;
	}
	file.close();
	return true;
}
//----< this defines the behavior of the sender >--------------------

void ServerHandler::execute(const size_t TimeBetweenMessages, const size_t NumMessages){
	try{
		SocketSystem ss;      //Sockets.h
		SocketConnecter si;  //Sockets.h
		while (!si.connect("localhost", 8081)){
			Show::write("\n Server waiting to connect");   //put it into the queue
			::Sleep(100);
		}
		HttpMessage msg;
			std::string msgBody =
				"<msg>Message #" + Converter<size_t>::toString(1) +
				" from server #" + "</msg>";
			msg = makeMessage(1, msgBody, "localhost:8081"); //MsgClient.cpp
			sendMessage(msg, si);   //send byte by byte
			Show::write("\n\n  Server received message to download files\n");  //enqueue the message
			Show::write("\n\n  Server sent\n" + msg.toIndentedString());  //enqueue the message
			::Sleep(TimeBetweenMessages);
		//  send all *.htm, *css and .js files from TestFiles folder
		std::vector<std::string> files = FileSystem::Directory::getFiles("../ServersRepository", "*.htm*");
		std::vector<std::string>filescss = FileSystem::Directory::getFiles("../ServersRepository", "*.css");
		std::vector<std::string>filesjs = FileSystem::Directory::getFiles("../ServersRepositoarry", "*.js");
		for (size_t i = 0; i < files.size(); ++i){
			Show::write("\n\n  sending file " + files[i]);  
			sendFile(files[i], si);
		}
		for (size_t i = 0; i < filescss.size(); ++i) {
			Show::write("\n\n  sending file " + filescss[i]); 
			sendFile(filescss[i], si);
		}
		for (size_t i = 0; i < filesjs.size(); ++i) {
			Show::write("\n\n  sending file " + filesjs[i]);  
			sendFile(filesjs[i], si);
		}
		msg = makeMessage(2, "quit", "toAddr:localhost:8081");  //quits 
		sendMessage(msg, si);
		Show::write("\n\n  Server sent\n" + msg.toIndentedString() + "\n" + "\n  All done folks");
	}
	catch (std::exception& exc){
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}



//Server's MAIN
int main()
{
  ::SetConsoleTitle(L"HttpMessage Server - Runs Forever");
  std::string dirName = "../ServersRepository";
  _mkdir(dirName.c_str());
  Show::attach(&std::cout);     // Show->StaticLogger->Logger.h
  Show::start();
  Show::title("\n  HttpMessage Server started");
  BlockingQueue<HttpMessage> msgQ;
  try
  {
    SocketSystem ss;
    SocketListener sl(8080, Socket::IP6);
    ClientHandler cp(msgQ); 
	cp.argfunc();
    sl.start(cp);

    while (true)
    {
      HttpMessage msg = msgQ.deQ();    //BlockingQueue.h
      Show::write("\n\n  server recvd message with body contents:\n" + msg.bodyString());
    }
  }
  catch (std::exception& exc)
  {
    Show::write("\n  Exeception caught: ");
    std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
    Show::write(exMsg);
  }
}

\
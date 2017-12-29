#pragma once
#pragma warning (disable : 4367)
/////////////////////////////////////////////////////////////////////////
// MsgClient.cpp - Demonstrates simple HTTP messaging from Client      //
//                 to Server										   //	
//                                                                     //
// Platform:    MacBook Pro, Windows 10(dual boot)                     //
// Application: Project #4, CSE687 - Object Oriented Design, S2017     //
// Author:      Suruchi Singh, Syracuse University                     //
//              ssingh11@syr.edu				                       //
/////////////////////////////////////////////////////////////////////////

/*  Package Operations :
*  ===================
* This package implements a client that sends HTTP style messages and
* files to a server that displays messages and stores files.
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
*
*  Public Interface Document:
*  ---------------------------
*   ClientCounter to keep count of the number of clients
*   void execute(const size_t TimeBetweenMessages, const size_t NumMessages);
*	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
*	void sendMessage(HttpMessage& msg, Socket& socket);
*	bool sendFile(const std::string& fqname, Socket& socket);
*	void operator()(Socket socket); to listen to the server
*
*  Maintanence History :
*--------------------
*  ver 1.0
*  -first release
*/

#include "MsgClient.h"

/////////////////////////////////////////////////////////////////////
// ClientCounter creates a sequential number for each client
//
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

/////////////////////////////////////////////////////////////////////
// MsgClient class
// - was created as a class so more than one instance could be 
//   run on child thread
//

//----< factory for creating messages >------------------------------
/*
 * - EndPoints are strings of the form ip:port, e.g., localhost:8081. This argument
 *   expects the receiver EndPoint for the toAddr attribute.
 */
HttpMessage MsgClient::makeMessage(size_t n, const std::string& body, const EndPoint& ep)
{
  HttpMessage msg;
  HttpMessage::Attribute attrib;
  EndPoint myEndPoint = "localhost:8081";  // commands to upload,publish,downlaod and quit                                     
  switch (n)
  {
  case 1:
    msg.clear();
    msg.addAttribute(HttpMessage::attribute("POST", "UPLOAD"));
    break;
  case 2:
	  msg.clear();
	  msg.addAttribute(HttpMessage::attribute("POST", "PUBLISH"));
	  break;
  case 3:
	  msg.clear();
	  msg.addAttribute(HttpMessage::attribute("POST", "DOWNLOAD"));
	  break;
 case 4:
	 msg.clear();
	 msg.addAttribute(HttpMessage::attribute("POST", "QUIT"));
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
  if (body.size() > 0){
	  attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
	  msg.addAttribute(attrib);
  }
  return msg;
}
//----< send message using socket >----------------------------------

void MsgClient::sendMessage(HttpMessage& msg, Socket& socket)
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
bool MsgClient::sendFile(const std::string& filename, Socket& socket)
{
  std::string fqname = "../TestFiles/" + filename;
  FileSystem::FileInfo fi(fqname);  //if the file is good
  size_t fileSize = fi.size();     //size of this particular file
  std::string sizeString = Converter<size_t>::toString(fileSize); //convering that size into a string
  FileSystem::File file(fqname);   //returns features of the file, if it is good or not
  file.open(FileSystem::File::in, FileSystem::File::binary);
  if (!file.isGood())
    return false;
  
  HttpMessage msg = makeMessage(1, "", "localhost::8080");
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
//----< this defines the behavior of the client >--------------------

void MsgClient::execute(const size_t TimeBetweenMessages, const size_t NumMessages){
  ClientCounter counter;     
  size_t myCount = counter.count();   //gives myCount an incremented counter
  std::string myCountString = Utilities::Converter<size_t>::toString(myCount);   //converts count into the string
  Show::attach(&std::cout);       //Loger.h from Static.h  gives the cout thing to Logger.cpp
  Show::start();                  //starts thread 
  Show::title(
    "Starting HttpMessage client" + myCountString + 
    " on thread " + Utilities::Converter<std::thread::id>::toString(std::this_thread::get_id()));//get the thread id
  try{
    SocketSystem ss;      //Sockets.h
    SocketConnecter si;  //Sockets.h
    while (!si.connect("localhost", 8080)){
      Show::write("\n client waiting to connect");   //put it into the queue
      ::Sleep(100);
    } HttpMessage msg;
	for (size_t i = 1; i <= NumMessages; i++){		std::string msgBody =
			"<msg>Message #" + Converter<size_t>::toString(i) +
			" from client #" + myCountString + "</msg>";
		msg = makeMessage(i, msgBody, "localhost:8080"); //MsgClient.cpp
		sendMessage(msg, si);   //send byte by byte
		Show::write("\n\n  client" + myCountString + " sent\n" + msg.toIndentedString());  //enqueue the message
		::Sleep(TimeBetweenMessages);
		if (msg.attributes()[0].second == "UPLOAD"){
			std::vector<std::string> files = FileSystem::Directory::getFiles("../TestFiles", "*.*");
			for (size_t i = 0; i < files.size(); ++i){
				Show::write("\n\n  sending file " + files[i]);  //enqueue the filenames
				sendFile(files[i], si);
			}
		}
	}
		msg = makeMessage(4, "QUIT", "toAddr:localhost:8080");
		sendMessage(msg, si);
		Show::write("\n\n  client" + myCountString + " sent\n" + msg.toIndentedString());

		Show::write("\n");
		Show::write("\n  All done folks");
  }
  catch (std::exception& exc)
  {
    Show::write("\n  Exeception caught: ");
    std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
    Show::write(exMsg);
  }
}

//----< this defines processing to frame messages >------------------

HttpMessage ClientReceives::readMessage(Socket& socket){
	connectionClosed_ = false;  HttpMessage msg;
	while (true){
		std::string attribString = socket.recvString('\n'); 
		if (attribString.size() > 1){
			HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);  //trim white spaces using colon
			msg.addAttribute(attrib);  //we are getting these messages from the socket
		}
		else { break; }
	}
	if (msg.attributes().size() == 0){
		connectionClosed_ = true;   //close the connection
		return msg;
	}
	if (msg.attributes()[0].first == "GET"){  	 
		std::string filename = msg.findValue("file");
		if (msg.attributes()[0].second == "DOWNLOAD"){   //command for download
			if (filename != ""){
				size_t contentSize; std::string sizeString = msg.findValue("content-length");
				if (sizeString != "")
					contentSize = Converter<size_t>::toValue(sizeString);  //converting the string back to template
				else
					return msg;
				readFile(filename, contentSize, socket);
			}
			if (filename != ""){
				msg.removeAttribute("content-length");
				std::string bodyString = "<file>" + filename + "</file>";
				std::string sizeString = Converter<size_t>::toString(bodyString.size());
				msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
				msg.addBody(bodyString);
			}
		}
		else if (msg.attributes()[0].second == "QUIT")  {Show::write("\n\n  Quitting");	}
		if(filename == ""){
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
	}
	return msg;
}

/*
* This function will read the file and keep it in the Server's Repository after getiing send a file message,
* and when this function is running, continuosly send bytes until
* fileSize bytes have been sent.
*/
bool ClientReceives::readFile(const std::string& filename, size_t fileSize, Socket& socket)
{
	std::string dirName = "../TestFiles/ClientsRepository";
	_mkdir(dirName.c_str());
	std::string fqname = "../TestFiles/ClientsRepository/" + filename;
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
void ClientReceives::operator()(Socket socket)
{
	while (true)
	{
		HttpMessage msg = readMessage(socket);
		if (connectionClosed_ || msg.bodyString() == "quit")
		{
			Show::write("\n\n  Terminating");
			break;
		}
		msgQ_.enQ(msg);
	}
}

//Get command line arguments
void ClientReceives::argfunc(int argc, char* argv[])
{
	for (int i = 0; i<100; i++)
		newargv[i] = { 0 };

	newargc = argc;

	for (int i = 0; i<argc; i++)//the files[0] is the name of the command
		newargv[i] = argv[i];
}



//Client's main
int main()
{
  ::SetConsoleTitle(L"Clients Running on Threads");
  Show::title("Demonstrating two HttpMessage Clients each running on a child thread");
  MsgClient c1;  //Client 1
  std::thread t1(
    [&]() { c1.execute(100, 4); } 
  );
  MsgClient c2; //Client 2
  std::thread t2(
    [&]() { c2.execute(120, 4); } 
  );
  t1.join();
  t2.join();
//Client Listens to get files from the Server
  ::SetConsoleTitle(L"HttpMessage Server - Runs Forever");
  BlockingQueue<HttpMessage> msgQ;
  try
  {
	  SocketSystem ss;
	  SocketListener sl(8081, Socket::IP6);      //client listens to the server
	  ClientReceives cp(msgQ);
	  sl.start(cp);
	  while (true)
	  {
		  HttpMessage msg;
		  msg= msgQ.deQ();    //BlockingQueue.h
		  Show::write("\n\n  Client received message with body contents:\n" + msg.bodyString());
	  }
  }
  catch (std::exception& exc)
  {
	  Show::write("\n  Exeception caught: ");
	  std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
	  Show::write(exMsg);
  }
}


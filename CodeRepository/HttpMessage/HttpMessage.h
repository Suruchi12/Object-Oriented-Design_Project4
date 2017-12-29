#pragma once
/////////////////////////////////////////////////////////////////////
// HttpMessage.h - Build and parse Http style messages             //
//                                                                 //
// Platform:    MacBook Pro, Windows 10(dual boot)                     //
// Application: Project #4, CSE687 - Object Oriented Design, S2017     //
// Author:      Suruchi Singh, Syracuse University                     //
//              ssingh11@syr.edu				                       //
/////////////////////////////////////////////////////////////////////
/* Package Operations :
*== == == == == == == == == =
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
*   None
*
*  Public Interface Document:
*  ---------------------------
*   MockSocket(const HttpMessage& msg) : msgString_(msg.toString()) {};
*  std::string recvString();
*  bool recv(size_t bytes, byte* buffer);
*  size_t& setContent_length() { return content_length_; }
*
*  Maintanence History :
*--------------------
*  ver 1.0
*  -first release

*/

#include <vector>
#include <string>
#include <array>

class HttpMessage
{
public:
  using byte = char;
  using Name = std::string;
  using Value = std::string;
  using Attribute = std::pair<Name, Value>;
  using Attributes = std::vector<Attribute>;
  using Terminator = std::string;
  using Body = std::vector<byte>;

  // message attributes
  void addAttribute(const Attribute& attrib);
  Value findValue(const Name& name);
  size_t findAttribute(const Name& name);
  bool removeAttribute(const Name& name);
  Attributes& attributes();
  static std::string attribString(const Attribute& attrib);
  static Attribute attribute(const Name& name, const Value& value);
  static Attribute parseAttribute(const std::string& src);

  // message body
  void setBody(byte buffer[], size_t Buflen);
  size_t getBody(byte buffer[], size_t& Buflen);
  void addBody(const Body& body);
  void addBody(const std::string& body);
  void addBody(size_t numBytes, byte* pBuffer);
  Body& body();
  size_t bodyLength();

  // construct message
  //static HttpMessage parseHeader(const std::string& src);
  //static HttpMessage parseMessage(const std::string& src);

  // display
  std::string headerString() const;
  std::string bodyString() const;
  std::string toString() const;
  std::string toIndentedHeaderString() const;
  std::string toIndentedBodyString() const;
  std::string toIndentedString() const;

  // cleanup
  void clear();
  static void fillBuffer(byte buffer[], size_t BufSize, byte fill = '\0');

private:
  Attributes attributes_;
  Terminator term_ = "\n";
  Body body_;
};
#ifndef MESSAGE_H
#define MESSAGE_H
////////////////////////////////////////////////////////////////////
// Message.h -    capsulate Message class which contains a Header //
//				  class
// Version 1.0													  //
//																  //
// Platform:    Surface Pro 3 , Win 8.1 Pro, Visual Studio 2013   //
// Author:      Zhenya Li						                  //
//              zli52@syr.edu	                                  //
////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
*
* -This package defines Message class, and Header class.
	-a Header class contains a command and pairs of attribute
*   - Message class contains a Header and body. body is for storing string Message ie. XML files.
*
*  Public Interface:
* -----------------
class Header:
* -static bool isHeader(const std::string& str);
*	check if a string is a header
*	
*  -bool stringToHeader(const std::string& str);
*  transfer(parse) a string to header
*	
*  -bool addAttribute(const std::string& name,const std::string& value);
*	add an attribute pair to header by a string name and a string value	
*
*   -std::vector<std::pair<std::string, std::string>>& getAttributes();   
*	return all the attributes vector
*	
*	-std::string toString();    
*	translate the header to a single string
*	
*	-std::string &getCommand();
*	bool setCommand(const std::string &com);

class Message:
*
*	-bool stringToMessage(const std::string& str);  
*	parsing a string to this message
*
*	-bool addCommand(const std::string& com);
*	bool addAttrib(const std::string&name, const std::string& value);	
*
*	-Header& getHeader();				
*	return header
*	
*	-std::string& getBody();				
*	return body
*	
*	-void setBody(const std::string& body_content); 
*	set body content
*	
*	-std::string toString();	
*	transfer a message to string
*
*
* Required Files:
* ---------------
*	- AppHelpers.h, AppHelpers.cpp
*
* Build Process:
* --------------
*   devenv hw3.sln /debug rebuild
*
*
*
*/
#include <string>
#include <vector>




//************************************************************
//****************Header class declaration********************

class Header{
/*header format is:
	command:COMMAND
	attrib1=value1
	attrib2=value2
	...
	\n
*/
public:
	Header();
	Header(const Header&);  //copy ctor
	Header(const std::string &header_string);  //ctor need to parse a full header string
	~Header();
	
	static bool isHeader(const std::string& str);//check if a string is a header
	bool stringToHeader(const std::string& str);//transfer a string to header
	bool addAttribute(const std::string& name,const std::string& value);

	std::vector<std::pair<std::string, std::string>>& getAttributes();   //return all the attributes vector
	std::string toString();    //translate the header to a single string
	std::string &getCommand();
	bool setCommand(const std::string &com);   //set the command

private:
	std::string command;
	std::vector<std::pair<std::string, std::string>> attributes;  //the first attribute is the body size
};

inline std::vector<std::pair<std::string, std::string>>& Header::getAttributes(){ return attributes; }


//************************************************************
//****************Message class declaration*******************

class Message{

public:
	Message(const Message &m);			//copy ctor
	Message(const std::string& str);    //ctor with a string 
	Message();
	~Message();

	bool stringToMessage(const std::string& str);  //parsing a string to this message
	static std::pair<std::string, std::string>getAttributeFromString(const std::string&);
	bool addCommand(const std::string& com);
	bool addAttrib(const std::string&name, const std::string& value);	

	Header& getHeader();				//return header
	std::string& getBody();				//return body
	void setBody(const std::string& body_content); //set body content
	std::string toString();				//convert this message into string

private:
	Header header;						//header of the message
	std::string body;					//store the xml string
};

inline Header& Message::getHeader(){ return header; }
inline std::string& Message::getBody(){ return body; }
inline void Message::setBody(const std::string & body_content){ body = body_content; }


#endif
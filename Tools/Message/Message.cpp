////////////////////////////////////////////////////////////////////
// Message.cpp -    Definition of Message class which contains a  //
//					Header class                                  //
// Version 1.0													  //
//																  //
// Platform:    Surface Pro 3 , Win 8.1 Pro, Visual Studio 2013   //
// Author:      Zhenya Li						                  //
//              zli52@syr.edu	                                  //
////////////////////////////////////////////////////////////////////
#include <iostream>
#include "Message.h"
#include "../FileSystemDemo/FileSystem.h"

typedef std::pair<std::string, std::string> p_att;

void show(const std::string& s);//helper function for tests


//************************************************************
//*****************Header class definition********************
Header::~Header(){}
Header::Header(){}
//copy ctor
Header::Header(const Header& h):command(h.command),attributes(h.attributes){
}
//----ctor parse the header string

Header::Header(const std::string &str){
	if (!isHeader(str))command = "NO COMMAND";
	else stringToHeader(str);
}
//-----check if a string is a header,a header must have a command

bool Header::isHeader(const std::string& str){
	if (str.empty())return false;
	size_t foundpos = str.find_first_of(':');
	if (str.substr(0, foundpos) == "command")return true;
	else return false;
}
//-----transfer a string to header

bool Header::stringToHeader(const std::string& str){
	if (!isHeader(str))return false;
	std::string stringleft = str;
	//get title
	size_t foundpos = stringleft.find_first_of('\n');
	std::string commandstring = stringleft.substr(0, foundpos);
	size_t signpos = commandstring.find(":");
	command = commandstring.substr(signpos + 1);//set command
	
	stringleft = stringleft.substr(foundpos + 1);
	foundpos = stringleft.find_first_of('\n');
	std::string att_line = stringleft.substr(0, foundpos);//get one line of attribute
	while (att_line.size()>0){
		signpos = att_line.find("=");
		addAttribute(att_line.substr(0, signpos), att_line.substr(signpos + 1));
		stringleft = stringleft.substr(foundpos + 1);
		foundpos = stringleft.find_first_of('\n');
		att_line = stringleft.substr(0, foundpos);
	}
	return true;
}


//-----add an attribute with a string name and a string value

bool Header::addAttribute(const std::string & name, const std::string & value){
	if (name.empty())
		return false;
	attributes.push_back(std::pair<std::string, std::string>(name, value));
	return true;
}
//-----get command in string type

std::string& Header::getCommand(){ 
	return command;
}
//-----set command, if the command is still empty or not changed return false

bool Header::setCommand(const std::string &com){ 
	std::string oldCommand=command;
	command = com;
	if (command.empty() || command.compare(oldCommand) == 0)
		return false;
	else return true;
}


//-----transfer a header to string

std::string Header::toString(){
	std::string outputString;
	outputString += ("command:"+getCommand() + "\n");
	std::string attLine;
	for (auto att : getAttributes()){
		attLine = att.first + "=" + att.second + "\n";
		outputString += attLine;
	}
	outputString += "\n";
	return outputString;
}

//************************************************************
//****************Message class definition********************
//----default ctor
Message::Message(){}
//----default dtor
Message::~Message(){}
//----copy ctor

Message::Message(const Message& m):header(m.header),body(m.body){}
//----ctor receiving a string

Message::Message(const std::string & str){
	stringToMessage(str);
}
//----parse string into a message

bool Message::stringToMessage(const std::string& str){

	if (!Header::isHeader(str)){
		header.setCommand("NO COMMAND");
		return false;
	}
	size_t foundpos = str.find("\n\n");
	std::string header_string = str.substr(0, foundpos+2);
	std::string stringleft = str.substr(foundpos + 2);
	header.stringToHeader(header_string);
	if (stringleft.size() > 0)
		setBody(stringleft);
	return true;
}

//-----if a tring has a format of "string1=string2",return a pair of two strings as an attribute

p_att Message::getAttributeFromString(const std::string& string){
	p_att pa;
	size_t pos = string.find("=");
	if (pos == std::string::npos)return pa;
	else{
		pa.first = string.substr(0, pos);
		pa.second = string.substr(pos + 1);
	}
	return pa;
}

//-----add command to message header, a message header cannot be set twice

bool Message::addCommand(const std::string& com){
	if (!header.getCommand().empty())
		return false;
	else return header.setCommand(com);
}

//-----add attribute to the header

bool Message::addAttrib(const std::string&name, const std::string& value){
	return header.addAttribute(name, value);
}
//----convert this message to string

std::string Message::toString(){
	std::string outstring = header.toString();
	if (body.size() > 0)
		outstring += body;
	return outstring;
}

//----test stub

#ifdef MESSAGE_MAIN

void title(const std::string& s,const char& c='='){
	std::cout << "\n";
	for (size_t i = 0; i < s.size(); ++i)
		std::cout <<c;
	std::cout << "\n"<<s<<"\n";
	for (size_t i = 0; i < s.size(); ++i)
		std::cout <<c;
	std::cout << "\n";
}

void show(const std::string& s){
	std::cout << s << "\n";
}


int main(){
	
	title("testing Message constructor with a string");
	std::string string1="command:COMMAND1\nAtt1=value1\nAtt2=value2\n\nthis is a test body\nsecond line of body";
	//std::string string2="command:COMMAND2\nAtt1=value1\nAtt2=value2\n\n";
	
	Message msg1(string1);
		//,msg2(string2);
	show(msg1.toString());
	//show(msg2.toString());
	title("testing Message copy constructor");
	Message msg3(msg1);
	show(msg3.toString());

	title("testing getAttributeFromString");
	p_att p1=Message::getAttributeFromString("aaa=1212d");
	show(p1.first);
	show(p1.second);
	
	
	return 0;
}

#endif
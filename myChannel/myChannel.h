#ifndef MYCHANNEL_H
#define MYCHANNEL_H
/////////////////////////////////////////////////////////////////////////////
// myChannel.h - for CSE687 Project #4, Spring 2015                        //
// - build as DLL used for WPF interface								   //
// - myChannel reads from sendQ and writes to recvQ                        //
//	
//	ver 1.0
//	Surface pro 3, Windows 8.1											   //
//  Zhenya Li												               //
/////////////////////////////////////////////////////////////////////////////

/*
*  Package Operations:
*  -------------------
*  This package is used to output myChannel.dll and myChannel.lib for WPF GUI to use.
   
   dll output methods:
	ISendr* createSendr();
	IRecvr* createRecvr(size_t port);

	Public Interface:
	-------------------
   -ISendr:

	virtual bool connetToServer(const std::string& ip, size_t port)=0;   //connect
		connect to server with ip address and port.
	virtual bool sendMessage()=0;
		send a message from sending queue
	virtual bool uploadFile(const Message& msg) = 0;
		upload file to connected server
	virtual bool shutDownSend() = 0;
		shut down sending
	virtual void enQueue(const Message &msg)=0;
		enqueue a message into sending queue
	virtual Message deQueue()=0;
		dequeue a message from sending queue
	virtual size_t queueSize()=0;
		queue size

	-IRecvr:
	
	virtual bool start() = 0;
		start listening
	virtual std::string receiveString(char terminator) = 0;
		receive string from sender
	virtual bool receiveBuffer(size_t bytes, char* buffer) = 0;
		receive buffer from sender
	virtual void enQueue(const Message& s)=0;   
		enQue a message when a task is received
	virtual Message deQueue()=0;	
		deQue a message
	virtual size_t queueSize()=0;
		queue size

*  Required Files:
*  ---------------
*  myChannel.h, myChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
	Message.h, Message.cpp
	Sockets.h, Sockets.cpp
	FileSystem.h, FileSystem.cpp
	AppHelpers.h, AppHelpers.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
*  Maintenance History:
*  --------------------
* 
*  ver 1.0 : 22 Apr 2015
*  - complete with sender and receiver used for WPF
*/









#ifdef MYCHANNEL_DLL
#define DLL_OUT __declspec(dllexport)
#else
#define DLL_OUT __declspec(dllimport)
#endif


#include "../Tools/Message/Message.h"
#include <string>

struct ISendr
{
	virtual bool connetToServer(const std::string& ip, size_t port)=0;   //connect
	virtual bool sendMessage()=0;//send message from queue
	virtual bool uploadFile(const Message& msg) = 0;	//upload file
	virtual bool shutDownSend() = 0;			//shut down sending
	virtual void enQueue(const Message &msg)=0;	//enqueue
	virtual Message deQueue()=0;				//dequeue
	virtual size_t queueSize()=0;				//queue size
};

struct IRecvr
{
	virtual bool start() = 0;				//start listening
	virtual std::string receiveString(char terminator) = 0;//receive string
	virtual bool receiveBuffer(size_t bytes, char* buffer) = 0;  //receive buffer

	virtual void enQueue(const Message& s)=0;   //enQue a message when a task is received 
	virtual Message deQueue()=0;				  //deQue a message
	virtual size_t queueSize()=0;				//queue size
};

extern "C" {
	struct ObjectFactory
	{
		DLL_OUT ISendr* createSendr();			//create a sender
		DLL_OUT IRecvr* createRecvr(size_t port);	//create a receiver
	};
}

#endif
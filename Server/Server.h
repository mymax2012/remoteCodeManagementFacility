#ifndef WPF_SERVER_H
#define WPF_SERVER_H
/////////////////////////////////////////////////////////////////////////////
//                           //
// - packing sender and receiver into a server class					   //
//													                       //
//																		   //
//	Surface pro 3, Windows 8.1											   //
//  Zhenya Li												               //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package constructs a server which contains a sender and receiver. 
	Can receive and send messages from and to a client.
	this server is used for communicating with client, processing message and sending replies
	
	
	Commands rules:
	--------------------
	--client requests:

	EXPLORE_DIR
	PORT		:request port num
	DIRECTORY	:the directory relative to server folder

	UPLOAD_FILE
	FILE_NAME	:full path
	PORT		:request port num
	DIRECTORY	:upload to dir

	DOWNLOAD_FILE
	FILE_NAME   :full path
	PORT		:request port num
	DIRECTORY	:download to dir

	SEARCH_FILE
	PORT		:request port num
	FILE_PATTERN:file patthern
	FILE_PATTERN:file patthern
	...
	SEARCH_TEXT	:text to be searched

	--server replies:

	EXPLORE_RESULT
	PORT
	<BODY>

	UPLOAD_RECEIPT
	PORT
	FILE_NAME

	UPLOAD_FILE			//for clients, download from server is upload from server
	FILE_NAME	:full path
	PORT		:request port num
	DIRECTORY	:upload to dir

	SEARCH_RESULT
	PORT
	<BODY>


	Public Interface:
	-------------------
	void startNewHandler();			
		start a new handler to handle received messages
	void exploreServerDir(const Message&);	
		explore a server directory
	void searchText(const Message&);	
		search file with patterns and text
	void uploadFileReceipt(const Message&);	
		receive a file from client
	void downloadFileRequest(const Message&);  
		upload a file from server to client
	void sendReply(); 
		send reply when got message
	bool shutDownSend();			
		shut down sending message
	void exploreDir(sPtr root, const std::string& dir); 
		explore a directory and return the xml tree
	void addToSendingQ(Message &msg);   
		add a task of message to sender's queue
	bool sendMessageOut();			
		send a message from the sending queue

*  Required Files:
*  ---------------
*	myChannel.h, myChannel.cpp,
	Message.h, Message.cpp
	Sockets.h, Sockets.cpp
	FileSystem.h, FileSystem.cpp
	AppHelpers.h, AppHelpers.cpp
	FileManager.h, FileManager.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - 
*
*/


#include "../myChannel/myChannel.h"
#include "../Tools/hw2/XmlDocument.h"
#include "../Tools/Sockets/Sockets.h"
#include "../Tools/FileSystemDemo/FileSystem.h"

//class serverDispatcher{
//public:
//	void operator()(const Message&);
//
//};
using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement >;


class Server{
public:
	Server(size_t port, Socket::IpVer ipv = Socket::IP6);//ctor receiving port number and OP version to initialize receiver	
	~Server();
	void startNewHandler();			//start a new handler to handle received messages	
	
	void exploreServerDir(const Message&);	//explore a server directory
	void searchText(const Message&);	//search file with patterns and text
	void uploadFileReceipt(const Message&);	//receive a file from client
	void downloadFileRequest(const Message&);  //upload a file from server to client

	void sendReply(); //send reply when got message
	bool shutDownSend();			//shut down sending message	
	bool connectToClient(const std::string& ip, size_t port);			//connect to client if want to send message to it

	void exploreDir(sPtr root, const std::string& dir); //explore a directory and return the xml tree

	//size_t curReceived();  //return size of receiver's queue
	void addToSendingQ(Message &msg);   //add a task of message to sender's queue 
	bool sendMessageOut();			//send a message from the sending queue	

private:
	//XmlDocument pathTreeToXML(std::vector<std::string>&);//convert a parse tree to xml file
	void sendXMLFile(std::string &str, std::string& file_name);	//send a XML string message with port number
	void sendFile(const std::string& file_name,const std::string& dir);//send a file to client port
	Message getRcvdMsgFromQ();      //get received message

	size_t port_num;    //port number of this server
	std::string ip;		//ip address
	
	//XmlDocument *serverTree;

	std::string serverFolderName;		//name of server's directory

	ISendr* sender;		//server's sender
	IRecvr* receiver;	//server's receiver
};


#endif
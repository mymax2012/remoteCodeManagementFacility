/////////////////////////////////////////////////////////////////////////////
// myChannel.cpp - for CSE687 Project #4, Spring 2015						//
// - build as DLL used for WPF interface									//
// - myChannel reads from sendQ and writes to recvQ							//
//   
//	ver 1.0
//	Surface pro 3, Windows 8.1												//
//  Zhenya Li																//
/////////////////////////////////////////////////////////////////////////////

#define MYCHANNEL_DLL
#include "../Tools/Sockets/Sockets.h"
#include "myChannel.h"
#include "Cpp11-BlockingQueue.h"
#include "../Tools/ApplicationHelpers/AppHelpers.h"
#include "../Tools/FileSystemDemo/FileSystem.h"
#include <chrono>

#include <iostream>

using namespace ApplicationHelpers;
typedef std::pair<std::string, std::string> p_att;
typedef std::vector<std::pair<std::string, std::string>> attribs;


//*************************************************
//*****************class Sender********************

class Sender:public ISendr{
public:
	Sender();
	~Sender();
	virtual bool connetToServer(const std::string& ip, size_t port);   //connect
	virtual bool sendMessage();
	virtual bool uploadFile(const Message& msg);		//get a message and upload a file specified in the message
	virtual bool shutDownSend();							//shut down sending
	virtual void enQueue(const Message &msg);				//enqueue a task message 
	virtual Message deQueue();								//dequeue a task message
	virtual size_t queueSize();								//return queue size

	bool sendString(const std::string & s, Socket::byte terminator = '\0');	//send message string
	bool sendBuffer(size_t bytes, Socket::byte* buffer);//send a buffer to server

private:
	SocketSystem sSystemSender;					//socket system
	SocketConnecter connector;
	BlockingQueue<Message> sendQueue;
};


//*************************************************
//*************class ClientHandler*****************

class ClientHandler{
public:
	ClientHandler(BlockingQueue<Message>& recvQueueRef);  //ctor receive a reference of the receiver's queue
	~ClientHandler();
	void operator()(Socket& sock);						//functor operator ()
	bool handlerUploadFile(Socket& sock, const Message& msg);//handle is functor receive a upload file task
	void transferBlock(Socket& sock, FileSystem::File &file);//get a block from file and write it on server disk
private:
	BlockingQueue<Message> &recvQueue;
};

//*************************************************
//****************class Receiver*******************
class Receiver:public IRecvr{
public:
	Receiver(size_t port, Socket::IpVer ipv = Socket::IP6);  //ctor
	~Receiver();  //dtor

	virtual bool start();     //listener start 
	virtual std::string receiveString(Socket::byte terminator = '\0');	//receive string from sender
	virtual bool receiveBuffer(size_t bytes, Socket::byte* buffer);		//receive buffer from sender
	
	virtual void enQueue(const Message& s);   //enQue a message when a task is received 
	virtual Message deQueue();				  //deQue a message
	virtual size_t queueSize();					//return the size of queue

private:
	SocketSystem sSystemReceiver;       //socketsystem
	SocketListener listener;			//listener
	BlockingQueue<Message> recvQueue;	//receiving queue stores successfully received file's or string's messages 
	ClientHandler handler;				//callable object to create a socket and thread for receiving
};









//*************************************************
//***************IMPLEMENT of class Sender*********

//----shut down sending
bool Sender::shutDownSend(){ return connector.shutDownSend(); }

//----ctor of sender

Sender::Sender(){}
//----dtor of sender

Sender::~Sender(){}
//-----conncet sender to server

bool Sender::connetToServer(const std::string& ip, size_t port){
	return connector.connect(ip, port);
}
//-----send the whole message with header to server

bool Sender::sendMessage(){
	if (queueSize() == 0)return false;
	Message msg = deQueue();
	std::string command = msg.getHeader().getCommand();
	attribs attributes = msg.getHeader().getAttributes();
	if (command == "UPLOAD_FILE"){
		if (!uploadFile(msg))return false;
		//Verbose::show("up not false", always);
	}
	else{
		if (!sendString(msg.toString()))return false;
	}
	return true;
}
//-----send a file into server

bool Sender::uploadFile(const Message& msg){
	Message new_msg(msg);
	attribs atts = new_msg.getHeader().getAttributes();
	std::string file_name, portstr;
	size_t content_size;
	for (auto at : atts){
		if (at.first == "FILE_NAME")
			file_name = at.second;
		if (at.first == "PORT")
			portstr = at.second;
	}
	if (file_name.empty())return false;
	FileSystem::File file(file_name);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood()){
		Verbose::show("\nopen file " + file_name + " failed.\n", always);
		return false;
	}
	size_t count = 0;//test block counter
	while (file.isGood()){
		Socket::byte buf[10240];
		content_size = file.getBuffer(10240, buf);
		Message out_msg(new_msg);
		out_msg.addAttrib("CONTENT_SIZE", std::to_string(content_size));
		out_msg.addAttrib("PORT", portstr);
		if (!sendString(out_msg.toString()))return false;  //send message
		if (!sendBuffer(content_size, buf))return false;   //send buffer
		Verbose::show("\n<send>uploadfile " + std::to_string(content_size) + "bytes", always);
	}
	file.close();
	return true;
}
//-----send string to server

bool Sender::sendString(const std::string& s, Socket::byte terminator){
	Verbose::show("sender sending string " + s);
	return connector.sendString(s, terminator);
}

//-----send a block of buffer to server

bool Sender::sendBuffer(size_t bytes, Socket::byte* buffer){
	Verbose::show("<send>sender sending buffer of size " + bytes, always);
	return connector.send(bytes, buffer);
}

//-----push a string into the queue

void Sender::enQueue(const Message &msg){

	sendQueue.enQ(msg);
}
//-----dequeue and get string

Message Sender::deQueue(){
	return sendQueue.deQ();
}
//-----return the size of sender queue

size_t Sender::queueSize(){ return sendQueue.size(); }




/*********************************************
	IMPLEMENT of class ClientHandler
*********************************************/


ClientHandler::ClientHandler(BlockingQueue<Message>& recvQueueRef) :recvQueue(recvQueueRef){
}
ClientHandler::~ClientHandler(){}

//-----operator () for the callable objects
void ClientHandler::operator()(Socket& sock){
	while (true){
		std::string reStr = sock.recvString();

		if (reStr.size() == 0)
			break;
		Message msg(reStr);
		std::string command = msg.getHeader().getCommand();
		std::string filename, portstr;
		for (auto at : msg.getHeader().getAttributes()){
			if (at.first == "FILE_NAME")
				filename = at.second;
			if (at.first.find("PORT") != std::string::npos)
				portstr = at.second;
		}
		Verbose::show("\n<rcvd>received " + command + " from " + portstr + " \n", always);

		if (command == "UPLOAD_FILE"){
			handlerUploadFile(sock, msg);
			continue; // go back and get another command
		}
		else{
			recvQueue.enQ(msg);
			//if (command=="DOWNLOAD_FILE")
			continue;
		}
	}

}

//-----string handling function processing received strings

bool ClientHandler::handlerUploadFile(Socket& sock, const Message& msg){
	auto time_before = std::chrono::high_resolution_clock::now();
	Message rcvd_msg(msg), success_msg;
	success_msg.addCommand("RECEIVED_FILE");
	std::string file_name, output_name, portstr,mode;
	size_t blocksize,filesize=0;
	for (auto at : rcvd_msg.getHeader().getAttributes()){
		if (at.first == "FILE_NAME")file_name = FileSystem::Path::getName(at.second);
		if (at.first == "CONTENT_SIZE")blocksize = std::stoul(at.second);
		if (at.first == "PORT")portstr = at.second;
		if (at.first == "DIRECTORY")output_name = at.second;
		//if (at.first == "TRANSFER_MODE")mode = at.second;
	}
	//receive the first block of buffer
	Socket::byte buf[10240];
	sock.recv(blocksize, buf);
	//Verbose::show("dirname=" + output_name + " filename=" + file_name, always);
	
	output_name = output_name+"/"+ file_name;

	FileSystem::File file(output_name);
	success_msg.addAttrib("FILE_NAME", output_name);
	success_msg.addAttrib("PORT", portstr);
	//success_msg.addAttrib("TRANSFER_MODE", mode);
	success_msg.setBody("[File upload finished!] "+output_name);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood())return false;
	else { file.putBuffer(blocksize, buf); }//write this block into the server drive
	if (blocksize < 10240){
		file.close();
		std::string duration = toString(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time_before).count());
		success_msg.addAttrib("RESPONSE_TIME", duration);
		recvQueue.enQ(success_msg);
		Verbose::show("File uploaded success: " + output_name + "\n", always);
		return true;
	}
	transferBlock(sock, file);
	file.close();
	std::string duration = toString(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time_before).count());
	success_msg.addAttrib("RESPONSE_TIME", duration);
	recvQueue.enQ(success_msg);
	Verbose::show("File uploaded success: " + output_name + "\n", always);
	return true;
}
//-----transfer a block from received from socket and write into server drive

void ClientHandler::transferBlock(Socket& sock, FileSystem::File &file){
	while (true){
		size_t block_s;
		std::string reStr = sock.recvString();
		if (reStr.size() > 0){
			Message m(reStr);
			for (auto att : m.getHeader().getAttributes()){
				if (att.first == "CONTENT_SIZE"){
					block_s = std::stoul(att.second);
					break;
				}
			}
			Socket::byte bufs[10240];
			sock.recv(block_s, bufs);
			file.putBuffer(block_s, bufs);
		}
		if (block_s < 10240)break;
	}
}


/*********************************************
		IMPLEMENT of class Receiver
*********************************************/

//-----ctor takes a port number and ip version as input

Receiver::Receiver(size_t port, Socket::IpVer ipv) :listener(port, ipv), handler(recvQueue){}
//-----dtor

Receiver::~Receiver(){}
//-----start receiving

bool Receiver::start(){
	return listener.start(handler);
}
//-----receive string from sender

std::string Receiver::receiveString(Socket::byte terminator){
	std::string str = listener.recvString(terminator);
	return str;
}
//-----receive buffer from sender

bool Receiver::receiveBuffer(size_t bytes, Socket::byte* buffer){
	return listener.recv(bytes, buffer);
}


//-----enqueue a message

void Receiver::enQueue(const Message &s){ recvQueue.enQ(s); }
//-----dequeue a message
Message Receiver::deQueue(){ 
	return recvQueue.deQ(); 
}
//-----return queue size
size_t Receiver::queueSize(){ return recvQueue.size(); }



//*************************************************
//*****************Object Factory******************
ISendr* ObjectFactory::createSendr(){ return new Sender; }
IRecvr* ObjectFactory::createRecvr(size_t port){ return new Receiver(port); }

//-----------test stub
#ifdef CHANNEL_TEST
void main(){
	ObjectFactory* oj=new ObjectFactory;
	Sender s=oj->createSendr();
	Receiver re=oj->createRecvr(7000);


}
#endif
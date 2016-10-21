#include "Server.h"
#include <chrono>
#include "../Tools/hw1/FileManager.h"


using namespace ApplicationHelpers;

using sPtr = std::shared_ptr < AbstractXmlElement >;

//----ctor takes port number and ip version

Server::Server(size_t port, Socket::IpVer ipv) :port_num(port), ip("localhost"){
	ObjectFactory oFact;
	sender = oFact.createSendr();
	receiver = oFact.createRecvr(port);

	serverFolderName = "server_" + std::to_string(port);
	FileSystem::Directory::create(serverFolderName);
	startNewHandler();
}

Server::~Server(){
	delete sender;
	delete receiver;
	//delete serverTree;
}
//----start to receive from clients

void Server::startNewHandler(){
	Verbose::show("Server starts listening.\n", always);
	receiver->start();
	while (true){		
		Message msg = getRcvdMsgFromQ();

		if (msg.getHeader().getCommand() == "")continue;

		std::string command = msg.getHeader().getCommand();
		if (command == "EXPLORE_DIR"){
			exploreServerDir(msg);
		}
		else if (command == "SEARCH_FILE")
			searchText(msg);
		else if (command == "RECEIVED_FILE")
			uploadFileReceipt(msg);
		
		else if (command == "DOWNLOAD_FILE")
			downloadFileRequest(msg);
		else
		{
			Verbose::show("Bad request from client!\n", always);
		}
	}
}
void Server::exploreServerDir(const Message& msg){
	auto time_before = std::chrono::high_resolution_clock::now();
	Message m=msg, newmsg;
	size_t port=0;
	std::string dir = serverFolderName;
	std::string dirName;
	for (auto at : m.getHeader().getAttributes()){
		if (at.first == "PORT") port =std::stoul(at.second);
		if (at.first == "DIRECTORY") { 
			dir += ("/" + at.second); 
			dirName = at.second;
		}
	}
	if (port == 0){
		Verbose::show("Cannot get Client port number when exploring server directroy.\n", always); 
		return;
	}
	sPtr root = makeDocElement();
	XmlDocument doc(root);
	exploreDir(root, dir);

	std::string duration = toString(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time_before).count());

	newmsg.addCommand("EXPLORE_RESULT");
	newmsg.addAttrib("PORT", std::to_string(port_num));
	newmsg.addAttrib("DIRECTORY", dirName);
	newmsg.addAttrib("RESPONSE_TIME", duration);
	newmsg.setBody(doc.tostring());

	Verbose::show("send back to client port=" + std::to_string(port), always);
	while(sender->connetToServer("localhost",port)==false)
		Sleep(1000);	//connect back to client
	sender->enQueue(newmsg);
	sender->sendMessage();
	sender->shutDownSend();

}
//recursively explore a given path with a xml root
void Server::exploreDir(sPtr root,const std::string& dir){
	//Sleep(1000);
	sPtr pNode = root;
	sPtr dir_root = makeTaggedElement(dir);
	dir_root->addAttrib("Type","Dir");
	pNode->addChild(dir_root);
	for (auto f : FileSystem::Directory::getFiles(dir)){
		sPtr file_element = makeTextElement(f);
		dir_root->addChild(file_element);
	}
	std::vector<std::string> dirs = FileSystem::Directory::getDirectories(dir);
	if (dirs.size() > 2){
		for (size_t i = 2; i < dirs.size();++i){
			std::string dir_path = dir + "/" + dirs[i];
			exploreDir(dir_root, dir_path);
		}
	}
	else return;
}
//search text in directories
void Server::searchText(const Message&msg){
	auto time_before = std::chrono::high_resolution_clock::now();
	Verbose::show("Search text called", always);
	Message m = msg, newmsg;
	newmsg.addCommand("SEARCH_RESULT");
	newmsg.addAttrib("PORT", std::to_string(port_num));
	size_t port = 0;
	std::string dir;
	std::vector<std::string> patterns;
	std::string searchText;
	std::string xmlBody;
	for (auto at : m.getHeader().getAttributes()){
		if (at.first == "PORT") port = std::stoul(at.second);
		if (at.first == "FILE_PATTERN"){ 
			patterns.push_back(at.second); 
			newmsg.addAttrib("FILE_PATTERN", at.second);
		}
		if (at.first == "SEARCH_TEXT"){
			searchText = at.second;
			newmsg.addAttrib("SEARCH_TEXT", at.second);
		}
		if (at.first == "DIRECTORY"){
			dir = at.second;
			dir = serverFolderName + "/" + dir;
			newmsg.addAttrib("DIRECTORY", dir);
		}
	}
	if (port == 0){
		Verbose::show("Cannot get Client port number when exploring server directroy.\n", always);
		return;
	}
	
	FileManager fm;
	std::vector<std::string>results = fm.searchFile(patterns, dir, searchText);
	for (auto r : results)
		xmlBody += (r+"\n");
		//Verbose::show(r, always);
	newmsg.setBody(xmlBody);

	std::string duration = toString(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time_before).count());
	newmsg.addAttrib("RESPONSE_TIME", duration);
	Verbose::show("send back to client port=" + std::to_string(port), always);
	while (sender->connetToServer("localhost", port) == false)
		Sleep(1000);	//connect back to client
	sender->enQueue(newmsg);
	sender->sendMessage();
	sender->shutDownSend();
}
//receive a file from client
void Server::uploadFileReceipt(const Message& msg){

	Message m = msg, newmsg;
	newmsg.addCommand("UPLOAD_RECEIPT");
	newmsg.addAttrib("PORT", std::to_string(port_num));
	size_t port = 0;
	for (auto at : m.getHeader().getAttributes()){
		if (at.first == "PORT") port = std::stoul(at.second);
		if (at.first == "FILE_NAME"){
			newmsg.addAttrib("FILE_NAME",at.second);
		}
		if (at.first == "RESPONSE_TIME")
			newmsg.addAttrib("RESPONSE_TIME",at.second);
	}
	newmsg.setBody(m.getBody());
	if (port == 0){
		Verbose::show("Cannot get Client port number when exploring server directroy.\n", always);
		return;
	}

	Verbose::show("send back to client port=" + std::to_string(port), always);
	while (sender->connetToServer("localhost", port) == false)
		Sleep(1000);	//connect back to client
	sender->enQueue(newmsg);
	sender->sendMessage();
	sender->shutDownSend();
}
//when received a download file request, upload file to client
void Server::downloadFileRequest(const Message&msg){
	Message m = msg, newmsg;
	newmsg.addCommand("UPLOAD_FILE");//upload to client
	newmsg.addAttrib("PORT", std::to_string(port_num));
	size_t port = 0;
	//Verbose::show("!!!!in server's donwload request", always);
	std::string file_name,dir;
	for (auto at : m.getHeader().getAttributes()){
		if (at.first == "PORT") port = std::stoul(at.second);
		if (at.first == "FILE_NAME"){
			//file_name = FileSystem::Path::getName(at.second);
			newmsg.addAttrib("FILE_NAME", at.second);
		}
		if (at.first == "DIRECTORY")
			newmsg.addAttrib("DIRECTORY", at.second);
	}
	while (sender->connetToServer("localhost", port) == false)
		Sleep(1000);	//connect back to client
	sender->enQueue(newmsg);
	sender->sendMessage();
	sender->shutDownSend();
}


//----send a XML string message to client 

void Server::sendXMLFile(std::string &str, std::string& file_name){
	Message msg;
	msg.addCommand("XML_FILE");
	msg.addAttrib("FILE_NAME", file_name);
	//msg.addAttrib("IP", ip);
	msg.setBody(str);
	addToSendingQ(msg);
}
//-----send a file to client port

void Server::sendFile(const std::string&file_name, const std::string& dir){
	Message msg;
	msg.addCommand("UPLOAD_FILE");
	msg.addAttrib("FILE_NAME", file_name);
	msg.addAttrib("DIRECTORY", dir);
	addToSendingQ(msg);
	Sleep(500);
	sendMessageOut();
}
//-----sent a reply to client when received a message successfully

void Server::sendReply(){
	shutDownSend();
	Message  m_rcvd = getRcvdMsgFromQ();
	size_t port;
	std::string file_name;
	std::string command = m_rcvd.getHeader().getCommand();

	for (auto at : m_rcvd.getHeader().getAttributes()){
		if (at.first == "PORT")
			port = std::stoul(at.second);
		if (at.first == "FILE_NAME")
			file_name = at.second;
	}
	if (command == "RECEIPT_BACK") {
		Verbose::show("<rcvd>server :" + std::to_string(this->port_num) + " received RECEIPT_BACK for \"" + file_name + "\" from client:" + std::to_string(port) + "\n", always);
		return;
	}
	::Sleep(150);
	connectToClient("localhost", port);
	Message msg;
	if (command == "RECEIPT")
		msg.addCommand("RECEIPT_BACK");
	else msg.addCommand("RECEIPT");
	msg.addAttrib("FILE_NAME", file_name);
	//msg.addAttrib("PORT", std::to_string(this->port_num));
	addToSendingQ(msg);
	sendMessageOut();
	::Sleep(150);
	Verbose::show("<send>server :" + std::to_string(this->port_num) + " sending out " + msg.getHeader().getCommand() + " for \"" + file_name + "\" to :" + std::to_string(port) + "\n", always);
}
//----send one message from the sending queue

bool Server::sendMessageOut(){
	return sender->sendMessage();
}

//-----add a message task to sender's queue

void Server::addToSendingQ(Message &msg){
	Message msg_with_port(msg);
	msg_with_port.addAttrib("PORT", std::to_string(this->port_num));
	
	sender->enQueue(msg_with_port);
}

//----shut down sending

bool Server::shutDownSend(){
	return sender->shutDownSend();
}
//----return size of receiver's queue
//
//size_t Server::curReceived(){
//	return receiver->queueSize();
//}

//----connet to a Client if want to send message

bool Server::connectToClient(const std::string& ip, size_t port){
	//receiving_port = std::to_string(port);
	return sender->connetToServer(ip, port);
}


//----get a received message in the receiver queue and dequeue

Message Server::getRcvdMsgFromQ(){
	return receiver->deQueue();
}

////convert a parse tree to xml file
//XmlDocument Server::pathTreeToXML(std::vector<std::string>& paths){
//	
//	sPtr root = makeDocElement();
//	for (auto p : paths){
//		std::string left = p;
//		std::vector<sPtr> eles;
//		std::vector<std::string> v;
//		
//		while (left.find("/") != std::string::npos){
//
//			std::string part = p.substr(0, p.find_first_of("/"));
//			left = p.substr(p.find_first_of("/") + 1);
//			v.push_back(part);
//		}
//		v.push_back(left);
//		for (auto d : v){
//			eles.push_back(makeTaggedElement(d));
//		}
//		root->addChild(eles[0]);
//		sPtr newroot = root;
//		for (size_t i = 1; i < eles.size()-2; ++i){
//			newroot = eles[1];
//			for (auto child:root->children())
//			eles[i]->addChild(eles[i+1]);
//		}
//		
//	}
//}


//-----test stub-----
#ifdef SERVER_TEST

void main(int argc, char* argv[]){
	if (argc >= 2){
	size_t port = std::stoul(argv[1]);
	Server server(port);
	
	}
	//Server server(8000);
	//server.connectToClient("localhost", 8080);
	//Message m;
	//m.addCommand("EXPLORE_DIR");
	//m.addAttrib("DIRECTORY", "");
	//client.addToSendingQ(m);
	//client.sendMessageOut();
	/*server1.exploreDir(r, "Debug");*/
	//std::cout << doc.tostring();
	
	
	//std::vector<std::string> vec,vec2;
	//vec.push_back("*.h");
	//FileManager fm;
	//vec2=fm.searchFile(vec,"..");
	//for (auto i : vec2)
	//	std::cout << i << std::endl;


	std::cin.get();
}
#endif
#ifndef DATASTORE_H
#define DATASTORE_H
///////////////////////////////////////////////////////////////////////////
// Datastore.h	used to store file searched results						 //
//																		 //
// ver 2.0       
//
// surface pro 3, windows 8.1, visual studio 2013
// Zhenya Li zli52@syr.edu												 //
///////////////////////////////////////////////////////////////////////////
/*


*  Package Operations:
*  -------------------
*  This package is used to store searching results of file manager

	Public interface:
	-------------------
	using File = string;
	using Path = string;
	using Ref = set<string>::iterator;
	using ListofRefs = list<Ref>;

	void addToPathSet(const std::string &);
		add a path to set
	std::set<std::string> getPathSet();		
		get the path set
	void addToFileMap(std::string&,std::string&);
		add pair of file name and path to map
	const std::map<std::string, std::list<std::set<std::string>::iterator>> &getFileMap();	
		get store map


*  Required Files:
*  ---------------
	Display.h, Display.cpp
	Datastore.h, Datastore.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*
*
*  
*/
#include <iostream>
#include <cstdlib>
#include <map>
#include <set>
#include <list>
#include <string>

class Datastore{

public:
	Datastore();
	~Datastore();
	void addToPathSet(const std::string &);// add a path to set
	std::set<std::string> getPathSet();		//get the path set
	void addToFileMap(std::string&,std::string&);//add pair of file name and path to map
	const std::map<std::string, std::list<std::set<std::string>::iterator>> &getFileMap();	//get store map

private:
	std::set<std::string> file_set;
	std::map<std::string, std::list<std::set<std::string>::iterator>> file_map;
};



#endif
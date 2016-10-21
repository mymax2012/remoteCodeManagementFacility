///////////////////////////////////////////////////////////////////////////
// Cataloger.h	used to parse file searched results						 //
//          															//
// ver 2.0																//
//																		//
// surface pro 3, windows 8.1, visual studio 2013
// Zhenya Li,															 //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package is used to parse searching results of file manager
	Can also search for files with given text

	Public interface:
	-------------------
	void setAFullPathToData(const std::string&);
		set a full path to data
	Datastore getData();
		get data
	std::vector<std::string> getFullPathsInMap();	
		get all the paths in storing map
	std::vector<std::string> getMatchFileCurDir(const std::string& filepath, const std::string& filename);  
		get matched files in current directory by file pattern
	std::vector<std::string> getMatchFileWholeDir(const std::string& filepath, const std::string& filename); 
		get matched files in all directories rooted by current dirctory by file patthern
	std::vector<std::string> getFilesInPath(const std::string& filepath);
		get files in this directory
	std::vector<std::string> getFilesInRootPath(const std::string& filepath); 
		get files in all directories rooted by this directory
	std::vector<std::string> getMatchTextFiles(const std::string& text="");	
		do text search and return matched files

*  Required Files:
*  ---------------
*	Cataloger.h, Cataloger.cpp
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



#ifndef CATALOG_H
#define CATALOG_H
#include <iostream>
#include <string>
#include "Datastore.h"
#include <vector>

class Cataloger{
public:

	Cataloger();
	~Cataloger();
	
	void setAFullPathToData(const std::string&);//set a full path to data
	Datastore getData();// get data
	std::vector<std::string> getFullPathsInMap();	//get all the paths in storing map
	std::vector<std::string> getMatchFileCurDir(const std::string& filepath, const std::string& filename);  //get matched files in current directory by file pattern
	std::vector<std::string> getMatchFileWholeDir(const std::string& filepath, const std::string& filename); //get matched files in all directories rooted by current dirctory by file patthern
	std::vector<std::string> getFilesInPath(const std::string& filepath);//get files in this directory
	std::vector<std::string> getFilesInRootPath(const std::string& filepath); //get files in all directories rooted by this directory
	std::vector<std::string> getMatchTextFiles(const std::string& text="");	//do text search and return matched files
	
	
	void printData();
	void printSet();

private:
	
	Datastore data;
};
#endif
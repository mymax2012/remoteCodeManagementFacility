///////////////////////////////////////////////////////////////////////////
// FileManager.h	used to process file search functions                //
//																		 //
// ver 2.0       
//
// surface pro 3, windows 8.1, visual studio 2013
// Zhenya Li															//
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package is used to construct a file manager system,
	can search files in a directort by file patterns, and text the files may contain

	Public interface:
	-------------------
	std::set<std::string> getFullPathSet();
		get full path set and store into cataloger
	Cataloger getCataloger();
		get stored cataloger
	std::vector<std::string> searchFile(const std::vector<std::string>& patterns, const std::string& path = ".",const std::string& text = "");
		search all files with path, patterns, and text. returns a vector of full paths.

*  Required Files:
*  ---------------
*	Cataloger.h, Cataloger.cpp
	Display.h, Display.cpp
	Datastore.h, Datastore.cpp
	FileSystem.h, Filesystem.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
*/

#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <vector>
#include <string>
#include "Cataloger.h"
//#include "../hw2/XmlDocument.h"

class FileManager
{
public:

	//void processCommend(const std::vector<std::string>& patterns, const std::string& path = ".", 
	//					const std::string& text = "", const bool searchRooted = false, 
	//					const bool file_search_mode = false, const bool show_duplicates = false);	
	std::set<std::string> getFullPathSet();
	Cataloger getCataloger();
	
	std::vector<std::string> searchFile(const std::vector<std::string>& patterns, const std::string& path = ".",
		const std::string& text = "");//search all files with path, patterns, and text 

	//XmlProcessing::XmlDocument searchFileInXML(const std::vector<std::string>& patterns, const std::string& path = ".",
	//	const std::string& text = "");//search file and return xml
private:
	void setAllFilePath(const std::string& path = ".", const std::string& pattern = "*.*");
	//void searchFileInXML(sPtr root,const std::string& path = ".", const std::string& pattern = "*.*");//search file and return xml
	void setCurFilePath(const std::string& path = ".", const std::string& pattern = "*.*");
	void storeToCataloger();
	std::set<std::string> full_path;
	Cataloger cat;
};

#endif
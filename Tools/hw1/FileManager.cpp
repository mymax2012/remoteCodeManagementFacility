///////////////////////////////////////////////////////////////////////////
// FileManager.cpp	used to process file search functions                //
//																		 //
// ver 2.0       
//
// surface pro 3, windows 8.1, visual studio 2013
// Zhenya Li												    		 //
///////////////////////////////////////////////////////////////////////////


#include "FileManager.h"
#include "FileSystem.h"
#include "Display.h"
#include "Cataloger.h"
#include <iostream>
using namespace std;


//search current directory for files
void FileManager::setCurFilePath(const std::string& path, const std::string& pattern)
{
	//int f_count = FileSystem::Directory::getFiles(path, pattern).size();

	FileSystem::Directory::setCurrentDirectory(path);
	std::string cur_p = FileSystem::Directory::getCurrentDirectory();

	for (auto i : FileSystem::Directory::getFiles(cur_p, pattern))
	{
		this->full_path.insert(cur_p + "\\" + i);
	}
}

//search recursively for all files rooted by current path
void FileManager::setAllFilePath(const std::string& path, const std::string& pattern)
{
	std::vector<string> filevec = FileSystem::Directory::getFiles(path, pattern);
	std::vector<string> dirvec = FileSystem::Directory::getDirectories(path);


	for (auto i : filevec)
	{
		this->full_path.insert(path + "\\" + i);
	}
	std::string p;

	if (dirvec.size() > 2)
	{
		for (size_t i = 2; i < dirvec.size(); ++i)
		{
			p = path+"\\"+dirvec[i];
			setAllFilePath(p, pattern);
			
		}
	}
}
//search file and return xml

//set searched file(s) paths 
//void FileManager::processCommend(const std::vector<std::string>& patterns, const std::string& path,
//	const std::string& text, const bool searchRooted,
//	const bool file_search_mode, const bool show_duplicates)
//{
//	std::cout << "///////////////////////////////////////////////" << std::endl;
//	std::cout << "Searching entire directory:" << (searchRooted ? "YES" : "NO") << std::endl;
//	std::cout << "Searching text :" << (file_search_mode ? "YES" : "NO") << std::endl;
//	std::cout << "Show duplicated files:" << (show_duplicates ? "YES" : "NO") << std::endl;
//	std::cout << "///////////////////////////////////////////////" << std::endl;
//
//	if (!searchRooted){
//		for (size_t i = 0; i < patterns.size(); i++){
//			setCurFilePath(path, patterns[i]);
//		}
//	}
//	else{
//		for (size_t i = 0; i < patterns.size(); i++){
//			setAllFilePath(path, patterns[i]);
//		}	
//	}
//	storeToCataloger();
//
//	Display::showBriefsummary(cat);
//
//	if (file_search_mode)
//		Display::showMatchedFilesInVec(cat.getMatchTextFiles(text), show_duplicates);
//	else
//		Display::showMatchedFilesInVec(cat.getFullPathsInMap(), show_duplicates);
//
//	const int size = 1000;
//	char s[size];
//	string search_text;
//	std::vector<std::string> new_patterns, match_files_in_map;
//
//	cout << endl << "Please input text and patterns you want to search in files(case sensitive)" << endl;
//	cout << "plz input text as \"your text\", and each file pattern divided with space.ctrl+z to quit" << endl;
//	while (cin.getline(s,size,'\n')){
//		search_text = Display::getSearchTextFromUserInput(Display::seperateString(string(s)));
//		new_patterns = Display::getSearchPatternsFromUserInput(Display::seperateString(string(s)));
//		match_files_in_map = Display::getMatchPatternFilesFromVectorOfFiles(new_patterns, cat.getMatchTextFiles(search_text));
//		Display::showMatchedFilesInVec(match_files_in_map);
//		std::cout << std::endl << "Please input text and patterns you want to search in files(case sensitive)" << std::endl;
//		cout << "plz input text as \"your text\", and each file pattern divided with space.ctrl+z to quit" << endl;
//	}
//}


//search all files with path, patterns, and text 
std::vector<std::string> FileManager::searchFile(const std::vector<std::string>& patterns, 
												const std::string& path,
												const std::string& text){
	
	for (size_t i = 0; i < patterns.size(); i++){
		setAllFilePath(path, patterns[i]);
	}
	storeToCataloger();
	//std::vector<std::string> result;
	std::vector<std::string> result = cat.getMatchTextFiles(text);
	return result;

}
//search file and return xml
//XmlDocument FileManager::searchFileInXML(const std::vector<std::string>& patterns,
//	const std::string& path,
//	const std::string& text){
//
//	sPtr root = makeDocElement();
//	XmlDocument doc(root);
//
//
//	return doc;
//}

//get full path set 
std::set<std::string> FileManager::getFullPathSet()
{
	return full_path;
}

//store the path vector into the catalog datastore system
void FileManager::storeToCataloger()
{
	for (auto path : full_path)
	{
		cat.setAFullPathToData(path);
	}
}

//get the catalog
Cataloger FileManager::getCataloger()
{
	return cat;
}


//-------tset stub-----
#ifdef FILEMANAGER_TEST
void mian(){

	FileManager fm;
	std::vector<std::string> v;
	v.pushback("*.cpp");
	v.pushback("*.h");
	for(auto i:fm.searchFile(v,"../","main"))
		std::cout<<i<<"\n";

}

#endif // FILEMANAGER_TEST

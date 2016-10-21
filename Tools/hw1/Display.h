#ifndef DISPLAY_H
#define DISPLAY_H
///////////////////////////////////////////////////////////////////////////
// display.h	used to display file searched results					//
//																		 //
// ver 2.0       
//
// surface pro 3, windows 8.1, visual studio 2013
// Zhenya Li															 //
///////////////////////////////////////////////////////////////////////////
/*


*  Package Operations:
*  -------------------
*  This package is used to display file search results

	Public interface:
	-------------------


	//show match files from a vector of file names
	static bool showMatchedFilesInVec(const std::vector<std::string>& fullpath_vec, const bool showDuplicate=false);

	//show duplicated fiels from a vector of file names
	static bool showDuplicateInVec(const std::vector<std::string>& fullpath_vec);

	//show a brief summary
	static void showBriefsummary(Cataloger);

	//separate string
	static vector<string> seperateString(string s);

	//get search text from user input commend line
	static string getSearchTextFromUserInput(vector<string>);

	//get search patterns from user input commend line
	static vector<string> getSearchPatternsFromUserInput(vector<string>);

	//get matched files with patterns from vector of filenames
	static vector<string> getMatchPatternFilesFromVectorOfFiles(const vector<string>& patterns, const vector<string>& files);


*  Required Files:
*  ---------------
Display.h, Display.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*
*

*/



#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <string>
#include "Cataloger.h"
using namespace std;

class Display
{
public:
	//show match files from a vector of file names
	static bool showMatchedFilesInVec(const std::vector<std::string>& fullpath_vec, const bool showDuplicate=false);
	//show duplicated fiels from a vector of file names
	static bool showDuplicateInVec(const std::vector<std::string>& fullpath_vec);
	//show a brief summary
	static void showBriefsummary(Cataloger);
	//separate string
	static vector<string> seperateString(string s);
	//get search text from user input commend line
	static string getSearchTextFromUserInput(vector<string>);
	//get search patterns from user input commend line
	static vector<string> getSearchPatternsFromUserInput(vector<string>);
	//get matched files with patterns from vector of filenames
	static vector<string> getMatchPatternFilesFromVectorOfFiles(const vector<string>& patterns, const vector<string>& files);
};

#endif
///////////////////////////////////////////////////////////////////////////
// display.cpp	used to display file searched results					 //
//																		 //
// ver 2.0       
//
// surface pro 3, windows 8.1, visual studio 2013
// Zhenya Li, CSE687 - Object Oriented Design, Spring 2015				 //
///////////////////////////////////////////////////////////////////////////

#include "Display.h"
#include "Datastore.h"
#include "FileSystem.h"
using fullPath = std::string;
//show match files from a vector of file names
bool Display::showMatchedFilesInVec(const std::vector<fullPath>& path_vec, const bool showDuplicate)
{
	if (path_vec.empty())
	{
		std::cout << "|*NOT FOUND*| There is no matching files." << std::endl;
		return false;
	}
	std::cout<<std::endl << "All the files matched: " << std::endl << std::endl;
	for (auto i : path_vec)
	{
		std::cout << i.c_str() << std::endl;
	}
	if (showDuplicate){
		std::cout << std::endl << "---------Duplicated Files----------" << std::endl << std::endl;
		if (showDuplicateInVec(path_vec));
		else std::cout << "|*NOT FOUND*| There are no duplicated files" << std::endl;
		std::cout << std::endl << "-----------------------------------" << std::endl << std::endl;
	}
	return true;
}

//show duplicate files in vector, if there are no duplicates, return false
bool Display::showDuplicateInVec(const std::vector<fullPath>& path_vec)
{
	Datastore dat;
	for (auto fullpath : path_vec)
		dat.addToFileMap(FileSystem::Path::getName(fullpath), FileSystem::Path::getPath(fullpath));
	for (auto name : dat.getFileMap())
	{
		if (name.second.size() > 1)
		{
			std::cout << "The file " << name.first << " is contained in: " << std::endl;
			for (auto path : name.second)
				std::cout << *path << std::endl;
			return true;
		}
	}
	return false;
}
//seperate a string into vector of strings divided by space
vector<string> Display::seperateString(string s)
{
	vector<string> return_vec;
	if (s.size() != 0){
		string temp = s;
		temp = temp.substr(temp.find_first_not_of(' '), temp.find_last_not_of(' ') + 1);
		while (temp.find_first_not_of(' ') != string::npos &&temp != "\0")
		{
			int start = temp.find_first_not_of(' ');
			temp = temp.substr(start, temp.size());
			int end = 0;
			if (temp.find(' ') == string::npos)
			{
				return_vec.push_back(temp);
				break;
			}
			else {
				end = temp.find_first_of(' ');
			}
			if (start >= end)break;
			return_vec.push_back(temp.substr(0, end));
			temp = temp.substr(end + 1, temp.size());
		}
	}
	return return_vec;
}
//get search text from user's input
string Display::getSearchTextFromUserInput(vector<string> inputs)
{
	if (inputs.empty())return "";
	for (size_t i = 0; i < inputs.size();++i)
	{
		if (inputs[i].find('\"') != string::npos){
			return inputs[i].substr(inputs[i].find_first_not_of('\"'), inputs[i].find_last_not_of('\"'));
		}
	}
	return "";
}
//get searched patterns from user input
vector<string>Display::getSearchPatternsFromUserInput(vector<string>inputs)
{
	vector<string> return_vec;
	for (auto i : inputs)
	{
		if (i.find('\"') == string::npos){
			return_vec.push_back(i);
		}
	}
	return return_vec;
}

//get all files with matched patterns from a vectro of files
std::vector<std::string> Display::getMatchPatternFilesFromVectorOfFiles(const std::vector<std::string>& patterns, const std::vector<std::string>& files)
{
	if (patterns.empty())return files;
	std::vector<std::string> return_vec;
	for (auto p : patterns)
	{
		for (auto file : files)
		{
			if (FileSystem::Path::getName(file).find(p) != string::npos)
				return_vec.push_back(file);
		}
	}
	return return_vec;
}

//show brief summary as: ** files found in ** dirs.  output all the files found in map
void Display::showBriefsummary(Cataloger cat)
{
	unsigned num_of_paths = cat.getData().getPathSet().size();
	unsigned num_of_files = cat.getData().getFileMap().size();
	std::cout << std::endl << "-----------Brief Summary-----------" << std::endl;
	std::cout << std::endl << "There are " << num_of_files << " files found in " << num_of_paths << " paths." << std::endl;
	std::cout << std::endl << "-----------------------------------" << std::endl << std::endl;
}

//---test stub----
#ifdef DISPLAY_TEST
void main(){
	std::vector<std::string>v;
	v.pushback("aa/bb/dd/cc/ff1.h");
	v.pushback("aa/bb/dd/cc/ff2.h");
	v.pushback("aa/bb/dd/cc/ff3.h");
	v.pushback("aa/bb/dd/cc/ff4.h");
	v.pushback("aa/bb/dd/cc/ff5.h");
	v.pushback("aa/bb/dd/cc/ff6.h");

	showMatchedFilesInVec(v);


	

}
#endif // DISPLAY_TEST

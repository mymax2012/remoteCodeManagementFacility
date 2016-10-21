///////////////////////////////////////////////////////////////////////////
// Cataloger.cpp	used to parse file searched results					 //
//																		 //
// ver 2.0       
//
// surface pro 3, windows 8.1, visual studio 2013
// Zhenya Li															 //
///////////////////////////////////////////////////////////////////////////

#include "Cataloger.h"
#include "FileSystem.h"

using namespace std;
//using namespace FileSystem;

using FullPath = std::string;
using filePath = std::string;
using fileName = std::string;
using text = std::string;

Cataloger::Cataloger()
{
}

Cataloger::~Cataloger()
{
}
//set one full path to data
void Cataloger::setAFullPathToData(const FullPath& input_path)
{
	fileName fn;
	filePath fp;
	unsigned int divider = input_path.find_last_of("\\");
	fn = input_path.substr(divider + 1);
	fp = input_path.substr(0, divider);
	data.addToFileMap(fn, fp);
}
//get stored data
Datastore Cataloger::getData()
{
	return data;
}

//get matched files in this specified path
std::vector<FullPath> Cataloger::getMatchFileCurDir(const filePath& filepath, const fileName& filename)
{
	vector<FullPath> path_vec;
	for (auto name : data.getFileMap())
	{
		if (name.first.find(filename) != std::string::npos)
		{
			for (auto path : name.second)
			{
				std::string p = *path;

				if (filepath == p)
				{
					path_vec.push_back(*path + "\\" + name.first);
				}
			}
		}
	}
	return path_vec;
}

//get all matched files rooted at this specified path
std::vector<FullPath> Cataloger::getMatchFileWholeDir(const filePath& filepath, const fileName &filename)
{
	vector<FullPath> path_vec;
	for (auto name : data.getFileMap())
	{
		if (name.first.find(filename) != std::string::npos)
		{
			for (auto path : name.second)
			{
				std::string p = *path;
				if (p.find(filepath) != std::string::npos)
				{
					path_vec.push_back(*path + "\\" + name.first);
				}
			}
		}
	}
	return path_vec;
}

//get all file names in full path format in the map which matches the filepath
std::vector<FullPath> Cataloger::getFilesInPath(const filePath& filepath)
{
	vector<FullPath> path_vec;
	for (auto name : data.getFileMap())
	{
		for (auto path : name.second)
		{
			std::string p = *path;
			if (p == filepath)
			{
				path_vec.push_back(*path + "\\" + name.first);
			}
		}
	}
	return path_vec;
}

//get all file names in full path format in the map rooted from the filepath
std::vector<FullPath> Cataloger::getFilesInRootPath(const filePath& filepath)
{
	vector<FullPath> path_vec;
	for (auto name : data.getFileMap())
	{
		for (auto path : name.second)
		{
			std::string p = *path;
			if (p.find(filepath)!=std::string::npos)
			{
				path_vec.push_back(*path + "\\" + name.first);
			}
		}
	}
	return path_vec;
}

//get files that contains searched text in the whole map
std::vector<FullPath> Cataloger::getMatchTextFiles(const text& search_text)
{
	vector<FullPath> return_vec;
	if (search_text == ""){
		return getFullPathsInMap();
	}
	for (auto pa : getFullPathsInMap())
	{
		/*File file(pa);
		file.open(File::in);*/
		std::ifstream file(pa);
		if (!file.good())
		{
			std::cout << std::endl << "Error: file can't open." << endl<<endl;
			continue;
		}
		while (1)
		{
			if (!file.good())
				break;	
			text t;
			getline(file, t);
			if (t.find(search_text) != std::string::npos)
			{
				return_vec.push_back(pa);
				break;
			}
		}
	}
	return return_vec;
}

//get full paths of all files in the whole map
std::vector<FullPath> Cataloger::getFullPathsInMap()
{
	vector<FullPath> return_vec;
	
	for (auto name : data.getFileMap())
	{
		for (auto path : name.second)
		{			
			return_vec.push_back(*path+"\\"+name.first);
		}
	}
	return return_vec;
}

//a test function to output the map
void Cataloger::printData()
{
	for (auto name : data.getFileMap())
	{
		for (auto path : name.second)
		{
			cout << *path << endl;
		}
	}
}
//a test function to output path set in the data
void Cataloger::printSet()
{
	cout << "path set in data is " << endl;
	for (auto i : data.getPathSet())
	{
		cout << i << endl;
	}
}


//----test stub-------
#ifdef CATALOGER_TEST
void mian(){
	Cataloger cat;
	Datastore dat;
	//dat.
	for(auto i:cat.getFullPathsInMap()){
	
		std::cout<<"the path found in map is:"<<i<<"\n";
	}
}

#endif // CATALOGER_TEST

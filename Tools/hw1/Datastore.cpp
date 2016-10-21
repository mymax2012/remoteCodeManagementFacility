///////////////////////////////////////////////////////////////////////////
// Datastore.cpp	used to store file searched results					 //
//																		 //
// ver 2.0       
//
// surface pro 3, windows 8.1, visual studio 2013
// Zhenya Li															 //
///////////////////////////////////////////////////////////////////////////

#include "Datastore.h"
#include <iostream>
#include <iomanip>
using namespace std;

using File = string;
using Path = string;
using Ref = set<string>::iterator;
using ListofRefs = list<Ref>;

//constructor
Datastore::Datastore()
{
	
}
//destructor
Datastore::~Datastore()
{
	
}
// add a path to set
void Datastore::addToPathSet(const Path &file_path)
{
	file_set.insert(file_path);
}
//get the path set
set<Path> Datastore::getPathSet()
{
	return file_set;
}
//add pair of file name and path to map
void Datastore::addToFileMap(File &file_name,  Path &file_path)
{
	file_set.insert(file_path);	
	file_map[file_name].push_back(file_set.find(file_path));
}
//get store map
const map<File, ListofRefs>& Datastore:: getFileMap()
{
	return file_map;
}


//----test stub-----
#ifdef DATASTORE_TEST

void main(){
	Datastore dat;
	dat.addToPathSet("myfile/file1/dir/test1.h");
	dat.addToPathSet("myfile/file1/dir/test2.h");
	dat.addToPathSet("myfile/file1/dir/test3.h");


	for (auto i:dat.getPathSet()){
		std::cout<<i<<std::endl;
	}

}
#endif // DATASTORE_TEST

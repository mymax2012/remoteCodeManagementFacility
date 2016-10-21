///////////////////////////////////////////////////////////////////
// main.cpp:    excution package to demostrate all the			 //
//				requirements of project #2						 //
// ver 1.0													     //
// Application: for CSE687 project#2   demostrate requirements   //
// Platform:    Surface Pro 3 , Win 8.1 Pro, Visual Studio 2013  //
// Author:      Zhenya Li						                 //
//              zli52@syr.edu	                                 //
///////////////////////////////////////////////////////////////////

#include "XmlDocument.h"
#include <iostream>
using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement >;

#define _MAIN
#ifdef _MAIN
void main(int argc, char* argv[]){
	title("REQUIREMENT #3", '=');	
	XmlDocument doc1(XmlDocument::commendLine(argc,argv),true);
	//XmlDocument doc1("LectureNote.xml", true);
	std::cout << "imported file:" << XmlDocument::commendLine(argc, argv) << std::endl;
	std::cout << doc1.tostring() << std::endl;
	title("REQUIREMENT #4", '=');
	std::cout << "\nXmlDocument doc2(doc1)" << std::endl;
	XmlDocument doc2 = std::move(doc1);
	std::cout << "\nnow doc1 is:" << doc1.tostring() << std::endl;
	std::cout << "\nnow doc2 is:" << doc2.tostring() << std::endl;
	std::cout << "\ndoc1 = std::move(doc2)\n";
	doc1 = std::move(doc2);
	std::cout << "\nnow doc1 is:" << doc1.tostring() << std::endl;
	std::cout << "\nnow doc2 is:" << doc2.tostring() << std::endl;
	title("REQUIREMENT #5", '=');
	testRequirement5(doc1);
	title("REQUIREMENT #6", '=');
	testRequirement6(doc1);
	title("REQUIREMENT #7", '=');
	testRequirement7(doc1);
	title("REQUIREMENT #8", '=');
	testRequirement8(doc1);
	title("REQUIREMENT #9", '=');
	testRequirement9(doc1);
	title("REQUIREMENT #10", '=');
	testRequirement10();
}
#endif
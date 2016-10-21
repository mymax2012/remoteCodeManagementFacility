#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
///////////////////////////////////////////////////////////////////
// XmlDocument.h - a container of XmlElement nodes               //
// ver 1.3      Modified from Jim Fawcett's XmlDocument.h        //
// Application: for CSE687 project#2   XML elements definitions  //
// Platform:    Surface Pro 3 , Win 8.1 Pro, Visual Studio 2013  //
// Author:      Zhenya Li						                 //
//              zli52@syr.edu	                                 //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is a container with some managing functions of an XML file
*
* Abstract Syntax Trees, defined in this package, are unordered trees with 
* two types of nodes:
*   Terminal nodes     - nodes with no children
*   Non-Terminal nodes - nodes which may have a finite number of children
* They are often used to contain the results of parsing some language.
*
* The elements defined in the companion package, XmlElement, will be used in
* the AST defined in this package.  They are:
*   AbstractXmlElement - base for all the XML Element types
*   Element            - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   Comment            - XML element with comment markup and text
*   ProcInstruction    - XML element with markup and attributes but no children
*
*
*  Public Interface:
* -----------------
* using sPtr = std::shared_ptr < AbstractXmlElement > ;
*
* static std::string commendLine(int argc, char* argv[]);
*	read the commend line and get XML file's name.  
*
* XmlDocument(const std::string& src, bool isFile=0);
*	constructor which takes a string and bool value as input. string can be xml format string or file name.
*	this constructor can parse the string or file into a XML tree and store it inside pDocElement_
* 
* XmlDocument(sPtr root);
*	this constructor support adding a root node into an empty XML document file.
* 
* XmlDocument(XmlDocument&&);	
*	move constructor 
*
* XmlDocument& operator=(XmlDocument&&);
*	move assignment
*
* XmlDocument& element(const std::string& tag="");
*	search the tree with a tag for an element, store the result in found_, return itself so as to enable cascading.
*
* XmlDocument& elements(const std::string& tag="")
*	search the tree with a tag for all element, store the result in found_, return itself so as to enable cascading.
*
* XmlDocument& children(const std::string& tag = "")
*	search the children of prior found element with a tag, store the result in found_, return itself so as to enable cascading.
*
* XmlDocument& descendents(const std::string& tag = "")
*	search all the descendents of prior found element with a tag, store the result in found_, return itself so as to enable cascading.
*
* std::vector<sPtr>&& select(); 
*	return the found vector with searched results
*
* sPtr findEleById(const std::string & s, bool is_att_name=true);
*	given a string of attribute name or value, return the fist found element in the tree
*
* bool removeEleByTag(const std::string & tag = "");
*	remove an element by it's tag,(the first found one)
*
* bool removeEleByAttName(const std::string & attname);  
*	remove an element by attribute name(the first one found)
*
* bool removeEleByAttValue(const std::string & attvalue); 
*	remove an element by attribute value(the first one found)
*
* bool addChildByTag(const sPtr childnode,const std::string &tag = "");      
*	add child node to an element which can hold children
*
* bool addChildByAttName(const sPtr childnode, const std::string & attname);  
*	add an element by attribute name(the first one found)
*	
* bool addChildByAttValue(const sPtr childnode, const std::string & attvalue); 
*	add an element by attribute value(the first one found)
*
* std::vector<sPtr>& found(); 
*	return the found_ vector of searching results
*	
* std::string tostring();
*	return the whole XML tree as a string
*
* void writeToFile(const std::string & file_name);
*	output the whole XML tree as a string into a file
*
* void showSelected(std::vector<sPtr>& v)
*	print out all the nodes stored in the vector
*
* Required Files:
* ---------------
*   - XmlDocument.h, XmlDocument.cpp, 
*     XmlElement.h, XmlElement.cpp
*	  itokcollection.h, Tokenizer.h, Tokenizer.cpp
*	  xmlElementParts.h, xmlElementParts.cpp
*
* Build Process:
* --------------
*   devenv AST.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.3 Mar 18
* fixed bugs in parsing.
*
* ver 1.2 Mar 17 
* 1)Modified the constructor to take a bool value to consider the taken string a file name or a xml format string.
* 2)Write to file function added
* 3)Modified show function check first if the tree is empty
*
* ver 1.1 Mar 16
* 1)add and remove child functions and attribute functions
* 2)find element by ID function.
*
* ver 1.0 Mar 12
* 1)finished the constructor which could parse a string or file and construct the whole XML tree structure.
* 2)finished element(), elements(), children(), descendents(). (and their helper functions in private)
* 3)constructor taking a root element,  move constructor, move assignment
*
*
*/
#include <memory>
#include <string>
#include "XmlElement.h"

namespace XmlProcessing
{
  class XmlDocument
  {
  public:
    using sPtr = std::shared_ptr < AbstractXmlElement > ;

	static std::string commendLine(int argc, char* argv[]); //read commend line and get file name
    
    XmlDocument(const std::string& src, bool isFile=0); //read xml from file or string
	XmlDocument(sPtr);//support add an root to an empty xml document
	
	XmlDocument(XmlDocument&&);				      //move constructor
	XmlDocument& operator=(XmlDocument&&);		  //move assignment

	XmlDocument(XmlDocument&) = delete;				//disable copy constructor
	XmlDocument& operator=(XmlDocument&) = delete;   //disable copy assignment

    // queries return XmlDocument references so they can be chained, e.g., doc.element("foobar").descendents();
    XmlDocument& element(const std::string& tag="");     // found_[0] contains first element (DFS order) with tag
    XmlDocument& elements(const std::string& tag="");    // found_ contains all elements with tag
	XmlDocument& children(const std::string& tag = "");  // found_ contains sPtrs to children of prior found_[0] 
	XmlDocument& descendents(const std::string& tag = "");  // found_ contains sPtrs to descendents of prior found_[0]

	sPtr findEleById(const std::string & s, bool is_att_name=true); //find an element by it's attribute name/value

	bool removeEleByTag(const std::string & tag = "");    //remove an element by it's tag(the first one found)
	bool removeEleByAttName(const std::string & attname);  //remove an element by attribute name(the first one found)
	bool removeEleByAttValue(const std::string & attvalue); //remove an element by attribute value(the first one found)
	bool addChildByTag(const sPtr childnode,const std::string &tag = "");      //add child node to an element which can hold children
	bool addChildByAttName(const sPtr childnode, const std::string & attname);  //add an element by attribute name(the first one found)
	bool addChildByAttValue(const sPtr childnode, const std::string & attvalue); //add an element by attribute value(the first one found)
    
	std::vector<sPtr>& found();                       // return reference to found_	
	std::vector<sPtr>&& select();                   // return reference to found_.  Use std::move(found_) to clear found_
	
	std::string tostring();    //show the whole xml tree in xml format
	void writeToFile(const std::string & file_name);  //write the xml tree to a file
	static void showSelected(std::vector<sPtr>);  //show content of selected vector

  private:
	void push_father(sPtr);		//used to capture the father node when parsing xml file/string
	void pop_father();
	sPtr curr_father();			//get the current father node when parsing xml file/string
	void recursivePushFoundElement(sPtr father, const std::string&tag);//helper function recursively find the first element with tag
	void recursivePushFoundElements(sPtr father, const std::string&tag);//helper function for elements to recursively find all elments who has the tag
	void recursiveFoundEleByAtt(sPtr father, const std::string & s, bool is_att_name); //find element by attribute name/value

	sPtr pDocElement_;			// AST that holds procInstr, comments, XML root, and more comments
    std::vector<sPtr> found_;	// query results
	sPtr found_father;			//for remove or insert children
	std::vector<sPtr> father_ptrs; //father elements when transforming an xml file or string
  };

  void testRequirement5(XmlDocument&);
  void testRequirement6(XmlDocument&);
  void testRequirement7(XmlDocument&);
  void testRequirement7add(XmlDocument&);
  void testRequirement7emptyTree();
  void testRequirement7remove(XmlDocument&);
  void testRequirement8(XmlDocument&);
  void testRequirement9(XmlDocument&);
  void testRequirement10();
}
#endif

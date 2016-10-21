#ifndef XMLELEMENT_H
#define XMLELEMENT_H
///////////////////////////////////////////////////////////////////
// XmlElement.h - define XML Element types                       //
// ver 1.2 :Modified from Jim Fawcett's XmlElement.h ver 1.5     //
// Application: for CSE687 project#2   XML elements definitions  //
// Platform:    Surface Pro 3 , Win 8.1 Pro, Visual Studio 2013  //
// Author:      Zhenya Li						                 //
//              zli52@syr.edu		                             //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is used for CSE687 ood project#2 xml document processing. 
* This file classifies xml files into different types of elements.
*
* Abstract Syntax Trees are unordered trees with two types of nodes:
*   Terminal nodes     - nodes with no children
*   Non-Terminal nodes - nodes which may have a finite number of children
* They are often used to contain the results of parsing some language.
*
* The elements defined in this package will be used in the AST defined in
* the AbstractSyntaxTree package, included in this Visual Studio 2013
* solution.
*   AbstractXmlElement - base for all the XML Element types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   TaggedElement      - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   CommentElement     - XML element with comment markup and text
*   ProcInstrElement   - XML element with markup and attributes but no children
*   XmlDeclarElement   - XML declaration
*
*
* Public Interface:
* -----------------
* bool addChild(std::shared_ptr<AbstractXmlElement> pChild); 
*		add an element node to an element, if an element cannot have child, return false, if success return true.
*
* bool removeChild(std::shared_ptr<AbstractXmlElement> pChild);
*		remove an element node to an element, if an element don't have child or failed to remove, return false, if success return true.
*
* bool addAttrib(const std::string& name, const std::string& value);
*		add an attribute to an element, requires attribute's name and value by type of string. if failed return false, else return true.
*
* bool removeAttrib(const std::string& name);
*		remove an attribute from an element, requires attribute's name type of string. if failed return false, else return true.
*
* std::string value();
*		returns a string depends on different types of element. 
*		DocElement returns ""; 
*		TaggedElement returns it's tag; 
*		CommentElement returns comment;
*		TextElement returns it's text;
*		ProcInstrElment returns a string "xml processing instrcution";
*		XmlDecElement returns a string "xml declaration".
*
* std::string toString();
*		converts current element to a XML format string and return it.
*
* std::vector<std::shared_ptr<AbstractXmlElement>> children();
*		return the children elements of current element, if this element has no children return empty vector
*
* std::vector<std::pair<std::string,std::string>> getAttrib();
*		return attributes of current element in std::pair, if theirs no attribute return empty vector
*
*
* Required Files:
* ---------------
*   - XmlElement.h, XmlElement.cpp
*
* Build Process:
* --------------
*   devenv AST.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.2 Mar 18
* Modified DocElement's toString() function that set count=0 at the begining of the function.
*
* ver 1.1 Mar 12
* 1)Modified the DocElement's addchild() function to check if it has an XmlDecElement, when adding an XmlDecElement, 
* if it already had an XmlDecElement, don't add, if not, add it to the first children. 
* 2)Modified the taggedElement's addchild() function. a taggedElment cannot add a XmlDecElement(only DocElement can).
*
* ver 1.0 Mar 10
* Modified from Jim Fawcett's XmlElement.j ver 1.5
* 1)finished toString() function of each type of elemnt
* 2)Modified the ProcInstrElement's type value to "xml processing instruction"
* 3)added function virtual std::vector<std::shared_ptr<AbstractXmlElement>> children()
*	to each type of element to get their children nodes, if an element do not have any child return empty vector.
* 4)added function virtual std::vector<std::pair<std::string,std::string>> getAttrib()
*   to each type of element to get their attributes, if an element do not have any attribute return empty vector.
*
*/

#include <memory>
#include <string>
#include <vector>

namespace XmlProcessing
{
	/////////////////////////////////////////////////////////////////////////////
	// AbstractXmlElement - base class for all concrete element types

	class AbstractXmlElement
	{
	public:
		virtual bool addChild(std::shared_ptr<AbstractXmlElement> pChild);
		virtual bool removeChild(std::shared_ptr<AbstractXmlElement> pChild);
		virtual bool addAttrib(const std::string& name, const std::string& value);
		virtual bool removeAttrib(const std::string& name);
		virtual std::string value() = 0;
		virtual std::string toString() = 0;
		virtual std::vector<std::shared_ptr<AbstractXmlElement>> children() = 0;
		virtual std::vector<std::pair<std::string,std::string>> getAttrib() = 0;
		virtual ~AbstractXmlElement();
		
	protected:
		static size_t count;
		static size_t tabSize;
	};

	inline bool AbstractXmlElement::addChild(std::shared_ptr<AbstractXmlElement> pChild) { return false; }
	inline bool AbstractXmlElement::removeChild(std::shared_ptr<AbstractXmlElement> pChild) { return false; }
	inline bool AbstractXmlElement::addAttrib(const std::string& name, const std::string& value) { return false; }
	inline bool AbstractXmlElement::removeAttrib(const std::string& name) { return false; }
	inline AbstractXmlElement::~AbstractXmlElement() {}

	/////////////////////////////////////////////////////////////////////////////
	// DocElement - holds the document prologue, XML tree, and epilog

	class DocElement : public AbstractXmlElement
	{
	public:
		DocElement(std::shared_ptr<AbstractXmlElement> pRoot = nullptr);
		~DocElement() {}
		DocElement(const DocElement& doc) = delete;
		DocElement& operator=(const DocElement& doc) = delete;
		virtual bool addChild(std::shared_ptr<AbstractXmlElement> pChild);
		virtual bool removeChild(std::shared_ptr<AbstractXmlElement> pChild);
		virtual std::string value();
		virtual std::string toString();
		virtual std::vector<std::shared_ptr<AbstractXmlElement>> children();
		virtual std::vector<std::pair<std::string, std::string>> getAttrib();
		std::shared_ptr<AbstractXmlElement> getTagRoot();
	private:
		bool hasXmlRoot();
		std::vector<std::shared_ptr<AbstractXmlElement>> children_;
	};
	inline std::vector<std::shared_ptr<AbstractXmlElement>> DocElement::children(){ return children_; }
	inline std::vector<std::pair<std::string, std::string>> DocElement::getAttrib(){ std::vector<std::pair<std::string, std::string>> v; return v; }
	std::shared_ptr<AbstractXmlElement> makeDocElement(std::shared_ptr<AbstractXmlElement> pRoot = nullptr);

	/////////////////////////////////////////////////////////////////////////////
	// TextElement - represents the text part of an XML element

	class TextElement : public AbstractXmlElement
	{
	public:
		TextElement(const std::string& text) : text_(text) {}
		virtual ~TextElement() {}
		TextElement(const TextElement& te) = delete;
		TextElement& operator=(const TextElement& te) = delete;
		virtual std::string value();
		virtual std::string toString();
		virtual std::vector<std::shared_ptr<AbstractXmlElement>> children();
		virtual std::vector<std::pair<std::string, std::string>> getAttrib();
	private:
		std::string text_;
	};
	inline std::vector<std::shared_ptr<AbstractXmlElement>> TextElement::children(){ std::vector<std::shared_ptr<AbstractXmlElement>> v; return v; }
	inline std::vector<std::pair<std::string, std::string>> TextElement::getAttrib(){ std::vector<std::pair<std::string, std::string>> v; return v; }

	inline std::string TextElement::value() { return text_; }

	std::shared_ptr<AbstractXmlElement> makeTextElement(const std::string& text);

	/////////////////////////////////////////////////////////////////////////////
	// Element - represents a tagged element with attributes and child elements

	class TaggedElement : public AbstractXmlElement
	{
	public:
		TaggedElement(const std::string& tag) : tag_(tag) {};
		TaggedElement(const TaggedElement& te) = delete;
		virtual ~TaggedElement() {}
		TaggedElement& operator=(const TaggedElement& te) = delete;
		virtual bool addChild(std::shared_ptr<AbstractXmlElement> pChild);
		virtual bool removeChild(std::shared_ptr<AbstractXmlElement> pChild);
		virtual bool removeChild(const std::string& value);
		virtual bool addAttrib(const std::string& name, const std::string& value);
		virtual bool removeAttrib(const std::string& name);
		virtual std::string value();
		virtual std::string toString();
		virtual std::vector<std::shared_ptr<AbstractXmlElement>> children();
		virtual std::vector<std::pair<std::string, std::string>> getAttrib();
	private:
		std::string tag_;
		std::vector<std::shared_ptr<AbstractXmlElement>> children_;
		std::vector<std::pair<std::string, std::string>> attribs_;
	};

	std::shared_ptr<AbstractXmlElement> makeTaggedElement(const std::string& tag);
	inline std::vector<std::shared_ptr<AbstractXmlElement>> TaggedElement::children(){ return children_; }
	inline std::vector<std::pair<std::string, std::string>> TaggedElement::getAttrib(){ return attribs_; }


	/////////////////////////////////////////////////////////////////////////////
	// CommentElement - represents XML comments, e.g., <!-- comment text -->
	//
	// Incomplete

	class CommentElement : public AbstractXmlElement
	{
	public:
		CommentElement(const std::string& text) : commentText_(text) {}
		CommentElement(const CommentElement& ce) = delete;
		virtual ~CommentElement() {}
		CommentElement& operator=(const CommentElement& ce) = delete;
		virtual std::string value() { return commentText_; }
		virtual std::string toString();
		virtual std::vector<std::shared_ptr<AbstractXmlElement>> children();
		virtual std::vector<std::pair<std::string, std::string>> getAttrib();
	private:
		std::string commentText_ = "to be defined";
	};

	std::shared_ptr<AbstractXmlElement> makeCommentElement(const std::string& comment);
	inline std::vector<std::shared_ptr<AbstractXmlElement>> CommentElement::children(){ std::vector<std::shared_ptr<AbstractXmlElement>> v; return v; }
	inline std::vector<std::pair<std::string, std::string>> CommentElement::getAttrib(){ std::vector<std::pair<std::string, std::string>> v; return v; }

	/////////////////////////////////////////////////////////////////////////////
	// ProcInstrElement - represents XML Processing Instructions, e.g., <?xml version="1.0"?>
	//
	// Incomplete

	class ProcInstrElement : public AbstractXmlElement
	{
	public:
		ProcInstrElement(const ProcInstrElement& pe) = delete;
		ProcInstrElement(const std::string& type) : type_(type) {}
		ProcInstrElement& operator=(const ProcInstrElement& pe) = delete;
		virtual bool addAttrib(const std::string& name, const std::string& value);
		virtual bool removeAttrib(const std::string& name);
		virtual std::string value() { return type_; }
		virtual std::string toString();
		virtual std::vector<std::shared_ptr<AbstractXmlElement>> children();
		virtual std::vector<std::pair<std::string, std::string>> getAttrib();
	private:
		std::vector<std::pair<std::string, std::string>> attribs_;
		std::string type_ = "xml processing instruction";
		std::string text;
	};

	std::shared_ptr<AbstractXmlElement> makeProcInstrElement(const std::string& type);
	inline std::vector<std::shared_ptr<AbstractXmlElement>> ProcInstrElement::children(){ std::vector<std::shared_ptr<AbstractXmlElement>> v; return v; }
	inline std::vector<std::pair<std::string, std::string>> ProcInstrElement::getAttrib(){ return attribs_; }

	/////////////////////////////////////////////////////////////////////////////
	// XmlDeclarElement - <?xml version="1.0"?>

	class XmlDeclarElement : public AbstractXmlElement
	{
	public:
		XmlDeclarElement(const ProcInstrElement& pe) = delete;
		XmlDeclarElement() {}
		XmlDeclarElement& operator=(const ProcInstrElement& pe) = delete;
		virtual bool addAttrib(const std::string& name, const std::string& value);
		virtual bool removeAttrib(const std::string& name);
		virtual std::string value() { return ""; }
		virtual std::string toString();
		virtual std::vector<std::shared_ptr<AbstractXmlElement>> children();
		virtual std::vector<std::pair<std::string, std::string>> getAttrib();
	private:
		std::vector<std::pair<std::string, std::string>> attribs_;
		std::string type_ = "xml declaration";
	};

	std::shared_ptr<AbstractXmlElement> makeXmlDeclarElement();
	inline std::vector<std::shared_ptr<AbstractXmlElement>> XmlDeclarElement::children(){ std::vector<std::shared_ptr<AbstractXmlElement>> v; return v; }
	inline std::vector<std::pair<std::string, std::string>> XmlDeclarElement::getAttrib(){ return attribs_; }

	void title(const std::string& title, char underlineChar = '-');
}
#endif

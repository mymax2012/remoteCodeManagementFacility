///////////////////////////////////////////////////////////////////
// XmlElement.cpp - implementations of XML Element types         //
// ver 1.0 :													 //
//																 //
// Platform:    Surface Pro 3 , Win 8.1 Pro, Visual Studio 2013  //
// Author:      Zhenya Li						                 //
//              zli52@syr.edu		                             //
///////////////////////////////////////////////////////////////////

#include "XmlElement.h"
#include <iostream>

using namespace XmlProcessing;

size_t AbstractXmlElement::count = 0;
size_t AbstractXmlElement::tabSize = 2;

//----< factory for doc elements >----------------------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeDocElement(std::shared_ptr<AbstractXmlElement> pRoot)
{
	std::shared_ptr<AbstractXmlElement> ptr(new DocElement(pRoot));
	return ptr;
}

//----< factory for tagged elements >----------------------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeTaggedElement(const std::string& tag)
{
	std::shared_ptr<AbstractXmlElement> ptr(new TaggedElement(tag));
	return ptr;
}
//----< factory for text elements >------------------------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeTextElement(const std::string& text)
{
	std::shared_ptr<AbstractXmlElement> ptr(new TextElement(text));
	return ptr;
}
//----< factory for comment elements >---------------------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeCommentElement(const std::string& text)
{
	std::shared_ptr<AbstractXmlElement> ptr(new CommentElement(text));
	return ptr;
}
//----< factory for processing instruction elements >------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeProcInstrElement(const std::string& text)
{
	std::shared_ptr<AbstractXmlElement> ptr(new ProcInstrElement(text));
	return ptr;
}
//----< factory for XML Declaration elements >-------------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeXmlDeclarElement()
{
	std::shared_ptr<AbstractXmlElement> ptr(new XmlDeclarElement());
	return ptr;
}
//----< doc element constructor and destructor >-----------------------------

DocElement::DocElement(std::shared_ptr<AbstractXmlElement> pRoot)
{
	/*std::shared_ptr<AbstractXmlElement> pDecl = makeXmlDeclarElement();
	pDecl->addAttrib("version", "1.0");
	children_.push_back(pDecl);*/
	if (pRoot != nullptr)
		children_.push_back(pRoot);
}
//get tag root of the whole xml
std::shared_ptr<AbstractXmlElement> DocElement::getTagRoot(){
	for (auto pElement : children_)
	{
		if (dynamic_cast<TaggedElement*>(pElement.get()) != nullptr)
			return pElement;
	}
	return nullptr;
}

//----< private helper for DocElement::addChild(...) >-----------------------

bool DocElement::hasXmlRoot()
{
	for (auto pElement : children_)
	{
		if (dynamic_cast<TaggedElement*>(pElement.get()) != nullptr)
			return true;
	}
	return false;
}
//----< add only one child to doc element using pointer to child >-----------

bool DocElement::addChild(std::shared_ptr<AbstractXmlElement> pChild)
{
	
	// don't add TextElements
	TextElement* pTxEl = dynamic_cast<TextElement*>(pChild.get());
	if (pTxEl != 0)  // don't add text elements to DocElement
		return false;
	
	// don't add another DocElement
	DocElement* pDcEl = dynamic_cast<DocElement*>(pChild.get());
	if (pDcEl != 0)  // don't add text elements to DocElement
		return false;
	
	// if add declaration, add just once and put to the first children
	XmlDeclarElement* pdec = dynamic_cast<XmlDeclarElement*>(pChild.get());
	if (pdec != 0){
		if (children_.size() == 0){
			children_.push_back(pChild);
			return true;
		}
		
		XmlDeclarElement* p = dynamic_cast<XmlDeclarElement*>(children_[0].get());		
		if (p != 0)
			return false;
		else{
			
			std::vector<std::shared_ptr<AbstractXmlElement>> vec;
			vec.push_back(pChild);
			for (auto ele : children_)
				vec.push_back(ele);
			
			children_.clear();
			children_ = vec;
			return true;
		}
	}
	// add ProcInstrElements and CommentElements 
	TaggedElement* te = dynamic_cast<TaggedElement*>(pChild.get());
	if (te == nullptr) // is not a TaggedElement
	{
		children_.push_back(pChild);
		return true;
	}

	// add only one TaggedElement
	if (!hasXmlRoot())
	{
		children_.push_back(pChild);
		return true;
	}
	return false;
}
//----< remove child from doc element using pointer to child >---------------

bool DocElement::removeChild(std::shared_ptr<AbstractXmlElement> pChild)
{
	auto iter = std::find(begin(children_), end(children_), pChild);
	if (iter != end(children_))
	{
		children_.erase(iter);
		return true;
	}
	return false;
}
//----< return value = tag from doc element >-----------------------------

std::string DocElement::value() { return std::string(""); }

//----< generate xml string for tagged element >-----------------------------

std::string DocElement::toString()
{
	if (children_.size() == 0) std::cout<<"\nThe tree is empty\n";
	count = 0;
	std::string rtn;
	for (auto elem : children_)
		rtn += elem->toString();
	return rtn;
}


//----< add child to tagged element using pointer to child >-----------------

bool TaggedElement::addChild(std::shared_ptr<AbstractXmlElement> pChild)
{
	XmlDeclarElement* p = dynamic_cast<XmlDeclarElement*>(pChild.get());
	if (p != 0) return false;
	else children_.push_back(pChild);
	return true;
}
//----< remove child from tagged element using pointer to child >------------

bool TaggedElement::removeChild(std::shared_ptr<AbstractXmlElement> pChild)
{
	auto iter = std::find(begin(children_), end(children_), pChild);
	if (iter != end(children_))
	{
		children_.erase(iter);
		return true;
	}
	return false;
}
//----< remove child from tagged element using child's value >---------------

bool TaggedElement::removeChild(const std::string& value)
{
	for (auto iter = begin(children_); iter != end(children_); ++iter)
	{
		if ((*iter)->value() == value)
		{
			children_.erase(iter);
			return true;
		}
	}
	return false;
}
//----< add attribute to tagged element >------------------------------------

bool TaggedElement::addAttrib(const std::string& name, const std::string& value)
{
	std::pair<std::string, std::string> atPair(name, value);
	attribs_.push_back(atPair);
	return true;
}
//----< remove attribute from tagged element >-------------------------------

bool TaggedElement::removeAttrib(const std::string& name)
{
	for (auto iter = begin(attribs_); iter != end(attribs_); ++iter)
	{
		if (iter->first == name)
		{
			attribs_.erase(iter);
			return true;
		}
	}
	return false;
}
//----< return value = tag from tagged element >-----------------------------

std::string TaggedElement::value() { return tag_; }

//----< generate xml string for tagged element >-----------------------------

std::string TaggedElement::toString()
{
	std::string spacer(tabSize*++count, ' ');
	std::string xml = "\n" + spacer + "<" + tag_;
	for (auto at : attribs_)
	{
		xml += " ";
		xml += at.first;
		xml += "=\"";
		xml += at.second;
		xml += "\"";
	}
	xml += ">";
	for (auto pChild : children_)
		xml += pChild->toString();
	xml += "\n" + spacer + "</" + tag_ + ">";
	--count;

	return xml;
}
//----< generate xml string for text element >-------------------------------

std::string TextElement::toString()
{
	std::string spacer(tabSize * ++count, ' ');
	std::string xml = "\n" + spacer + text_;
	--count;

	return xml;
}
//----< generate xml string for comment element >-------------------------------

std::string CommentElement::toString(){
	std::string spacer(tabSize * ++count, ' ');
	std::string xml = "\n" + spacer + "<!-- " + commentText_ + " -->";
	return xml;
}
//----< generate xml string for processing instruction element >-------------------------------

std::string ProcInstrElement::toString(){
	std::string spacer(tabSize * ++count, ' ');
	std::string xml = "\n" + spacer + "<?"+type_.c_str();
	for (auto a: attribs_){
		xml += " ";
		xml += a.first;
		xml += "=\"";
		xml += a.second;
		xml += "\"";
	}
	xml+="?>";
	--count;

	return xml;
}
//----< add attribute to ProcInstElement >-----------------------------------

bool ProcInstrElement::addAttrib(const std::string& name, const std::string& value)
{
	std::pair<std::string, std::string> atPair(name, value);
	attribs_.push_back(atPair);
	return true;
}
//----< remove attribute from ProcInstElement >------------------------------

bool ProcInstrElement::removeAttrib(const std::string& name)
{
	for (auto iter = begin(attribs_); iter != end(attribs_); ++iter)
	{
		if (iter->first == name)
		{
			attribs_.erase(iter);
			return true;
		}
	}
	return false;
}
//----< generate xml string for text element >-------------------------------

std::string XmlDeclarElement::toString()
{
	std::string spacer(tabSize * ++count, ' ');
	std::string xml = "\n" + spacer + "<?xml";
	for (auto at : attribs_)
	{
		xml += " ";
		xml += at.first;
		xml += "=\"";
		xml += at.second;
		xml += "\"";
	}
	xml += "?>";
	--count;
	return xml;
}
//----< add attribute to ProcInstElement >-----------------------------------

bool XmlDeclarElement::addAttrib(const std::string& name, const std::string& value)
{
	std::pair<std::string, std::string> atPair(name, value);
	attribs_.push_back(atPair);
	return true;
}
//----< remove attribute from ProcInstElement >------------------------------

bool XmlDeclarElement::removeAttrib(const std::string& name)
{
	for (auto iter = begin(attribs_); iter != end(attribs_); ++iter)
	{
		if (iter->first == name)
		{
			attribs_.erase(iter);
			return true;
		}
	}
	return false;
}
//----< helper function displays titles >------------------------------------

void XmlProcessing::title(const std::string& title, char underlineChar)
{
	std::cout << "\n  " << title;
	std::cout << "\n " << std::string(title.size() + 2, underlineChar);
}

//----< test stub >----------------------------------------------------------
//#define TEST_XMLELEMENT
#ifdef TEST_XMLELEMENT

int main()
{
	title("Testing XmlElement Package", '=');
	std::cout << "\n";

	title("Testing XmlElement manipulations", '=');
	std::cout << "\n";
	using sPtr = std::shared_ptr < AbstractXmlElement >;

	sPtr docEl = makeDocElement();

	std::cout << "\n create a root taggedelement";
	sPtr root = makeTaggedElement("root");
	docEl->addChild(root);
	std::cout << docEl->toString() << std::endl;

	std::cout << "\n add a text ele to root";
	docEl->addChild(makeTextElement("this is a test"));
	std::cout << docEl->toString() << std::endl;

	std::cout << "\n add a ProcInstrElement";
	sPtr prochild = makeProcInstrElement("type A");
	docEl->addChild(prochild);
	std::cout << docEl->toString() << std::endl;
	
	std::cout << "\n add attributes to the ProcInstrElement";
	prochild->addAttrib("att1", "attvalue");
	prochild->addAttrib("att2", "attvalue");	
	std::cout << root->toString() << std::endl;

	std::cout << "\n add a commentElement";
	sPtr com = makeCommentElement("comment");
	docEl->addChild(com);
	std::cout << docEl->toString() << std::endl;

	std::cout << "\n add a XmlDecElement with one attribute";
	sPtr dec = makeXmlDeclarElement();
	dec->addAttrib("version", "1.0");
	docEl->addChild(dec);
	std::cout << docEl->toString() << std::endl;

	std::cout << "\n add a TaggedElement to root tag element";
	sPtr tagele = makeTaggedElement("tagEle");
	root->addChild(tagele);
	std::cout << docEl->toString() << std::endl;

	std::cout << "\n remove a TaggedElement from root tag element";
	root->removeChild(tagele);
	std::cout << docEl->toString() << std::endl;

	std::cout << "\n remove an attribute from ProcInstrElement";
	prochild->removeAttrib("att1");
	std::cout << docEl->toString() << std::endl;

	title("Testing mistaken manipulations", '=');
	std::cout << "\n";

	std::cout << "\n add a TaggedElement to comment element(which should not have a child)" << std::endl;
	if (com->addChild(makeTaggedElement("errorTag")))
		std::cout << "adding child to comment element success!" << std::endl;
	else std::cout << "adding child to comment element failed!" << std::endl;

	std::cout << "\n add an attribute to comment element(which should not have an attribute)" << std::endl;
	if (com->addAttrib("errAtt","errAttValue"))
		std::cout << "adding attribute to comment element success!" << std::endl;
	else std::cout << "adding attribute to comment element failed!" << std::endl;

	std::cout << "\n add an XmlDecElement to the tagged root element which can have children" << std::endl;
	if (root->addChild(makeXmlDeclarElement()))
		std::cout<<"adding XmlDecElement to tagged root element success!"<<std::endl;
	else std::cout << "adding XmlDecElement to tagged root element failed!" << std::endl;

	std::cout << "\n remove the XmlDecElement" << std::endl;
	if (docEl->removeChild(dec))
		std::cout << "XmlDecElement removed!" << std::endl;
	else std::cout << "XmlDecElement removed failed!" << std::endl;
	std::cout << docEl->toString() << std::endl;

	std::cout << "\n redo adding an XmlDecElement" << std::endl;
	if (docEl->addChild(makeXmlDeclarElement()))
		std::cout << "adding XmlDecElement to tagged root element success!" << std::endl;
	else std::cout << "adding XmlDecElement to tagged root element failed!" << std::endl;
	std::cout << docEl->toString() << std::endl;
	std::cout << "\n\n";
}

#endif




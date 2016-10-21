///////////////////////////////////////////////////////////////////
// XmlDocument.cpp - a container of XmlElement nodes             //
// ver 1.3      Modified from Jim Fawcett's XmlDocument.h        //
//	XML elements definitions									 //
// Platform:    Surface Pro 3 , Win 8.1 Pro, Visual Studio 2013  //
// Author:      Zhenya Li						                 //
//              zli52@syr.edu	                                 //
///////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include "XmlDocument.h"
#include "xmlElementParts.h"
using namespace std;
using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement >;

//constructor reading a string or file to construct a xml tree
XmlProcessing::XmlDocument::XmlDocument(const std::string& src, bool isFile)
{
	Toker toker(src, isFile);
	toker.setMode(Toker::xml);
	XmlParts parts(&toker);
	pDocElement_ = makeDocElement();
	father_ptrs.push_back(pDocElement_);
	sPtr father;
	while (parts.get()){
		if (parts[0] == "<"&&parts[1]=="?"){ //can only be children of DocElment
			father = curr_father();
			if (parts[2] == "xml" && (isalpha(parts[3][0]) || parts[3][0]=='_')){ //when name is xml it's declaration
				std::shared_ptr<AbstractXmlElement> decptr = makeXmlDeclarElement();
				for (auto p : parts.extractAtbs())
					decptr->addAttrib(p.first, p.second);
				father->addChild(decptr);
			}
			else{
				std::string text = parts[2];
				if (parts[3] == "_") text += (parts[3] + parts[4]);
				std::shared_ptr<AbstractXmlElement> preptr = makeProcInstrElement(text);
				for (auto p : parts.extractAtbs())
					preptr->addAttrib(p.first, p.second);
				father->addChild(preptr);
			}			
		}
		else if (parts[0] == "<" && parts[1] == "!" && parts[2] == "--"){ //comment element
			father = curr_father();			
			sPtr comptr = makeCommentElement(parts.extractComment());
			father->addChild(comptr);
		}
		else if (parts[0] != "<"){  //text element
			father = curr_father();
			sPtr texptr = makeTextElement(parts.extractText());
			father->addChild(texptr);
		}
		else if (parts[0] == "<"&& parts[1] != "/") {  //tagged ele
			father = curr_father();
			sPtr tagptr = makeTaggedElement(parts[1]);
			for (auto p : parts.extractAtbs())
				tagptr->addAttrib(p.first, p.second);
			father->addChild(tagptr);
			if (parts[parts.length() - 2] != "/")       //when it's not self-colsed
				push_father(tagptr);
		}
		else if (parts[0] == "<" && parts[1] == "/") //end tag
			pop_father();		
	}
}
//constructor adding an root pointer to the empty xml tree
XmlProcessing::XmlDocument::XmlDocument(sPtr p){
	DocElement* rootP = dynamic_cast<DocElement*>(p.get());
	if (rootP != nullptr){
		pDocElement_ = p;
	}
}
//move constructor
XmlProcessing::XmlDocument::XmlDocument(XmlDocument&& xmldoc): pDocElement_(xmldoc.pDocElement_){
	xmldoc.pDocElement_ = nullptr;
	std::cout << "XmlDocument move ctor called!"<<std::endl;
}
//move asssignment
XmlDocument& XmlProcessing::XmlDocument::operator=(XmlDocument&& xmldoc){
	pDocElement_ = xmldoc.pDocElement_;
	xmldoc.pDocElement_ = nullptr;
	std::cout << "XmlDocument move assignment called!" << std::endl;
	return *this;
}

void XmlProcessing::XmlDocument::push_father(sPtr ptr){
	father_ptrs.push_back(ptr);
}

sPtr XmlProcessing::XmlDocument::curr_father(){
	if (father_ptrs.size() > 0)
		return father_ptrs[father_ptrs.size() - 1];
	else return nullptr;
}

void XmlProcessing::XmlDocument::pop_father(){
	if (father_ptrs.size() > 1){
		father_ptrs.pop_back();
	}
}

//output the whole xml file

std::string XmlProcessing::XmlDocument::tostring(){
	if (pDocElement_!=0)
		return pDocElement_->toString();
	else return "\n\n this xml tree is empty";
}

XmlDocument & XmlProcessing::XmlDocument::element(const std::string& tag){
	found_.clear();
	recursivePushFoundElement(pDocElement_, tag);
	return *this;
}

//helper function recursively find the first element with tag

void XmlProcessing::XmlDocument::recursivePushFoundElement(sPtr father, const std::string&tag){
	if (tag == ""){
		found_.push_back(father->children()[0]);
		found_father = father;
		return;
	}
	else if (father->value().compare(tag) == 0){
		found_.push_back(father);		
		return;
	}
	else if (father->children().size() != 0){
		for (auto ele : father->children()){
			if (found_.size() != 0)return;
			else {
				found_father = father;
				recursivePushFoundElement(ele, tag);
			}
		}
	}
	else return;
}

XmlDocument & XmlProcessing::XmlDocument::elements(const std::string& tag){
	found_.clear();
	recursivePushFoundElements(pDocElement_, tag);
	return *this;
}

//helper function for elements to recursively find all elments who has the tag
void XmlProcessing::XmlDocument::recursivePushFoundElements(sPtr father, const std::string&tag){
	if (tag == ""){
		found_=father->children();
	}
	else{
		if (father->value().compare(tag) == 0){
			found_.push_back(father);
		}
		if (father->children().size() != 0){
			for (auto ele : father->children()){
				recursivePushFoundElements(ele, tag);
			}
		}
	}
}

XmlDocument & XmlProcessing::XmlDocument::children(const std::string& tag){
	if (found_.size() != 0){
		sPtr father = found_[0];
		found_.clear();
		recursivePushFoundElement(father, tag);
	}
	return *this;
}

XmlDocument & XmlProcessing::XmlDocument::descendents(const std::string& tag){
	if (found_.size() != 0){
		sPtr father = found_[0];
		found_.clear();
		recursivePushFoundElements(father, tag);
	}
	return *this;
}

sPtr XmlProcessing::XmlDocument::findEleById(const std::string & s, bool is_att_name){
	found_.clear();
	recursiveFoundEleByAtt(pDocElement_, s, is_att_name);
	if (found_.size() != 0)
		return found_[0];
	else return nullptr;
}

bool XmlProcessing::XmlDocument::removeEleByTag(const std::string & tag){
	found_.clear();
	element(tag);
	if (found_.size() == 0)
		return false;
	else{
		return found_father->removeChild(found_[0]);	
	}
}

bool XmlProcessing::XmlDocument::removeEleByAttName(const std::string & attname){
	found_.clear();
	found_father = nullptr;
	recursiveFoundEleByAtt(pDocElement_, attname, true);
	if (found_.size() == 0)
		return false;
	else return found_father->removeChild(found_[0]);
}

bool XmlProcessing::XmlDocument::removeEleByAttValue(const std::string & attvalue){
	found_.clear();
	found_father = nullptr;
	recursiveFoundEleByAtt(pDocElement_, attvalue, false);
	if (found_.size() == 0)
		return false;
	else return found_father->removeChild(found_[0]);
}

bool XmlProcessing::XmlDocument::addChildByTag(const sPtr childnode,const std::string &tag){
	if (tag == "")
		return pDocElement_->addChild(childnode);
	else{
		return element(tag).select()[0]->addChild(childnode);
	}
}

bool XmlProcessing::XmlDocument::addChildByAttName(const sPtr childnode, const std::string & attname){
	found_.clear();
	recursiveFoundEleByAtt(pDocElement_, attname, true);
	if (found_.size() == 0)
		return false;
	else return found_[0]->addChild(childnode);
}

bool XmlProcessing::XmlDocument::addChildByAttValue(const sPtr childnode, const std::string & attvalue){
	found_.clear();
	recursiveFoundEleByAtt(pDocElement_, attvalue, false);
	if (found_.size() == 0)
		return false;
	else return found_[0]->addChild(childnode);
}

void XmlProcessing::XmlDocument::recursiveFoundEleByAtt(sPtr father, const std::string & s, bool is_att_name){
	if (is_att_name){
		if (father->getAttrib().size() != 0){
			for (auto att : father->getAttrib()){
				if (att.first.compare(s) == 0){
					found_.push_back(father);
					return;
				}
			}
		}
		if (father->children().size() != 0){
			for (auto ele : father->children()){
				if (found_.size()!= 0)return;
				else{
					found_father = father;
					recursiveFoundEleByAtt(ele, s, is_att_name);
				}
			}
		}
		return;
	}
	else{
		if (father->getAttrib().size() != 0){
			for (auto att : father->getAttrib()){
				if (att.second.compare(s) == 0){
					found_.push_back(father);
					return;
				}
			}
		}
		if (father->children().size() != 0){
			for (auto ele : father->children()){
				if (found_.size() != 0)return;
				else{
					found_father = father;
					recursiveFoundEleByAtt(ele, s, is_att_name);
				}
			}
		}
		return;
	}
}

std::vector<sPtr>&& XmlProcessing::XmlDocument::select(){
	return move(found_);
}

void XmlProcessing::XmlDocument::writeToFile(const std::string & file_name){
	std::ofstream out(file_name);
	if (pDocElement_ == nullptr)return;
	out << pDocElement_->toString();
	out.close();
}

void XmlProcessing::XmlDocument::showSelected(std::vector<sPtr> v){
	if (v.size() == 0)
		std::cout << "\nCan't find any element!\n";
	else{
		for (auto ele : v)
			std::cout <<"\n selected node is:"<< ele->toString()<<"\n";
	}
}

std::string XmlProcessing::XmlDocument::commendLine(int argc, char* argv[]){
	if (argc == 2)return argv[1];
	else {
		std::cout << "\nUsage: please input ONE file name you want to process.\n";
		exit(0);
	}
}

void XmlProcessing::testRequirement5(XmlDocument& doc){
	std::string s;
	int i;
	bool b;
	sPtr p;
	title("Testing search element from attribute name/value.", '-');
	while (true){
		std::cout << "\nInput '1' to search by attribute name, '0' by attribute value.'-1' TO END TEST\n";
		while ((!(cin >> i)) || i > 1 || i < -1) {
			cout << "Incorrect input. Please try again.\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		if (i == -1)break;
		else if (i == 0)b = false;
		else b = true;
		
		std::cout << "\nInput attribute "<<( b? "name":"value" )<<" of element you want to search: ";
		std::cin >> s;
		p=doc.findEleById(s,b);

		if(p!=0){
			std::cout<<"\nThe element found is: "<<p->toString();
		}
		else std::cout<<"\nCannot find the element.";
	}
}
void XmlProcessing::testRequirement6(XmlDocument& doc){
	std::string s;
	title("Testing search element by tag(ONLY THE FIRST FOUND ELEMENT)",'-');
	while (true){
		std::cout << "\nInput tag name to search or \"-1\" TO END TEST\n";
		cin >> s;
		if (s == "-1") break;
		else{
			if (doc.element(s).select().size() == 0)
				std::cout << "\nCannot find any elements.";
			else{
				for (auto ele : doc.element(s).select())
					std::cout << "\nElement found:" << ele->toString();
			}
		}
	}

	title("Testing search elements by tag(ALL ELEMENTS)", '-');
	while (true){
		std::cout << "\nInput tag name to search or \"-1\" TO END TEST\n";
		cin >> s;
		if(s=="-1") break;
		else{
			if(doc.elements(s).select().size()==0)
				std::cout<<"\nCannot find any elements.";
			else{
				for (auto ele : doc.elements(s).select())
					std::cout << "\nElement found:" << ele->toString();
			}
		}
	}
}
void XmlProcessing::testRequirement7(XmlDocument& doc){
	testRequirement7add(doc);
	testRequirement7emptyTree();
	testRequirement7remove(doc);
}
void XmlProcessing::testRequirement7add(XmlDocument& doc){
	std::string s;
	sPtr tag_c = makeTaggedElement("addedTagged");
	std::cout << "\nCreated tag_c = makeTaggedElement(\"addedTagged\")";

	title("Testing add child by tag searched element", '-');
	
	while (true){
		std::cout << "\nInput tag name to add a child or \"-1\" TO END TEST\n";
		cin >> s;
		if (s == "-1") break;
		else
			std::cout<<(doc.addChildByTag(tag_c,s)?"\nAdding child success!"+doc.tostring():"\nAdding child failed!");
	}

	title("Testing add child by attribute name searched element", '-');
	while (true){
		std::cout << "\nInput attribute name to add a child or \"-1\" TO END TEST\n";
		cin >> s;
		if (s == "-1") break;
		else
			std::cout<<(doc.addChildByAttName(tag_c,s)?"\nAdding child success!"+doc.tostring():"\nAdding child failed!");
	}

	title("Testing add child by attribute value searched element", '-');
	while (true){
		std::cout << "\nInput attribute value to add a child or \"-1\" TO END TEST\n";
		cin >> s;
		if (s == "-1") break;
		else
			std::cout<<(doc.addChildByAttValue(tag_c,s)?"\nAdding child success!"+doc.tostring():"\nAdding child failed!");
	}
}
void XmlProcessing::testRequirement7emptyTree(){
	title("Testing adding root to empty tree", '-');
	sPtr docele = makeDocElement();
	std::cout << "\nCreated docele = makeDocElement();";
	XmlDocument doc(docele);
	std::cout << "\nRoot docele added to the tree by XmlDocument doc(docele);";
	std::cout <<doc.tostring();
}

void XmlProcessing::testRequirement7remove(XmlDocument& doc){
	std::string s;
	title("Testing removing element by tag searched element", '-');
	while (true){
		std::cout << "\nInput tag name to add a child or \"-1\" TO END TEST\n";
		cin >> s;
		if (s == "-1") break;
		else
			std::cout << (doc.removeEleByTag(s) ? "\nRemoving child success!" + doc.tostring() : "\nRemoving child failed!");
	}

	title("Testing removing element by attribute name searched element", '-');
	while (true){
		std::cout << "\nInput attribute name to add a child or \"-1\" TO END TEST\n";
		cin >> s;
		if (s == "-1") break;
		else
			std::cout << (doc.removeEleByAttName(s) ? "\nRemoving child success!" + doc.tostring() : "\nRemoving child failed!");
	}

	title("Testing removing element by attribute value searched element", '-');
	while (true){
		std::cout << "\nInput attribute value to add a child or \"-1\" TO END TEST\n";
		cin >> s;
		if (s == "-1") break;
		else
			std::cout << (doc.removeEleByAttValue(s) ? "\nRemoving child success!" + doc.tostring() : "\nRemoving child failed!");
	}
}
void XmlProcessing::testRequirement8(XmlDocument& doc) {
	std::string s;
	title("Testing getting attributes of an elment", '-');
	while (true){
		std::cout << "\nInput tag name to search or \"-1\" TO END TEST\n";
		cin >> s;
		if (s == "-1") break;
		else{
			if (doc.element(s).select().size() == 0)
				std::cout << "\nCannot find any elements.";
			else{
				std::cout << "\nThe attributes of the found element are:\n";
				for (auto pairs : doc.element(s).select()[0]->getAttrib()){
					std::cout << pairs.first << "=\"" << pairs.second << "\"\n";
				}
			}
		}
	}

	title("Testing getting children of an elment", '-');
	while (true){
		std::cout << "\nInput tag name to search or \"-1\" TO END TEST\n";
		cin >> s;
		if (s == "-1") break;
		else{
			if (doc.element(s).select().size() == 0)
				std::cout << "\nCannot find any elements.";
			else{
				std::cout << "\nThe children of the found element are:\n";
				for (auto ele : doc.element(s).select()[0]->children())
					std::cout<<ele->toString();
			}
		}
	}
}
void XmlProcessing::testRequirement9(XmlDocument& doc){
	std::string s,name,value;
	title("Testing adding attributes of an elment", '-');
	while (true){
		std::cout << "\nInput tag name to get the element or \"-1\" TO END TEST\n";
		cin >> s;
		if (s == "-1") break;
		else{
			if (doc.element(s).select().size() == 0)
				std::cout << "\nCannot find any elements.";
			else{
				std::cout << "\nInput attribute name to add.\n";
				cin >> name;
				std::cout << "\nInput attribute value to add.\n";
				cin >> value;
				doc.element(s).select()[0]->addAttrib(name,value);
				std::cout << "\nNow the XML file is:" << doc.tostring();
			}
		}
	}

	title("Testing removing attributes of an elment", '-');
	while (true){
		std::cout << "\nInput tag name to get the element or \"-1\" TO END TEST\n";
		cin >> s;
		if (s == "-1") break;
		else{
			if (doc.element(s).select().size() == 0)
				std::cout << "\nCannot find any elements.";
			else{
				std::cout << "\nInput attribute name to remove.\n";
				cin >> name;
				doc.element(s).select()[0]->removeAttrib(name);
				std::cout << "\nNow the XML file is:" << doc.tostring();
			}
		}
	}
}
void XmlProcessing::testRequirement10(){
	std::cout << "READING a file should already demostrated by above requirements.\n";
	title("\nTesting reading XML from string",'-');
	std::string s="<root><child1>child1 body</child1><child2><grandchild21>gc body</grandchild21>some text</child2></root>";
	std::cout << "\nstring is: \n" << s << std::endl;
	XmlDocument doc(s, false);
	std::cout << "\nParse string to tree:"<< std::endl;
	std::cout << doc.tostring();

	title("\nTesting writing XML to file",'-');
	std::string fname;
	std::cout<<"\ninput the file name you want to write: ";
	cin>>fname;
	doc.writeToFile(fname);
}
//#define TEST_XMLDOCUMENT
#ifdef TEST_XMLDOCUMENT
int main()
{
	title("Testing XmlDocument class", '=');
	title("testing parsing from file", '-');
	std::cout << "\n file name is LectureNote.xml" << std::endl;
	std::cout << "\n XmlDocument doc1(LectureNote.xml,true)" << std::endl;
	XmlDocument doc1("LectureNote.xml", true);
	std::cout << "print the parsed xml file:" << std::endl;
	cout << doc1.tostring();

	title("testing move constructor", '-');
	std::cout << "\n XmlDocument doc2(doc1)" << std::endl;
	XmlDocument doc2 = std::move(doc1);
	std::cout << "\n now doc1 is:" << doc1.tostring() << std::endl;
	std::cout << "\n now doc2 is:" << doc2.tostring() << std::endl;
	std::cout << "\n\n";

	title("testing move assignment", '-');
	std::cout << "\n doc1 = std::move(doc2)\n";
	doc1 = std::move(doc2);
	std::cout << "\n now doc1 is:" << doc1.tostring() << std::endl;
	std::cout << "\n now doc2 is:" << doc2.tostring() << std::endl;
	std::cout << "\n\n";

	title("testing --XmlDocument& element(const std::string& tag)--", '-');
	std::cout << "\nXmlDocument::showSelected(doc1.element(\"title\").select())\n";
	XmlDocument::showSelected(doc1.element("title").select());

	title("testing --XmlDocument& elements(const std::string& tag)--", '-');
	std::cout << "\nXmlDocument::showSelected(doc1.elements(\"title\").select())\n";
	XmlDocument::showSelected(doc1.elements("title").select());

	title("testing--XmlDocument& children(const std::string& tag)--", '-');
	std::cout << "\nXmlDocument::showSelected(doc1.element(\"LectureNote\").children(\"title\").select())\n";
	XmlDocument::showSelected(doc1.element("LectureNote").children("title").select());

	title("testing--XmlDocument& descendents(const std::string& tag)--", '-');
	std::cout << "\nXmlDocument::showSelected(doc1.element(\"LectureNote\").descendents(\"title\").select())\n";
	XmlDocument::showSelected(doc1.element("LectureNote").descendents("title").select());

	title("testing--sPtr findEleById(const std::string & s, bool is_att_name)--", '-');
	std::cout << "\nFind element by attribute name \"autherAtt1\"\n";
	std::cout << doc1.findEleById("autherAtt1", true)->toString();

	std::cout << "\nFind element by attribute value \"att2\"\n";
	std::cout << doc1.findEleById("att2", false)->toString();

	std::cout<<"\nTesting--addChildByTag(const sPtr childnode,const std::string &tag)--";
	std::cout << "\ntesting--addChildByAttName(const sPtr childnode, const std::string & attname)--";
	std::cout << "\ntesting--addChildByAttValue(const sPtr childnode, const std::string & attvalue)--";
	sPtr com_c = makeCommentElement("addedComment");
	sPtr pro_c = makeProcInstrElement("addedProIns");
	sPtr tex_c = makeTextElement("addedText");
	sPtr tag_c = makeTaggedElement("addedTagged");
	sPtr dec_c = makeXmlDeclarElement();

	std::cout << "\nCreated com_c = makeCommentElement(\"addedComment\")";
	std::cout << "\nCreated pro_c = makeProcInstrElement(\"addedProIns\")";
	std::cout << "\nCreated tex_c = makeTextElement(\"addedText\")";
	std::cout << "\nCreated tag_c = makeTaggedElement(\"addedTagged\")";
	std::cout << "\nCreated dec_c = makeXmlDeclarElement()";
	std::cout << "\n\nAdding processing instruction to xml file doc1.addChildByTag(pro_c,\"\")";
	doc1.addChildByTag(pro_c, "");
	std::cout << doc1.tostring();

	std::cout << "\n\nAdding comment to reference node doc1.addChildByAttName(com_c,\"refAtt1\")";
	doc1.addChildByAttName(com_c, "refAtt1");
	std::cout << doc1.tostring();

	std::cout << "\n\nAdding tagged to reference node doc1.addChildByAttValue(tag_c,\"att2\")";
	doc1.addChildByAttValue(tag_c, "att2");
	std::cout << doc1.tostring();

	std::cout << "\n\nTesting--removeEleByTag(const std::string & tag)--";
	std::cout << "\ntesting--removeEleByAttName(const std::string & attname)--";
	std::cout << "\ntesting--removeEleByAttValue(const std::string & attvalue)--";

	std::cout << "\n\nremoving addedTagged doc1.removeEleByTag(\"addedTagged\")";
	doc1.removeEleByTag("addedTagged");
	std::cout << doc1.tostring();

	std::cout << "\n\nremoving element contains ID 'autherAtt1' doc1.removeEleByAttName(\"autherAtt1\")";
	doc1.removeEleByAttName("autherAtt1");
	std::cout << doc1.tostring();
	
	std::cout << "\n\nremoving element contains attribute value 'ref2' doc1.removeEleByAttValue(\"ref2\")";
	doc1.removeEleByAttValue("ref2");
	std::cout << doc1.tostring();
}
#endif
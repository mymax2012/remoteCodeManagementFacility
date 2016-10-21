/////////////////////////////////////////////////////////////////////
//  XmlElementParts.cpp - Collects tokens for XML analysis         //
//                                                                 //
// ver 1.0     Modified from Jim Fawcett's XmlElementParts.h ver1.3//
// Application: for CSE687 project#2   XML elements parts		   //
// Platform:    Surface Pro 3 , Win 8.1 Pro, Visual Studio 2013    //
// Author:      Zhenya Li						                   //
//              zli52@syr.edu									   //
/////////////////////////////////////////////////////////////////////

#include <algorithm>
#include "XmlElementParts.h"

typedef std::vector<std::pair<std::string, std::string>> vecOfPairs;
typedef std::pair<std::string, std::string> pairOfStrings;

//----< construct XmlParts instance >---------------------------

XmlParts::XmlParts(Toker* pTokr) : pToker(pTokr), Verbose(false)
{
}
//----< destructor >-------------------------------------------

XmlParts::~XmlParts()
{
}
//----< is this a terminating token? >-------------------------

bool XmlParts::isTerminator(std::string tok)
{
  if(tok.length() == 0)
    return false;
  if(tok == ">")
    return true;
  return false;
}
//
//----< collect an XmlElementParts sequence >------------------

bool XmlParts::get()
{
  toks.clear();
  std::string tok;
  do
  {
    if(pToker->isFileEnd())
    {
      if(length() > 0)
        return true;
      return false;
    }
    tok = pToker->getTok();
    if(Verbose)
    {
      if(tok != "\n")
        std::cout << "\n--tok=" << tok << std::endl;
      else
        std::cout << "\n--tok=newline\n";
    }
    if(toks.size() > 0 && tok == "<")
    {
      pToker->putback('<');
      break;
    }
    if(tok != "\n")
      toks.push_back(tok);
  } while(!isTerminator(tok));
  return true;
}
//----< index operator >---------------------------------------

std::string& XmlParts::operator[](int n)
{
  if(n < 0 || toks.size() <= (size_t)n)
    throw std::exception("XmlParts index out of range");
  return toks[n];
}
//----< collect semi-expression as space-seperated string >----

std::string XmlParts::show()
{
  if(toks.size() == 0)
    return "";
  std::string temp(" ");
  for(size_t i=0; i<toks.size(); ++i)
    if(toks[i] != "\n")
      temp.append(" ").append(toks[i]);
  return temp;
}
//
//----< is tok found in XmlElementParts? >---------------------

int XmlParts::find(const std::string& tok)
{
  for(int i=0; i<length(); ++i)
    if(tok == toks[i])
      return i;
  return length();
}
//----< remove tok if found in semi-expression >---------------

bool XmlParts::remove(const std::string& tok)
{
  std::vector<std::string>::iterator it;
  it = std::find(toks.begin(),toks.end(),tok);
  if(it != toks.end())
  {
    toks.erase(it);
    return true;
  }
  return false;
}
//----< remove leading newlines >------------------------------

void XmlParts::trimFront()
{
  while(toks.size() > 0 && toks[0] == "\n")
    remove("\n");
}
//----< make tokens lowercase >--------------------------------

void XmlParts::toLower()
{
  for(int i=0; i<length(); ++i)
  {
    for(size_t j=0; j<(*this)[i].length(); ++j)
    {
      (*this)[i][j] = tolower((*this)[i][j]);
    }
  }
}
//----< extract the attributes from parsed parts >--------------------------------

vecOfPairs XmlParts::extractAtbs(){
	vecOfPairs returnVec;
	std::vector<std::string> tempTok=toks;
	for (size_t i = 0 ; i < tempTok.size() - 1; ++i){
		if (tempTok[i] == "=")
			returnVec.push_back(pairOfStrings(tempTok[i - 1], tempTok[i + 1].substr(1, tempTok[i + 1].size()-2)));
	}
	return returnVec;
}
//----< extract comments >--------------------------------

std::string XmlParts::extractComment(){
	std::string comment;
	if (toks[0] == "<"&&toks[1] == "!"&&toks[2] == "--"){
		size_t i = 3;
		for (; i < toks.size() - 3; ++i)
			comment.append(toks[i]).append(" ");
		comment.append(toks[i]);
		return comment;
	}
	else return "";
}
//----< extract text >--------------------------------

std::string XmlParts::extractText(){
	std::string text;
	if (toks[0] != "<"){
		size_t i = 0;
		for (; i < toks.size() - 1; ++i)
			text.append(toks[i]).append(" ");
		text.append(toks[i]);
		return text;
	}
	else return "";
}

//----< test stub >--------------------------------------------

#ifdef TEST_XMLELEMENTPARTS

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing XmlParts class\n "
            << std::string(23,'=') << std::endl;
  std::cout
    << "\n  Note that quotes are returned as single tokens\n\n";

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-') << "\n\n";
    try
    {
      Toker toker(argv[i]);
      toker.setMode(Toker::xml);
      XmlParts parts(&toker);
    //parts.verbose();                 // uncomment to show token details
      while(parts.get())
        std::cout << parts.show().c_str() << std::endl;
      std::cout << "\n\n";
    }
    catch(std::exception ex)
    {
      std::cout << "\n  " << ex.what() << "\n\n";
    }
  }

  try
  {
	  Toker toker("LectureNote.xml");
	  toker.setMode(Toker::xml);
	  XmlParts parts(&toker);
	  parts.verbose();                 // uncomment to show token details
	  while (parts.get())
		  std::cout << parts.show().c_str() << std::endl;
	  std::cout << "\n\n";
  }
  catch (std::exception ex)
  {
	  std::cout << "\n  " << ex.what() << "\n\n";
  }
}

#endif

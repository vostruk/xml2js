#include "Parser.h"
#include <iostream>
#include "XMLnode.h"
#include <algorithm>
#include <functional> 
#include <cctype>
#include <locale>

Parser::Parser(Lexer* l):
lexer(l),
treeRoot("&root")
{
}


Parser::~Parser()
{
}
XMLnode& Parser::getTreeRoot()
{
	return treeRoot;
}
void Parser::parse()
{
	Token t = lexer->nextToken();
	while (t.type != startTag) t = lexer->nextToken(); // omit all comments and process instructions
	parseElement(treeRoot);
	eh.errif("Not enough closing elements!", !controlStack.empty(), 20);
}

// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

void Parser::parseElement(XMLnode& treeNode)
{
	Token t = lexer->nextToken();

	eh.errif("Name tag expected!", t.type != nameTag, 12);
	controlStack.push_front(t);
	if (treeNode.getName() == "&root" || treeNode.getName() == "%noName"){ 
		//t.value.erase(std::remove(t.value.begin(), t.value.end(), '\n'), t.value.end());
		treeNode.getName(trim(t.value));
	}
	t = lexer->nextToken();
	if (t.type != closeEmptyTag && t.type != closeTag)
	{
		while(t.type == nameTag) //argument list processing
		{
			XMLnode *atrnode = treeNode.createNextChild("@" + trim(t.value));
			t = lexer->nextToken();
			eh.errif("'=' sign is expected after argument type", t.type != equalTag, 13);
			t = lexer->nextToken(); //this will be value tag
			eh.errif("Value of atribute is expected", t.type != atributeValue, 14);
			atrnode->createNextChild(trim(t.value), true);
			t = lexer->nextToken();
		} //all arguments are processed 
	} // t must be set to next token -> must be close or close empty tag
		
	if (t.type == closeTag) //process all inside elements untill </element> tag
	{ 
		while (1) //simple text or next element or comment inside //untill startCloseTag will be found
		{
			t = lexer->nextToken(true); //there may be simple text inside //return everything untill <
			Token t2 = lexer->nextToken(); //TODO: improve for recognizing CDATAtag and few Texts
					
			while (t2.type != startTag&&t2.type != startCloseTag&&t2.type!=CDATA) t2 = lexer->nextToken(); //omit comments
			if (t2.type == CDATA) { 
				t.value += t2.value; 
				t2 = lexer->nextToken(true); 
				t.value += t2.value; 
				t2 = lexer->nextToken();
			}
			if (t2.type==startTag&&t.value != "" && !all_of(t.value.begin(), t.value.end(), iswspace))  //there is some text here
			{
				t.value.erase(std::remove(t.value.begin(), t.value.end(), '\n'), t.value.end());
				t.value.erase(std::remove(t.value.begin(), t.value.end(), '\t'), t.value.end());
				treeNode.createNextChild("#text")->createNextChild(trim(t.value), true);
				parseElement(*treeNode.createNextChild("%noName"));
			}
			else if(t2.type==startTag) parseElement(*treeNode.createNextChild("%noName"));
			else if (t2.type == startCloseTag) //there's no inside elements
			{
				t.value.erase(std::remove(t.value.begin(), t.value.end(), '\n'), t.value.end());
				t.value.erase(std::remove(t.value.begin(), t.value.end(), '\t'), t.value.end());
				if (treeNode.take_next_child("<", true) == NULL) //no other children - only text
					treeNode.createNextChild(trim(t.value), true);
				else if (t.value != "" && !all_of(t.value.begin(), t.value.end(), iswspace))
					treeNode.createNextChild("#text")->createNextChild(trim(t.value), true);
				break;
			}
		}
		t = lexer->nextToken(); //must be type of close tag
		eh.errif("Name tag expected for closing element!", t.type != nameTag, 15);
		eh.errif("Name expected for closing tag must be " + controlStack.front().value+", not " +t.value, controlStack.front().value != t.value, 16);
		
		controlStack.pop_front();
		t = lexer->nextToken();
		eh.errif("Close tag expected!", t.type != closeTag, 17);
			
	}
	else
		if (t.type == closeEmptyTag)
		{
			controlStack.pop_front();
		}
}


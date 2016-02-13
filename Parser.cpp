#include "Parser.h"
#include <iostream>
#include "XMLnode.h"

Parser::Parser(Lexer* l):
lexer(l)
{
	treeRoot = NULL;
}


Parser::~Parser()
{
}

XMLnode& Parser::getTreeRoot()
{
	eh.errif("The tree Root is NULL. Document must contain exactly one element.", treeRoot == NULL, 30);
	return *treeRoot;
}

void Parser::parse()
{	
	while (lexer->seeToken().type == comment || lexer->seeToken().type == proInsTag || lexer->seeToken().type == DOCTYPE)
		lexer->getToken();
	treeRoot = parseElement(); //NULL if no elements in document
	eh.errif("Not enough closing elements!", !controlStack.empty(), 20);
	return;
}

XMLnode* Parser::parseElement()
{
	Token t = lexer->seeToken();
	string elName = "";
	list<XMLnode*> childr;
	list<string> texts;

	if (t.type != startTag) return NULL; 
	else lexer->getToken();
	t = lexer->getToken(); //must be element name
	if (t.type == nameTag) elName = t.value;
	else return NULL;
	controlStack.push_front(t);

	auto nvList = parseAttributes(); //empty if no attributes. next t is > or />
	t = lexer->getToken();
	switch (t.type)
	{
	case closeEmptyTag:
		controlStack.pop_front();
		return new XMLnode(elName, nvList, childr, texts);
		break;
	case closeTag:
	{
		XMLnode* childptr;
		string s = "";
		while (t.type!=startCloseTag)
		{
			t = lexer->getText();
			s = t.value;
			if (s != "") texts.push_back(s);
			switch (lexer->seeToken().type)
			{
			case startTag: 
				if ((childptr = parseElement()) != NULL) childr.push_front(childptr);
				break;
			case CDATA:
				s = lexer->getToken().value;
				if (s != "") texts.push_back(s);
				break;
			case startCloseTag:
			case comment:
			case proInsTag:
				t= lexer->getToken();
				break;
			case ENDofFILE:
				eh.errif("Unexpected end of File!!", true, 30);
				break;
			default:
				break;
			}
		}
		t = lexer->getToken(); //name here
		eh.errif("Name expected for closing tag must be " + controlStack.front().value + ", not " + t.value, controlStack.front().value != t.value, 16);
		controlStack.pop_front();
		eh.errif("Close tag is expected after </name... ", lexer->getToken().type != closeTag, 28); //closeTag here
		return new XMLnode(elName, nvList, childr, texts);
	}break;
	default:
		break;
	}
	return NULL;
}


list<pair<string, string>> Parser::parseAttributes()
{
	string f, s;
	list<pair<string, string>> L;
	Token t = lexer->seeToken();
	while (t.type == nameTag)
	{
		lexer->getToken();
		f = t.value;
		t = lexer->getToken();
		eh.errif("'=' sign is expected after argument type", t.type != equalTag, 13);
		t = lexer->getToken(); //value tag
		eh.errif("Value of atribute is expected", t.type != atributeValue, 14);
		s = t.value;
		L.push_back(pair<string, string>(f,s));
		t = lexer->seeToken();
	}
	return L;
}

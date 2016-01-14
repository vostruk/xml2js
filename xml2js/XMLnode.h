#pragma once
#include <string>
#include <list>
#include <iostream>
#include "JSsourse.h"

using namespace std;
class XMLnode
{
	string Nazwa;
	bool is_text;
	list <pair<bool, XMLnode>> childElements;
	
public:
	XMLnode(string, bool=false);
	~XMLnode();
	void setTreeProcessedFalse(XMLnode&);
	void writeMe2JSON(JSsourse*, bool no_name = false);
	bool more_children_have_theSameName(string name);
	XMLnode* take_next_child(string = "<", bool=false);
	void printJS(string);
	void drawTree(int, ostream& o = cout);
	XMLnode* createNextChild(string, bool=false);
	string getName(string n = ""){ if (n != "") Nazwa = n; return Nazwa; }
};


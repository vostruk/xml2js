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
	list <pair<bool, XMLnode*>> childElements;
	
	void setTreeProcessedFalse(XMLnode&);
	bool more_children_have_theSameName(string name);
	XMLnode* take_next_child(string = "<", bool = false);
	string getName(string n = ""){ if (n != "") Nazwa = n; return Nazwa; }
	void del(list <pair<bool, XMLnode*>>);
public:
	XMLnode(string, bool=false);
	XMLnode(string, list<pair<string, string>>, list<XMLnode*>, list<string>);
	~XMLnode();
	

	void writeMe2JSON(JSsourse*, bool no_name = false);
	void drawTree(int, ostream& o = cout);
};


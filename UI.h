#pragma once
#include <string>
#include "XMLsourse.h"
#include <iostream>
#include "XMLnode.h"
using namespace std;
class UI
{
	string filename = "input.xml";

public:
	UI();
	~UI();
	void startUI();  //indegrates all methods below
	int getFilename();
	void showHelp(); 
	void generateJSON(string); //creates JSON file from XML file with name in argument


};


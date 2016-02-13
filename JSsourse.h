#pragma once
#include <string>
#include <fstream>

using namespace std;
class JSsourse
{
	ofstream JSfile;

public:
	JSsourse(string);
	~JSsourse();

	void writeToJSFile(string);
};


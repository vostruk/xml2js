#pragma once
#include <string>
#include <iostream>

using namespace std;
class ErrorHandler
{
public:
	ErrorHandler();
	~ErrorHandler();
	void errif(string e, bool cond=true, int errn=0, ostream& out = cout);
};


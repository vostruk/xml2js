#include "JSsourse.h"
#include <iostream>

JSsourse::JSsourse(string f)
{
	JSfile.open(f);
}


JSsourse::~JSsourse()
{
	JSfile.close();
}

void JSsourse::writeToJSFile(string elem)
{
	JSfile << elem;
}

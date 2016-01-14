#include "XMLsourse.h"
#include <iostream>

XMLsourse::XMLsourse(string f)
{
	XMLfile.open(f, ifstream::in);
}


XMLsourse::~XMLsourse()
{
	XMLfile.close();
}

char XMLsourse::charFromFile()
{
	return XMLfile.get();
	
}

char XMLsourse::getChar()
{
	if (fifoBuffer.empty()) return charFromFile();
	char c;
	c = fifoBuffer.front();
	fifoBuffer.pop_front();
	return c;
}

char XMLsourse::nextChar(unsigned int i)
{
	while (i >= fifoBuffer.size()) fifoBuffer.push_back(charFromFile());
	return fifoBuffer[i];
}

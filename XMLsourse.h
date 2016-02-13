#pragma once
#include <string>
#include <queue>
#include <fstream>
using namespace std;

class XMLsourse
{
	deque<char> fifoBuffer;
	ifstream XMLfile;

	char charFromFile();

public:
	XMLsourse(string);
	~XMLsourse();

	char getChar();  //get(pop) next symbol from buffer
	char nextChar(unsigned int = 0); //see netx symbol from buffer.
};


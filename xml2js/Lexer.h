#pragma once
#include "XMLsourse.h"
#include "Token.h"
#include <map>
#include "ErrorHandler.h"
class Lexer
{
	XMLsourse* sourse;
	ErrorHandler eh;
public:
	Lexer(XMLsourse*);
	~Lexer();
	
	Token nextToken(bool = false);
	char processPredef();
};


#pragma once
#include "XMLsourse.h"
#include "Token.h"
#include <map>
#include "ErrorHandler.h"
class Lexer
{
	XMLsourse* sourse;
	ErrorHandler eh;
	deque<Token> fifoTokens;
public:
	Lexer(XMLsourse*);
	~Lexer();
	
	Token nextToken();
	Token getText();
	Token seeToken();
	Token getToken();
	char processPredef();
};


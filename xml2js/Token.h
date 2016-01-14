#pragma once
#include <string>
using namespace std;
enum Tokens {
	ENDofFILE,
	simpleTEXT,
	closeTag,
	closeEmptyTag,
	startCloseTag,
	startTag,
	proInsTag,
	comment,
	CDATA,
	DOCTYPE,
	atributeValue,
	equalTag,
	nameTag
};
class Token
{
public:
	Tokens type;
	string value = "";

	Token(Tokens, string v = "");
	~Token();
};


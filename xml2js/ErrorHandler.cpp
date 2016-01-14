#include "ErrorHandler.h"


ErrorHandler::ErrorHandler()
{
}


ErrorHandler::~ErrorHandler()
{
}

void ErrorHandler::errif(string e, bool cond, int critical, ostream & out)
{
	if (cond)
	{
		if (critical > 11) e = "Parser Error## "+e;
		else e = "Lexer Error## " + e;
		out << e;
		if (critical) {
			cin.clear();
			fflush(stdin);
			cin.get();
			exit(critical);
		}
	}
}
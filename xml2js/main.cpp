//main file
#include "UI.h"
#include "Lexer.h"
#include "Parser.h"
#include "JSsourse.h"
int main(int argc, char **argv)
{
	UI consoleInterface;
	if (argc<2)
		consoleInterface.startUI();   //loop interface
	else{
		string filename(argv[1]);
		consoleInterface.generateJSON(filename); //one run
	}
	return 0;
}

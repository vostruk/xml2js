#include "UI.h"
#include "Parser.h"
#include "XMLnode.h"
UI::UI()
{
}


UI::~UI()
{
}

void UI::startUI() //indegrates all methods below
{
	cout << "Welcome to XML to JSON translator"<<endl;
	//petla dzialan tutaj
	//cout << "Print 'help' to display the help. Print 'exit' to exit the program" << endl;
	showHelp();
	char task;

	while (true)
	{
		cin.clear();
		fflush(stdin);
		cout << "Choose your command: ";
		cin >> task;
		switch (task)
		{
		case '0':
			return;
			break;
		case '1':
			getFilename();
			break;
		case '2':
		{
			XMLsourse sourse(filename);
			Lexer lexer(&sourse);
			Token t = lexer.nextToken();
			while (t.type != ENDofFILE)
			{
				if (t.type == simpleTEXT&&!all_of(t.value.begin(), t.value.end(), iswspace))
				{
					std::cout << t.type << t.value << endl;
					t = lexer.nextToken();
				}
				else if (t.type == closeTag || t.type == closeEmptyTag)
				{
					std::cout << t.type << t.value << endl;
					t = lexer.nextToken(true);
				}else if(t.type!=simpleTEXT){
					std::cout << t.type << t.value << endl;
					t = lexer.nextToken();
				}
				else t = lexer.nextToken();
			}
		}break;
		case '3':
		case '4':
		{
			XMLsourse sourse(filename);
			Lexer lexer(&sourse);
			Parser parser(&lexer);
			parser.parse();
		
			if (task == '3'){
				parser.getTreeRoot().drawTree(0);
				cout << endl << endl;
			}else{
				ofstream jfile(filename + ".json");
				if (!jfile.is_open())
				{
					std::cout << "Coul not open" << filename << ".json file!" << endl;
					break;
				}else{
					jfile.close();
					JSsourse JSfile(filename + ".json");
					JSfile.writeToJSFile("{");
					parser.getTreeRoot().writeMe2JSON(&JSfile);
					JSfile.writeToJSFile("}");
					cout << "JSON file was successfully writen out." << endl;
				}
			}break;
		}
		default:

			cout << "Unknown command! Give a number as in help:" << endl;
			showHelp();
			break;
		}
		//getchar();
	} //while true
}
int UI::getFilename()
{
	filename = "";
	cout << "Please give the type of xml file: ";
	cin >> filename;
	ifstream f(filename, ifstream::in);
	if (!f.is_open())
	{
		cout << "The file with type " <<filename <<" could not be read or does not exist" << endl;
		return 1;
	}
	f.clear();
	f.close();
	cout << "The file is OK" << endl;
	return 0;
}
void UI::showHelp()
{
	cout << "Give a number with required option:" << endl;
	cout << " 1 Read XML file\n 2 Show recognized TOKENS\n 3 Draw XML tree\n 4 Build JSON file\n 0 - exit the program" << endl;
}
void UI::generateJSON(string xmlFileName)
{
	filename = xmlFileName;
	XMLsourse sourse(filename);
	Lexer lexer(&sourse);
	Parser parser(&lexer);
	parser.parse();
	JSsourse JSfile(filename + ".json");
	JSfile.writeToJSFile("{");
	parser.getTreeRoot().writeMe2JSON(&JSfile);
	JSfile.writeToJSFile("}");
}
#include "Lexer.h"

Lexer::Lexer(XMLsourse* input)
{
	sourse = input;
}


Lexer::~Lexer()
{

}

//possible predefs:
// &lt; is "<"
// &gt; is ">"
// &amp; is "&"
// &apos; is '
// &quot; is "
// allowed only in atribute values and texts
char Lexer::processPredef()
{
	char c = ' ';
	switch (sourse->getChar())
	{
	case 'g':
		eh.errif("Predefined expression warning: 't' is expected after '&g' and '&l'\n", sourse->getChar() != 't', 0);
		c = '>';
		break;
	case 'l':
		eh.errif("Predefined expression warning: 't' is expected after '&g' and '&l'\n", sourse->getChar() != 't', 0);
		c = '<';
		break;
	case 'q':
		eh.errif("Predefined expression warning: 'u' is expected after '&q'\n", sourse->getChar() != 'u', 0);
		eh.errif("Predefined expression warning: 'o' is expected after '&qu'\n", sourse->getChar() != 'o', 0);
		eh.errif("Predefined expression warning: 't' is expected after '&quo'\n", sourse->getChar() != 't', 0);
		c = '"';
		break;
	case 'a':
		switch (sourse->getChar())
		{
		case 'm':
			eh.errif("Predefined expression warning: 'p' is expected after '&am'\n", sourse->getChar() != 'p', 0);
			c = '&';
			break;
		case 'p':
			eh.errif("Predefined expression warning: 'o' is expected after '&ap'\n", sourse->getChar() != 'o', 0);
			eh.errif("Predefined expression warning: 's' is expected after '&apo'\n", sourse->getChar() != 's', 0);
			c = '\'';
			break;
		default: eh.errif("Predefined expression warning: unexpected symbol after '&a'", true, 0);
			return c;
		}break;
		
	default: //eh.errif("Predefined expression warning: unknown entity\n", true, 0);
		return c;
	}
	eh.errif("Predefined expression error: must end with ';'\n", sourse->getChar() != ';', 0);
	return c;
}
/*
@returns tokens:
	Token("ENDofFILE") // nazwy - na enum
	Token("simpleTEXT", value)
	Token("closeTag", ">")
	Token("closeEmptyTag", "/>")
	Token("startCloseTag", "</")
	Token("startTag", "<")
	Token("proInsTag", value)
	Token("comment", value)
	Token("CDATA", value)
	Token("DOCTYPE", value)
	Token("atributeValue", value)
	Token("equalTag", "=")
	Token("nameTag", value)
*/

Token Lexer::nextToken(bool is_text)
{
	char c;
	string value = "";
	if (is_text){ //it's simple text - get everything including white symbols and special characters
		while ((c=sourse->nextChar()) != '<') if (c == EOF) return Token(ENDofFILE);
		else{
			c = sourse->getChar();
			if (c == '&') c = processPredef();
			if (c == '"') value += '\\';
			value += c;
		}
		return Token(simpleTEXT, value);
	}
	c = sourse->getChar();
	while (iswspace(c) && c != EOF) c = sourse->getChar();
	if (c == EOF) return Token(ENDofFILE);
	
	
	switch (c)
	{
		case '<':
			switch (sourse->nextChar())
			{
				case '!':
				{
					sourse->getChar();
					switch (sourse->nextChar())
					{
					case '-':
					{ //comment
						sourse->getChar();
						eh.errif("Comment: '-' character is expected after '<!-'", sourse->getChar() != '-', 2);
						while (c != EOF && !(c == '-' && sourse->nextChar() == '-')) value += (c = sourse->getChar());
						eh.errif("Unexpected end of file!", c == EOF,4);
						eh.errif("Comment: single '-' character is not allowed", sourse->getChar() != '-', 2);
						eh.errif("Comment: '>' is expected after '--' ", sourse->getChar() != '>', 2);
						value.pop_back();
						return Token(comment, value);
					}break;
					case '[':
					{ //CDATA
						sourse->getChar();
						bool t = true;
						t = t && (sourse->getChar() == 'C');
						t = t && (sourse->getChar() == 'D');
						t = t && (sourse->getChar() == 'A');
						t = t && (sourse->getChar() == 'T');
						t = t && (sourse->getChar() == 'A');
						t = t && (sourse->getChar() == '[');
						eh.errif("Start of CDATA tag is not properly formatted", !t, 3);
						while (c != EOF && !(c == ']'&&sourse->nextChar() == ']'&&sourse->nextChar(1) == '>')) {
							c = sourse->getChar();
							if (c == '"') value+= '\\';
							value += c;
						}
						eh.errif("Unexpected end of file!", c == EOF, 4);
						sourse->getChar(); sourse->getChar();
						value.pop_back();
						return Token(CDATA, value);
					}break;
					default: //otherwise it's DOCTYPE tag untill >
						int o = 1;
						while (c != EOF)
						{
							c = sourse->getChar();
							if (c == '<') o++;
							if (c == '>' && --o == 0) break;
							else value += c;
						}
						eh.errif("Unexpected end of file!", c == EOF, 4);
						sourse->getChar();
						return Token(DOCTYPE, value);
						break;
					}
				} break;  //end <! case
				case '?':
				{//process instruction
					sourse->getChar();
					while (c != '?'&& c != EOF) value += (c = sourse->getChar());
					eh.errif("Unexpected end of file!", c == EOF, 4);
					eh.errif("Process instruction: '>' symbol is espected after '?'", sourse->getChar() != '>',5);
					value.pop_back();
					return Token(proInsTag, value);
				}break; //end <? case
				case '/': sourse->getChar(); return Token(startCloseTag, "</");
				break;
				default: return Token(startTag, "<"); //nextChar is some other symbol
				break;
			}
		break;
		case '>': return Token(closeTag, ">");
		break;
		case '/': 
				eh.errif("Close empty tag: '>' is expected after '/'", sourse->getChar() != '>', 7);
				return Token(closeEmptyTag, "/>");
		break;
		case 34: //quote -> arg value
			while (c != EOF && sourse->nextChar() != 34)
			{
				c = sourse->getChar();
				if (c == '&') c = processPredef();
				if (c == '"') value += '\\'; //????
				value += c;
			}
			eh.errif("Unexpected end of file!", c == EOF, 4);
			sourse->getChar(); //zjadamy pozostaly quote
			return Token(atributeValue, value);
		break;
		case 39: //apostroph -the same
			while (c != EOF && sourse->nextChar() != 39)
			{
				c = sourse->getChar();
				if (c == '&') c = processPredef();
				if (c == '"') value += '\\'; //to omit the problems with quotes in JSon
				value += c;
			}
				eh.errif("Unexpected end of file!", c == EOF, 4);
				sourse->getChar(); //zjadamy pozostaly quote
				return Token(atributeValue, value);
		break;
		case '=': return Token(equalTag, "=");
		break;
		default: //it's some name then - element name or attribute name
				value += c;
				while (c != EOF && sourse->nextChar() != '='&&sourse->nextChar()!='>'
					&&sourse->nextChar() != '<'&&  sourse->nextChar() != '/'&& !iswspace(sourse->nextChar()))
					value += (c = sourse->getChar());
				eh.errif("Unexpected end of file!", c == EOF, 4);
				return Token(nameTag, value);	
		break;
	}
}
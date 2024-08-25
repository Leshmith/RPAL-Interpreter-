#include "Lexer.h"

using namespace std;

lexer::lexer(std::ifstream* source) {
	// Constructor for the lexer class
	sourceFile = source; 
	lineCount = 1;
	charCount = 1;
}

lexer::~lexer() {
	 // Destructor for the lexer class
}

void lexer::lexerReset(){
	// Resets the lexer by clearing and seeking to the beginning of the source file
	sourceFile->clear();
	sourceFile->seekg(0, ios::beg);
}

bool lexer::isPunction(char c){
// Checks if a character is a punctuation symbol
  if('(' == c || ')' == c || ';' == c || ',' == c)
    return true;
  else
    return false;
}

bool lexer::isOperatorSymbol(char c){
	// Checks if a character is an operator symbol
	 if ('+' == c || '-' == c || '*' == c || '<' == c || '>' == c || '*' == c || '&' == c || '.' == c
	    || '@' == c|| '/' == c|| ':'== c || '=' == c|| '~' == c|| '|'== c || '$'== c || '!' == c
	    || '#'== c || '%'== c || '^'== c || '_'== c || '['== c || ']'== c || '{'== c || '}'== c
	    || '"'== c || '`'== c || '?'== c)
	        return true;
	    else
	        return false;
}

bool lexer::isCommentChar(char c){
	// Checks if a character is a comment character (single quote)
	if (39 == c)  
		return true;
	else
		return false;
}

string lexer::tokenIdentifier(){
	// Extracts an identifier token from the source file
	string tokStr= "";
	char nextPeek;
	char nextChar;
	do {
		sourceFile->get(nextChar);
		charCount++;
		tokStr += nextChar;
		nextPeek = sourceFile->peek();
	}while (!(EOF == nextPeek) && (isalpha(nextPeek) || isdigit(nextPeek) || (95 == nextPeek)));
	return tokStr;
}

string lexer::tokenInteger(){
	// Extracts an integer token from the source file
	string tokStr= "";
	char nextPeek;
	char nextChar;
	do {
		sourceFile->get(nextChar);
		charCount++;
		tokStr += nextChar;
		nextPeek = sourceFile->peek();
	}while (!(EOF == nextPeek) && isdigit(nextPeek));
	return tokStr;
}

string lexer::tokenStrings(){
	// Extracts a string token from the source file
	string tokStr= "";
	char nextPeek;
	char nextChar;
	do{
		sourceFile->get(nextChar);
		charCount++;
		tokStr += nextChar;
		nextPeek = sourceFile->peek();
		if (nextChar == '\\'){
		    if ((nextPeek == 't' || nextPeek == 'n' || nextPeek == '\\' || nextPeek == '\''))
		    {
		        continue; //Valid escape sequence
		    } else {
		        printf ("Invalid escape sequence\n");
		        exit(0);
		    }
		}
	} while (!(EOF == nextPeek) && !(39 == nextPeek));
	sourceFile->get(nextChar);
	tokStr += nextChar;
	return tokStr;
}

string lexer::tokenSpaces(){
	// Extracts spaces (whitespace characters) from the source file
	string tokStr= "";
	char nextPeek;
	char nextChar;
	do{
		sourceFile->get(nextChar);
		charCount++;
		if (10 == nextChar || 13 == nextChar){
			charCount = 1;
			lineCount++;
		}
		tokStr += nextChar;
		nextPeek = sourceFile->peek();
	} while (!(EOF == nextPeek) && isspace(nextPeek));
	return tokStr;
}

string lexer::tokenComment(){
	// Extracts a comment token from the source file
	string tokStr= "";
	char nextPeek;
	char nextChar;
	sourceFile->get(nextChar);
	charCount++;
	tokStr += nextChar;
	nextPeek = sourceFile->peek();
	if (47 == nextPeek){
		sourceFile->get(nextChar);
		tokStr += nextChar;
		do{
			sourceFile->get(nextChar);
			tokStr += nextChar;
			nextPeek = sourceFile->peek();
		} while (!(EOF == nextPeek) && !(10 == nextPeek || 13 == nextPeek));
		return tokStr;
	} else {
		return tokStr;
	}
}

string lexer::tokenOperator(){
	// Extracts an integer token from the source file
	string tokStr= "";
	char nextPeek;
	char nextChar;
	do{
		sourceFile->get(nextChar);
		charCount++;
		tokStr += nextChar;
		nextPeek = sourceFile->peek();
	} while (!(EOF == nextPeek) && isOperatorSymbol(nextPeek));
	return tokStr;
}

token* lexer::getNextToken(){
	token* tok = new token();
	string tokenizedLex;
	int nextChar;
	char readNext;
	nextChar = sourceFile->peek();

	// Check if the next character is an alphabetic character (identifier)
	if (isalpha(nextChar)){
		if (LEXLOGS) printf ("This is an identifier\n");
		tokenizedLex = tokenIdentifier();
        tok->tokType = TOKEN_IDENTIFIER;
	// Check if the next character is a digit (integer)
	} else if (isdigit(nextChar)) {
		if (LEXLOGS) printf ("This is an integer\n");
		tokenizedLex = tokenInteger();
		tok->tokType = TOKEN_INTEGER;
	// Check if the next character is a single quote (string)
	} else if (39 == nextChar){
		if (LEXLOGS) printf ("This is a string\n");
		tokenizedLex = tokenStrings();
		tok->tokType = TOKEN_STRING;
	// Check if the next character is whitespace
	} else if (isspace(nextChar)){
		if (LEXLOGS) printf ("This is an space\n");
		tokenizedLex = tokenSpaces();
		tok->tokType = TOKEN_DELETE;
	// Check if the next character is punctuation
	} else if (isPunction(nextChar)){
		if (LEXLOGS) printf ("This is a punction\n");
		sourceFile->get(readNext);
		charCount++;
		tokenizedLex = readNext;
		tok->tokType = TOKEN_PUNCTION;
	// Check if the next character is a forward slash (comment or operator)
	} else if (47 == nextChar) {
		tokenizedLex = tokenComment();
		if (!tokenizedLex.compare("/")){
			if (LEXLOGS) printf ("This is an operator\n");
			tok->tokType = TOKEN_OPERATOR;
		} else{
			if (LEXLOGS) printf ("This is a comment\n");
			tok->tokType = TOKEN_DELETE;
		}
	// Check if the next character is an operator symbol
	} else if (isOperatorSymbol(nextChar)){
		if (LEXLOGS) printf ("This is an operator\n");
		tokenizedLex = tokenOperator();
		tok->tokType = TOKEN_OPERATOR;
	// Check if we've reached the end of the file
	} else if (EOF == nextChar) {
		if (LEXLOGS) printf ("EOF reached\n");
		tok->tokType = TOKEN_EOF;
	}
	// Set token values and counts
	tok->tokValue = tokenizedLex;
	tok->charCount = charCount;
	tok->lineCount = lineCount;
	if (LEXLOGS) printf ("Lexer returning token type = %d, charCount = %d, lineCount = %d, value = %s\n",
			tok->tokType, tok->charCount, tok->lineCount, tok->tokValue.c_str());
	return tok;
}

#ifndef LEXER_H_
#define LEXER_H_
using namespace std;

// Define a constant for controlling logging (set to 0 for no logs)
#define LEXLOGS 0
#define PARSERLOGS 0
#define STANDARDIZERLOGS 0
#define MAIN_DEBUG_LOGS 0

#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include "Token.h"

class lexer {
public:
	// Constructor: Initializes the lexer with an input file stream
	lexer(std::ifstream*);

	// Destructor: Cleans up any resources used by the lexer
	virtual ~lexer();

	// Get the next token from the input stream
	token* getNextToken();

	// Reset the lexer state (e.g., line and character counts)
	void lexerReset();

private:
    ifstream* sourceFile; // Pointer to the input file stream
    int lineCount, charCount; // Keep track of line and character counts

	// Helper functions for tokenization
    string tokenIdentifier();
    string tokenInteger();
    string tokenStrings();
    string tokenSpaces();
    string tokenComment();
    string tokenOperator();
    string tokenPunction();

	// Various checks for character types
	bool isPunction(char);
	bool isOperatorSymbol(char);
	bool isSpaces(char);
	bool isEOL(char);
	bool isEOF(char);
	bool isCommentChar(char);
	bool isKeyword(string);
};

#endif /* LEXER_H_ */

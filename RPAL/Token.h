#include <string>
#ifndef TOKEN_H_
#define TOKEN_H_

enum tokenType{
	TOKEN_DEFAULT = 0,
	TOKEN_KEYWORD = 1,
	TOKEN_IDENTIFIER = 2,
	TOKEN_OPERATOR = 3,
	TOKEN_WHITESPACE = 4,
	TOKEN_COMMENT = 5,
	TOKEN_STRING = 6,
	TOKEN_INTEGER = 7,
	TOKEN_PUNCTION = 8,
	TOKEN_EOF = 9,
	TOKEN_DELETE = 10,
	TOKEN_ERROR,
	TOKEN_ANY
};

class token {

public:
	token();
	virtual ~token();

	// Properties
	int tokType; // Type of the token (e.g., keyword, identifier, etc.)
	std::string tokValue; // Value of the token (e.g., actual keyword or identifier)
	int lineCount; // Line number where the token appears
	int charCount; // Character position within the line
};

#endif /* TOKEN_H_ */

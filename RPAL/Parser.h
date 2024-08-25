#ifndef PARSER_H_
#define PARSER_H_
#include "Lexer.h"
#include <stack>
#include "Standardized_Tree.h"
#include "CSE_Machine.h"

using namespace std;

class parser {
public:

	// Constructor: Initializes the parser with a lexer
	parser (lexer*);

	// Print the Abstract Syntax Tree (AST)
	void printAST();

	// Print the Symbol Table (ST)
	void printST();

	// Evaluate the program
    void evaluateProg();

	// Destructor: Clean up any resources used by the parser
	virtual ~parser();

private:
    lexer* lex;
    stack <treeNode*> treeStack;
    token *nextToken;

	// Production rules
	void E();
	void Ew();
	void T();
	void Ta();
	void Tc();
	void B();
	void Bt();
	void Bs();
	void Bp();
	void A();
	void At();
	void Af();
	void Ap();
	void R();
	void Rn();
	void D();
	void Da();
	void Dr();
	void Db();
	void Vb();
	void Vl();

	// Parsing and tree-building methods
	void parse();
	void standardize(treeNode*);
    void buildTree(string, int); 
    void buildTree(string, int, int);
    void printTree(treeNode*, int);
    void read(string);

	// Helper method to check if a token value is a keyword
	bool isKeyword(string);

	// Helper method to convert a tree node to a string representation
	string to_s(treeNode*);
};

#endif /* PARSER_H_ */

#include "Parser.h"

parser::parser(lexer* lexr){
	this->lex = lexr;
	nextToken = NULL;
}

parser::~parser() {
	// Destructor: Clean up any resources used by the parser
}

void parser::printAST(){
	parse();
	if (!treeStack.empty())
		printTree(treeStack.top(), 0);
	lex->lexerReset();
}

void parser::printST(){
    parse();
    Standardized_Tree *ts = new Standardized_Tree(treeStack.top());
    if (!treeStack.empty())
        printTree(treeStack.top(), 0);
    lex->lexerReset();
}

void parser::evaluateProg(){
    parse();
    Standardized_Tree *ts = new Standardized_Tree(treeStack.top());
    CSE_Machine* cse = new CSE_Machine();
    cse->run(treeStack.top());
}

void parser::parse(){
	if (PARSERLOGS) printf ("Initiating parsing\n");
	do {
		nextToken = lex->getNextToken();
	}while (nextToken->tokType == TOKEN_DELETE);
	E();
	if (!treeStack.empty() && treeStack.size() != 1){
		printf ("Error: Stack not empty at the end of parsing\n");
		exit(0);
	} else if (lex->getNextToken()->tokType != TOKEN_EOF){
		printf ("Error: Parsing finished input still remaining\n");
		exit(0);
	}
	if (PARSERLOGS) printf ("Parsing Finished. AST Generated\n");
}

bool parser::isKeyword(string val){
	// Check if the given value is a keyword
    if (val == "in" || val == "where" || val == "." || val == "aug" || val == "and" || val == "or"
		|| val == "&" || val == "not" || val == "gr" || val == "ge" || val == "ls" || val == "le"
		|| val == "eq" || val == "ne" || val == "+" || val == "-" || val == "*" || val == "/"
		|| val == "**" || val == "@" || val == "within" || val == "rec" || val == "let" || val == "fn")
    return true;
    else
      return false;
}

void parser::read(string tokStr){
	// Read the next token and compare it with the expected token string
    if (PARSERLOGS) printf ("read() args %s\\, match %s\\\n", tokStr.c_str(), nextToken->tokValue.c_str());
    if (!(nextToken->tokValue == tokStr)){
        printf ("At L:C::%d:%d Expected '%s', received '%s'\n", nextToken->lineCount, nextToken->charCount,
        tokStr.c_str(), nextToken->tokValue.c_str());
        exit(0);
    }
    do {
        nextToken = lex->getNextToken();
    }while (nextToken->tokType == TOKEN_DELETE);
}

void parser::buildTree(string nodeStr, int numChildNodes, int type){
	// Build a tree node with the given node string, number of child nodes, and type
    if (PARSERLOGS) printf ("buildTree. Current Tree Size = %d, Node to add = %s, numChild = %d\n", (int)treeStack.size(), nodeStr.c_str(), numChildNodes);
    int finalSize = treeStack.size() - numChildNodes + 1;
    treeNode* newNode = new treeNode();
    treeNode* tempNode = new treeNode();
    newNode->nodeString = nodeStr;
    newNode->type = type;
    if (numChildNodes == 0){
        treeStack.push(newNode);
        if (PARSERLOGS) printf ("New Node added. Stack Size = %d\n", (int)treeStack.size());
        return;
    }
    if (treeStack.empty()){
        if (PARSERLOGS) printf ("We were asked to pop but the stack is empty\n");
        return;
    } else {
        while ((numChildNodes - 1) > 0){
            if (!treeStack.empty()){
                tempNode = treeStack.top();
                treeStack.pop();
                if (treeStack.size() != 0){
                    treeStack.top()->siblingNode = tempNode;
                } else if (treeStack.size() == 0) { 
                    printf ("Parse Error: Empty Stack\n");
                    exit(0);
                }
                numChildNodes--;
            } else {
                if (PARSERLOGS) printf ("Stack size is less than numChildNodes. Abort.\n");
                return;
            }
        }
        tempNode = treeStack.top();
        newNode->childNode = tempNode;
        treeStack.pop();
    }
    treeStack.push(newNode);
    if (finalSize != treeStack.size()){
        if (PARSERLOGS) printf ("Something went horribly wrong in build tree function\n");
    }
    if (PARSERLOGS) printf ("BuildTree Success. Stack Size = %d\n", (int)treeStack.size());
}

string parser::to_s(treeNode* node)
// Convert a tree node to a string representation
 {
   string str ;
   switch(node->type)
   {
   case treeNode::IDENTIFIER:
       return "<ID:" + node->nodeString + ">";
   case treeNode::INTEGER:
       return "<INT:" + node->nodeString + ">";
   case treeNode::STRING:
       return "<STR:" + node->nodeString + ">";
   default:
       return node->nodeString;
   }
   return NULL;
 }

void parser::printTree(treeNode* topNode, int numDots){
	// Print the tree structure recursively
	int numDots1 = numDots;
	while (numDots1 > 0) {
		printf (".");
		numDots1--;
	}
	printf ("%s\n", to_s(topNode).c_str());

	if (topNode->childNode != NULL){
		printTree(topNode->childNode, numDots + 1);
	}
	if (topNode->siblingNode != NULL){
		printTree(topNode->siblingNode, numDots);
	}
}

void parser::E(){
	// Parse E production
	if (PARSERLOGS) printf ("Proc E next token = %s\n", nextToken->tokValue.c_str());
	if (nextToken->tokValue == "let")
	{
		read("let");
		D();
		read ("in");
		E();
		buildTree("let", 2, treeNode::LET);
	} else if (nextToken->tokValue == "fn"){
		read("fn");
		int n = 0;
		do {
			Vb();
			n++;
		}while(nextToken->tokValue == "(" || nextToken->tokType == TOKEN_IDENTIFIER);
		read(".");
		E();
		buildTree("lambda", n+1, treeNode::LAMBDA);
	} else {
		Ew();
	}
}

 void parser::Ew(){
	// Parse Ew production
	if (PARSERLOGS) printf ("Proc Ew next token = %s\n", nextToken->tokValue.c_str());
 	T();
 	if (nextToken->tokValue == "where")
 	{
 		read("where");
 	 	Dr();
 	 	buildTree("where",2, treeNode::WHERE);
 	}
 }

 void parser::T(){
	// Parse T production
	 if (PARSERLOGS) printf ("Proc T next token = %s\n", nextToken->tokValue.c_str());
	 int n = 0;
	 Ta();
	 if (nextToken->tokValue == ","){
		 do {
			 read(",");
			 Ta();
			 n++;
		 }while (nextToken->tokValue == ",");
		 buildTree("tau", n+1, treeNode::TAU);
	 }
 }

 void parser::Ta(){
	// Parse Ta production
	 if (PARSERLOGS) printf ("Proc Ta next token = %s\n", nextToken->tokValue.c_str());
	 Tc();
	 while (nextToken->tokValue == "aug"){
		 read("aug");
		 Tc();
		 buildTree("aug", 2, treeNode::AUG);
	 }
 }

 void parser::Tc(){
	// Parse Tc production
	 if (PARSERLOGS) printf ("Proc Tc next token = %s\n", nextToken->tokValue.c_str());
	 B();
	 if (nextToken->tokValue == "->"){
		 read("->");
		 Tc();
		 read("|");
		 Tc();
		 buildTree("->", 3, treeNode::TERNARY);
	 }
 }

void parser::B(){
	// Parse B production
	if (PARSERLOGS) printf ("Proc B next token = %s\n", nextToken->tokValue.c_str());
	Bt();
	while (nextToken->tokValue == "or"){
		read ("or");
		Bt();
		buildTree("or", 2, treeNode::OR);
	}
}

void parser::Bt(){
	// Parse Bt production
	if (PARSERLOGS) printf ("Proc Bt next token = %s\n", nextToken->tokValue.c_str());
	Bs();
	while (nextToken->tokValue == "&"){
		read("&");
		Bs();
		buildTree("&", 2, treeNode::AND_LOGICAL);
	}
}

void parser::Bs(){
	// Parse Bs production
	if (PARSERLOGS) printf ("Proc Bs next token = %s\n", nextToken->tokValue.c_str());
	if (nextToken->tokValue == "not"){
		read ("not");
		Bp();
		buildTree("not", 1, treeNode::NOT);
	}else {
		Bp();
	}
}

void parser::Bp(){
	// Parse Bp production
	if (PARSERLOGS) printf ("Proc Bp next token = %s\n", nextToken->tokValue.c_str());
	A();
	if (nextToken->tokValue == "gr" || nextToken->tokValue == ">"){
		if (nextToken->tokValue == "gr")
			read("gr");
		else if (nextToken->tokValue == ">")
			read(">");
		A();
		buildTree("gr", 2, treeNode::GR);
	}else if (nextToken->tokValue == "ge" || nextToken->tokValue == ">="){
		if (nextToken->tokValue == "ge")
			read("ge");
		else if (nextToken->tokValue == ">=")
			read(">=");
		A();
		buildTree("ge", 2, treeNode::GE);
	}else if (nextToken->tokValue == "ls" || nextToken->tokValue == "<"){
		if (nextToken->tokValue == "ls")
			read("ls");
		else if (nextToken->tokValue == "<")
			read("<");
		A();
		buildTree("ls", 2, treeNode::LS);
	}else if (nextToken->tokValue == "le" || nextToken->tokValue == "<="){
		if (nextToken->tokValue == "le")
			read("le");
		else if (nextToken->tokValue == "<=")
			read("<=");
		A();
		buildTree("le", 2, treeNode::LE);
	}else if (nextToken->tokValue == "eq"){
		read("eq");
		A();
		buildTree("eq", 2, treeNode::EQ);
	}else if (nextToken->tokValue == "ne"){
		read("ne");
		A();
		buildTree("ne", 2, treeNode::NE);
	}
}

void parser::A(){
	// Parse A production
	if (PARSERLOGS) printf ("Proc A next token = %s\n", nextToken->tokValue.c_str());
	string treeStr;
	if (nextToken->tokValue == "+"){
		read("+");
		At();
	} else if (nextToken->tokValue == "-"){
		read("-");
		At();
		buildTree ("neg", 1, treeNode::NEG);
	} else {
		At();
	}
	while (nextToken->tokValue == "+" || nextToken->tokValue == "-"){
		if (nextToken->tokValue == "+"){
			read("+");
			treeStr = "+";
		} else {
			read("-");
			treeStr = "-";
		}
		At();
		buildTree(treeStr, 2, treeStr == "+" ? treeNode::ADD: treeNode::SUBTRACT);
	}
}

void parser::At(){
	// Parse At production
	if (PARSERLOGS) printf ("Proc At next token = %s\n", nextToken->tokValue.c_str());
	string treeStr;
	Af();
	while(nextToken->tokValue == "*" || nextToken->tokValue == "/"){
		if (nextToken->tokValue == "*"){
			read("*");
			treeStr = "*";
		} else {
			read("/");
			treeStr = "/";
		}
		Af();
		buildTree(treeStr, 2, treeStr == "*" ? treeNode::MULTIPLY: treeNode::DIVIDE);
	}
}

void parser::Af(){
	// Parse Af production
	if (PARSERLOGS) printf ("Proc Af next token = %s\n", nextToken->tokValue.c_str());
	Ap();
	if (nextToken->tokValue == "**"){
		read("**");
		Af();
		buildTree("**", 2, treeNode::EXPONENTIAL);
	}
}

void parser::Ap(){
	// Parse Ap production
	if (PARSERLOGS) printf ("Proc Ap next token = %s\n", nextToken->tokValue.c_str());
	R();
	while (nextToken->tokValue == "@"){
		read("@");
		buildTree(nextToken->tokValue, 0, treeNode::IDENTIFIER);
		read(nextToken->tokValue);
		R();
		buildTree("@", 3, treeNode::AT);
	}
}

void parser::R(){
	// Parse R production
	if (PARSERLOGS) printf ("Proc R next token = %s\n", nextToken->tokValue.c_str());
	Rn();
	while ( (TOKEN_IDENTIFIER == nextToken->tokType  || TOKEN_INTEGER == nextToken->tokType|| TOKEN_STRING == nextToken->tokType
			||"(" == nextToken->tokValue || "false" == nextToken->tokValue
			|| "true" == nextToken->tokValue || "nil" == nextToken->tokValue || "dummy" == nextToken->tokValue) && !isKeyword(nextToken->tokValue)){
		Rn();
		buildTree("gamma", 2, treeNode::GAMMA);
	}
}

void parser::Rn(){
	// Parse Rn production
	if (PARSERLOGS) printf ("Proc Rn next token = %s\n", nextToken->tokValue.c_str());
	if("(" == nextToken->tokValue){
		read("(");
		E();
		read(")");
	}
	else if(TOKEN_IDENTIFIER == nextToken->tokType || TOKEN_INTEGER == nextToken->tokType || TOKEN_STRING == nextToken->tokType){
		if("true" == nextToken->tokValue){
			read("true");
			buildTree("<true>", 0, treeNode::TRUE);
		}
		else if("false" == nextToken->tokValue){
			read("false");
			buildTree("<false>", 0, treeNode::FALSE);
		}
		else if("nil" == nextToken->tokValue){
			read("nil");
			buildTree("<nil>", 0, treeNode::NIL);
		}
		else if("dummy" == nextToken->tokValue){
			read("dummy");
			buildTree("<dummy>",0, treeNode::DUMMY);
		}
		else if(TOKEN_IDENTIFIER == nextToken->tokType){
			buildTree(nextToken->tokValue, 0, treeNode::IDENTIFIER);
			read(nextToken->tokValue);
			if (PARSERLOGS) printf ("next Token = %s\n", nextToken->tokValue.c_str());
		}
		else if(TOKEN_STRING == nextToken->tokType){
			buildTree(nextToken->tokValue, 0, treeNode::STRING);
			read(nextToken->tokValue);
		}
		else if(TOKEN_INTEGER == nextToken->tokType){
			buildTree(nextToken->tokValue, 0, treeNode::INTEGER);
			read(nextToken->tokValue);
		}
	}
}

void parser::D(){
	// Parse D production
	if (PARSERLOGS) printf ("Proc D next token = %s\n", nextToken->tokValue.c_str());
	Da();
	if (nextToken->tokValue == "within"){
		read("within");
		D();
		buildTree("within", 2, treeNode::WITHIN);
	}
}

void parser::Da(){
	// Parse Da production
	if (PARSERLOGS) printf ("Proc Da next token = %s\n", nextToken->tokValue.c_str());
	int n = 0;
	Dr();
	while (nextToken->tokValue == "and"){
		read("and");
		Dr();
		n++;
	}
	if (n >0)
		buildTree("and", n+1, treeNode::AND);
}

void parser::Dr(){
	// Parse Dr production
	if (PARSERLOGS) printf ("Proc Dr next token = %s\n", nextToken->tokValue.c_str());
	if (nextToken->tokValue == "rec"){
		read("rec");
		Db();
		buildTree("rec", 1, treeNode::REC);
	} else {
		Db();
	}
}

void parser::Db(){
	// Parse Db production
	if (PARSERLOGS) printf ("Proc Db next token = %s\n", nextToken->tokValue.c_str());
	if (nextToken->tokValue == "("){
		read("(");
		D();
		read(")");
	} else if (nextToken->tokType == TOKEN_IDENTIFIER){
		buildTree(nextToken->tokValue, 0, treeNode::IDENTIFIER);
		read(nextToken->tokValue);
		if (nextToken->tokValue == "," || nextToken->tokValue == "="){
			Vl();
			read("=");
			E();
			buildTree ("=", 2, treeNode::BINDING);
		} else {
			int n = 0;
			do {
				Vb();
				n++;
			}while (nextToken->tokValue == "(" || nextToken->tokType == TOKEN_IDENTIFIER);
			read("=");
			E();
			buildTree("function_form", n+2, treeNode::FCN_FORM); 
		}
	}
}

void parser::Vb(){
	// Parse Vb production
	if (PARSERLOGS) printf ("Proc Vb next token = %s\n", nextToken->tokValue.c_str());
	if(nextToken->tokType == TOKEN_IDENTIFIER){
		buildTree(nextToken->tokValue, 0, treeNode::IDENTIFIER);
		read(nextToken->tokValue);
	}else if (nextToken->tokValue == "("){
		read("(");
		if (nextToken->tokValue == ")"){
			read(")");
			buildTree("()", 0, treeNode::PARANTHESES);
		} else if (nextToken->tokType == TOKEN_IDENTIFIER){
			buildTree(nextToken->tokValue, 0, treeNode::IDENTIFIER);
			read(nextToken->tokValue);
			Vl();
			read(")");
		}
	}
}

void parser::Vl(){
	// Parse Vl production
	if (PARSERLOGS) printf ("Proc Vl next token = %s\n", nextToken->tokValue.c_str());
	int n = 0;
	while (nextToken->tokValue == ","){
		read(",");
		if (nextToken->tokType == TOKEN_IDENTIFIER){
			buildTree(nextToken->tokValue, 0, treeNode::IDENTIFIER);
			read(nextToken->tokValue);
			n++;
		} else {
			printf ("ERROR In Vl()\n");
		}
	}
	if (n > 0)
		buildTree(",", n+1, treeNode::COMMA);
}

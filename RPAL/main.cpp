#include <stdio.h>
#include <string.h>
#include <fstream>
#include "Lexer.h"
#include "Parser.h"

using namespace std;

ifstream source;
int option = 0x0000;
enum cmdOpts {
    OPT_ST = 0x0100,
    OPT_AST = 0x1000
};

bool isEvaluateProg = true;

void exec() {
    lexer* rpalLexer = new lexer(&source);
    parser* rpalParser = new parser(rpalLexer);

    // Print Abstract Syntax Tree (AST) if the corresponding option is set
    if (option & OPT_AST) {
        rpalParser->printAST();
    }

    // Print Symbol Table (ST) if the corresponding option is set
    if (option & OPT_ST) {
        rpalParser->printST();
    }

    // Evaluate the program if enabled
    if (isEvaluateProg) {
        rpalParser->evaluateProg();
    }
}

int main(int argc, char* argv[]) {
    if (argc == 2 || argc == 3) {
        char* filename = argv[1];
        source.open(filename);
        if (!source.good()) {
            printf("File \"%s\" not found\n", filename);
            return 0;
        }

        if (argc == 3) {
            if (strcmp(argv[2], "-ast") == 0) {
                option |= OPT_AST;
                isEvaluateProg = false;
            } else if (strcmp(argv[2], "-st") == 0) {
                option |= OPT_ST;
                isEvaluateProg = false;
            } else {
                printf("Invalid Option\n");
                return 0;
            }
        }
        exec();
        source.close();
    } else {
        printf("\nTry: ./myrpal <filename>\n");
		printf("Try: ./myrpal <filename> -ast\n");
        printf("Try: ./myrpal <filename> -st\n\n");
    }
}

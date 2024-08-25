#ifndef RPAL_COMPILER_ENVIRONMENT_H_
#define RPAL_COMPILER_ENVIRONMENT_H_

#include <map>
#include <string>
#include "Controller.h"

using namespace std;

class Environment {
public:
    int id; // Unique identifier for the environment
    Environment *parent; // Pointer to the parent environment
    void assignParent(Environment* ); // Assigns the parent environment
    Control* lookup(string); // Looks up a symbol in the environment
    void envPrint(); // Prints the contents of the environment
    Environment(int); // Constructor for Environment class
    virtual ~Environment(); // Destructor for Environment class
    map<string, Control *> symbolTable; // Symbol table for storing variables
};

#endif /* RPAL_COMPILER_ENVIRONMENT_H_ */

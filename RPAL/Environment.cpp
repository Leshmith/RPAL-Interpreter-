#include "Environment.h"

Environment::Environment(int id) {
    // Constructor for Environment class
    this->id = id;
}

Environment::~Environment() {
    // Destructor for Environment class
}

void Environment::assignParent(Environment* parent){
    // Assigns the parent environment
    this->parent = parent;
}

Control* Environment::lookup(string str) {
    // Looks up a symbol in the environment
    map<string, Control *>::iterator it;
    it = symbolTable.find(str);
    if(symbolTable.end() == it)
        return parent->lookup(str);
    else
        return (*it).second;
}

void Environment::envPrint(){
    // Prints the contents of the environment
    map<string, Control *>::iterator it;
    for (it=symbolTable.begin() ; symbolTable.end() != it; it++ )
        cout << (*it).first << " => " << (*it).second->toStr() << endl;
}

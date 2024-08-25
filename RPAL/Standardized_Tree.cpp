#include "Standardized_Tree.h"

// Constructor to initialize the tree and standardize the given top node.
Standardized_Tree::Standardized_Tree(treeNode* topNode) {
    standardize(topNode);
}

// Destructor for Standardized_Tree.
Standardized_Tree::~Standardized_Tree() {
}

// Function to standardize a LET node.
void Standardized_Tree::standardizeLET(treeNode* topNode){
    treeNode* p;
    if (STANDARDIZERLOGS) printf ("CASE LET\n");

    // Check if the child node is a BINDING type.
    if(treeNode::BINDING != topNode->childNode->type){
        printf ("Subtree not standardised: case LET");
    }

    // Change the type and string of the top node.
    topNode->type = treeNode::GAMMA;
    topNode->nodeString = "gamma";
    topNode->childNode->type = treeNode::LAMBDA;
    topNode->childNode->nodeString = "lambda";

    // Rearrange the child and sibling nodes.
    p = topNode->childNode->siblingNode;
    topNode->childNode->siblingNode = topNode->childNode->childNode->siblingNode;
    topNode->childNode->childNode->siblingNode = p;
}

// Function to standardize a WHERE node.
void Standardized_Tree::standardizeWHERE(treeNode* topNode){
    treeNode* p, *x, *e;
    if (STANDARDIZERLOGS) printf ("CASE WHERE\n");

    // Check if the sibling node is a BINDING type.
    if(treeNode::BINDING != topNode->childNode->siblingNode->type){
      printf("Subtree not standardised: case WHERE");
    }

    // Change the type and string of the top node.
    topNode->type = treeNode::GAMMA;
    topNode->nodeString = "gamma";

    // Rearrange the child and sibling nodes.
    p = topNode->childNode;
    x = topNode->childNode->siblingNode->childNode;
    e = topNode->childNode->siblingNode->childNode->siblingNode;
    p->siblingNode = NULL;
    x->siblingNode = NULL;

    // Create a new lambda node and rearrange pointers.
    topNode->childNode = new treeNode();
    topNode->childNode->type = treeNode::LAMBDA;
    topNode->childNode->nodeString = "lambda";
    topNode->childNode->siblingNode = e;
    topNode->childNode->childNode = x;
    x->siblingNode = p;
}

// Function to standardize a WITHIN node.
void Standardized_Tree::standardizeWITHIN(treeNode* topNode){
    treeNode *x1, *x2, *e1;
    if (STANDARDIZERLOGS) printf ("CASE WITHIN\n");

    // Check if both the child and sibling nodes are of BINDING type.
    if(treeNode::BINDING != topNode->childNode->type || treeNode::BINDING != topNode->childNode->siblingNode->type){
        printf("Subtree not standardised: case WITHIN");
    }

    // Change the type of the top node.
    topNode->type = treeNode::BINDING;

    // Rearrange the child and sibling nodes.
    x1 = topNode->childNode->childNode;
    x2 = topNode->childNode->siblingNode->childNode;
    e1 = x1->siblingNode;
    x1->siblingNode = x2->siblingNode;

    // Create new nodes and rearrange pointers.
    x2->siblingNode = new treeNode();
    x2->siblingNode->type = treeNode::GAMMA;
    x2->siblingNode->nodeString = "gamma";
    x2->siblingNode->childNode = new treeNode();
    x2->siblingNode->childNode->type = treeNode::LAMBDA;
    x2->siblingNode->childNode->nodeString = "lambda";
    x2->siblingNode->childNode->siblingNode = e1; 
    x2->siblingNode->childNode->childNode = x1; 
    topNode->childNode = x2;
}

// Function to standardize a REC node.
void Standardized_Tree::standardizeREC(treeNode* topNode){
    treeNode *x, *e, *temp;
    if (STANDARDIZERLOGS) printf ("CASE REC\n");

    // Check if the child node is of BINDING type.
    if(treeNode::BINDING != topNode->childNode->type){
        printf("Subtree not standardised: case REC");
    }

    // Change the type of the top node.
    topNode->type = treeNode::BINDING;

    // Rearrange the child and sibling nodes.
    x = topNode->childNode->childNode;
    e = topNode->childNode->childNode->siblingNode;
    x->siblingNode = NULL;
    topNode->childNode = x; 

    // Create new nodes and rearrange pointers.
    temp = new treeNode();
    temp->type = treeNode::GAMMA;
    temp->nodeString = "gamma";
    x->siblingNode = temp;
    temp->childNode = new treeNode();
    temp->childNode->type = treeNode::YSTAR;
    temp->childNode->nodeString = "<Y*>";
    temp->childNode->siblingNode = new treeNode();
    temp->childNode->siblingNode->type = treeNode::LAMBDA;
    temp->childNode->siblingNode->nodeString = "lambda";
    temp->childNode->siblingNode->childNode = new treeNode();
    temp->childNode->siblingNode->childNode->type = x->type;
    temp->childNode->siblingNode->childNode->nodeString = x->nodeString;
    temp->childNode->siblingNode->childNode->siblingNode = e;
}

// Function to standardize a function form node.
void Standardized_Tree::standardizeFCNFORM(treeNode* topNode){
    treeNode *temp, *newNode;
    if (STANDARDIZERLOGS) printf ("CASE FCN_FORM\n");

    // Change the type and string of the top node.
    topNode->type = treeNode::BINDING;
    topNode->nodeString = "=";

    // Iterate through the siblings and create lambda nodes.
    temp = topNode->childNode;
    while (temp->siblingNode->siblingNode != NULL){
        newNode = new treeNode();
        if (STANDARDIZERLOGS) printf ("newNOde allocated at %p\n", newNode);
        newNode->type = treeNode::LAMBDA;
        newNode->nodeString = "lambda";
        newNode->childNode = temp->siblingNode;
        temp->siblingNode = newNode;
        temp = newNode->childNode;
    }
}

// Function to standardize a LAMBDA node.
void Standardized_Tree::standardizeLAMBDA(treeNode* topNode){
    treeNode *temp, *newNode;
    if (STANDARDIZERLOGS) printf ("CASE LAMBDA\n");

    // Iterate through the siblings and create lambda nodes.
    temp = topNode->childNode;
    while (temp->siblingNode->siblingNode != NULL){
        newNode = new treeNode();
        newNode->type = treeNode::LAMBDA;
        newNode->nodeString = "lambda";
        newNode->childNode = temp->siblingNode;
        temp->siblingNode = newNode;
        temp = temp->siblingNode;
    }
}

// Function to standardize an AND node.
void Standardized_Tree::standardizeAND(treeNode* topNode){
    treeNode* temp;
    treeNode* tauNode, *commaNode, **currentTau, **currentComma;
    if (STANDARDIZERLOGS) printf ("CASE AND\n");

    // Change the type and string of the top node.
    topNode->type = treeNode::BINDING;
    topNode->nodeString = "=";

    // Initialize the tau and comma nodes.
    temp = topNode->childNode;
    tauNode = new treeNode();
    commaNode = new treeNode();
    tauNode->type = treeNode::TAU;
    tauNode->nodeString = "tau";
    tauNode->childNode = NULL;
    tauNode->siblingNode = NULL;
    commaNode->type = treeNode::COMMA;
    commaNode->nodeString = ",";
    commaNode->childNode = NULL;
    commaNode->siblingNode = NULL;

    // Set the child nodes.
    topNode->childNode = commaNode;
    topNode->childNode->siblingNode = tauNode;
    currentTau = &(tauNode->childNode);
    currentComma = &(commaNode->childNode);

    // Rearrange the child and sibling nodes.
    while (temp != NULL){
        *currentTau = temp->childNode->siblingNode;
        temp->childNode->siblingNode = NULL;
        *currentComma = temp->childNode;
        temp = temp->siblingNode;
        currentComma = &((*currentComma)->siblingNode);  
        currentTau = &((*currentTau)->siblingNode);
    }
}

// Function to standardize an AT node.
void Standardized_Tree::standardizeAT(treeNode* topNode){
    treeNode* E1, *N, *E2;
    if (STANDARDIZERLOGS) printf ("CASE AT\n");

    // Get the child and sibling nodes.
    E1 = topNode->childNode;
    N = E1->siblingNode;
    E2 = N->siblingNode;

    // Change the type and string of the top node.
    topNode->type = treeNode::GAMMA;
    topNode->nodeString = "gamma";

    // Create a new gamma node and rearrange pointers.
    topNode->childNode = new treeNode();
    topNode->childNode->type = treeNode::GAMMA;
    topNode->childNode->nodeString = "gamma";
    topNode->childNode->siblingNode = E2;
    topNode->childNode->childNode = N;
    N->siblingNode = NULL;
    topNode->childNode->childNode->siblingNode = E1;
    E1->siblingNode = NULL;
}

// Recursive function to standardize the entire tree starting from the top node.
void Standardized_Tree::standardize(treeNode* topNode){
    treeNode *p, *e, *e1, *e2, *x, *x1, *x2, *n, *temp, *new_temp;
    string origString = topNode->nodeString;
    if (STANDARDIZERLOGS) printf ("Standardizing Node = %s\n", topNode->nodeString.c_str());

    // Recursively standardize the child and sibling nodes.
    if (topNode->childNode != NULL)
        standardize(topNode->childNode);
    if (topNode->siblingNode != NULL)
        standardize (topNode->siblingNode);

    // Switch case to handle different node types.
    switch(topNode->type){
    case treeNode::LET:
        standardizeLET(topNode);
        break;
    case treeNode::WHERE:
        standardizeWHERE(topNode);
        break;
    case treeNode::WITHIN:
        standardizeWITHIN(topNode);
        break;
    case treeNode::REC:
        standardizeREC(topNode);
        break;
    case treeNode::FCN_FORM:
        standardizeFCNFORM(topNode);
        break;
    case treeNode::LAMBDA:
        standardizeLAMBDA(topNode);
        break;
    case treeNode::AND:
        standardizeAND(topNode);
        break;
    case treeNode::AT:
        standardizeAT(topNode);
        break;
    default:
        if (0) printf ("Nothing to do. TopNode is %s\n", origString.c_str());
    }
    if (0) printf ("Done with node %s\n", origString.c_str());
}

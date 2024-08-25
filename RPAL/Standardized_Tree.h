#ifndef Standardized_Tree_H_
#define Standardized_Tree_H_

#include "Lexer.h"
#include "TreeNode.h"

using namespace std;

class Standardized_Tree {
    // Private methods for standardization
    void standardize(treeNode*);
    void standardizeLET(treeNode*);
    void standardizeWHERE(treeNode*);
    void standardizeWITHIN(treeNode*);
    void standardizeREC(treeNode*);
    void standardizeFCNFORM(treeNode*);
    void standardizeLAMBDA(treeNode*);
    void standardizeAND(treeNode*);
    void standardizeAT(treeNode*);

public:
    // Constructor taking a topNode as an argument
    Standardized_Tree(treeNode* topNode);

    // Virtual destructor (useful for inheritance)
    virtual ~Standardized_Tree();
};

#endif /* Standardized_Tree_H_ */

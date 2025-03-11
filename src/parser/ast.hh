#ifndef AST_HH
#define AST_HH

#include <memory>
#include <iostream>

using namespace std;
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void print() const = 0;
};

class NumberNode : public ASTNode {
public:
    int value;
    NumberNode(int);
    void print() const;
};

class BinaryOpNode : public ASTNode {
public:
    char op;
    ASTNode *left, *right;
    BinaryOpNode(char, ASTNode *, ASTNode *);
    void print() const;
};

#endif

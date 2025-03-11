#include "ast.hh"

using namespace std;

NumberNode::NumberNode(int val) : value(val) {}

void NumberNode::print() const { cout << value; }

BinaryOpNode::BinaryOpNode(char oper, ASTNode *lhs, ASTNode *rhs)
    : op(oper), left(move(lhs)), right(move(rhs)) {}

void BinaryOpNode::print() const {
    cout << "(";
    left->print();
    cout << " " << op << " ";
    right->print();
    cout << ")";
}

#ifndef CODEGEN_H
#define CODEGEN_H

#include <stack>

using namespace std;
using namespace ast;

// namespace codegen {
class Visitor {
protected:
    stack<string> strings;
    static string pop(stack<string>&);
    virtual string string_of_type(Type) = 0;

public:
    virtual void visitDeclaration(Declaration&) = 0;
    virtual void visitDeclarationList(DeclarationList&) = 0;
    virtual void visitEmpty(Empty&) = 0;
    virtual void visitNum(Num&) = 0;
    virtual void visitVar(Var&) = 0;
    virtual void visitAddExpression(AddExpression&) = 0;
    virtual void visitSubExpression(SubExpression&) = 0;
    virtual void visitMulExpression(MulExpression&) = 0;
    virtual void visitDivExpression(DivExpression&) = 0;
    virtual void visitFunctionCall(FunctionCall&) = 0;
    virtual void visitSkipStatement(SkipStatement&) = 0;
    virtual void visitAssignmentStatement(AssignmentStatement&) = 0;
    virtual void visitSequenceStatement(SequenceStatement&) = 0;
    virtual void visitBranchStatement(BranchStatement&) = 0;
    virtual void visitLoopStatement(LoopStatement&) = 0;
    virtual void visitBlockStatement(BlockStatement&) = 0;
    virtual void visitReturnStatement(ReturnStatement&) = 0;
    virtual void visitFunctionDefinition(FunctionDefinition&) = 0;
    virtual void visitParameterList(DeclarationList&) = 0;
    virtual void visitFunctionDefinitionList(FunctionDefinitionList&) = 0;

    virtual void visitProgram(Program&) = 0;
    void visitExpression(Expression&);
    void visitStatement(Statement&);
    string retrieve();
};

class CodeGenerator {
protected:
    Visitor *visitor;
    static string indent(string, string, int);
public:
    CodeGenerator(Visitor *v);
    string generateCode(Program&);
    virtual ~CodeGenerator();
};


class CVisitor : public Visitor {
public:
    virtual string string_of_type(Type);
    virtual void visitDeclaration(Declaration&);
    virtual void visitDeclarationList(DeclarationList&);
    virtual void visitEmpty(Empty&);
    virtual void visitNum(Num&);
    virtual void visitVar(Var&);
    virtual void visitAddExpression(AddExpression&);
    virtual void visitSubExpression(SubExpression&);
    virtual void visitMulExpression(MulExpression&);
    virtual void visitDivExpression(DivExpression&);
    virtual void visitFunctionCall(FunctionCall&);
    virtual void visitSkipStatement(SkipStatement&);
    virtual void visitAssignmentStatement(AssignmentStatement&);
    virtual void visitSequenceStatement(SequenceStatement&);
    virtual void visitBranchStatement(BranchStatement&);
    virtual void visitLoopStatement(LoopStatement&);
    virtual void visitBlockStatement(BlockStatement&);
    virtual void visitReturnStatement(ReturnStatement&);
    virtual void visitFunctionDefinition(FunctionDefinition&);
    virtual void visitParameterList(DeclarationList&);
    virtual void visitFunctionDefinitionList(FunctionDefinitionList&);

    virtual void visitProgram(Program&);
};

class CCodeGenerator : public CodeGenerator {
public:
    CCodeGenerator() : CodeGenerator(new CVisitor()) {}
};

// } // namespace codegen
#endif

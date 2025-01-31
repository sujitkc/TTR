#include <iostream>
#include <vector>

#include "ast.hh" 
#include "codegen.hh"

using namespace std;

namespace ast {

Declaration::Declaration(string n, Type t) : variable(n), type(t) {}

string Declaration::getVariable() { return variable; }

Type Declaration::getType() { return type; }

DeclarationList::DeclarationList(vector<Declaration *>& d) : declarations (d) {}

vector<Declaration *>& DeclarationList::getDeclarations() { return declarations; }

DeclarationList::~DeclarationList() {
    for(auto& d : declarations) {
        delete(d);
    }
}

Expression::Expression(ExpressionType et) : exprtype (et) {}
Expression::~Expression() {}

Empty::Empty() : Expression(EMPTY) {}

Num::Num(int v) : Expression(NUM), value(v) {}

Var::Var(string n) : Expression(VAR), name(n) {}

BinaryExpression::BinaryExpression(Expression *l, Expression *r, ExpressionType ty) :
    Expression(ty), left(l), right(r){}

BinaryExpression::~BinaryExpression() {
    delete left;
    delete right;
}

Expression& BinaryExpression::getLeft() { return *left; }

Expression& BinaryExpression::getRight() { return *right; }

AddExpression::AddExpression(Expression *l, Expression *r) : BinaryExpression(l, r, ADD) {}

SubExpression::SubExpression(Expression *l, Expression *r) : BinaryExpression(l, r, SUB) {}

MulExpression::MulExpression(Expression *l, Expression *r) : BinaryExpression(l, r, MUL) {}

DivExpression::DivExpression(Expression *l, Expression *r) : BinaryExpression(l, r, DIV) {}

FunctionCall::FunctionCall(string n, vector<Expression *>a) : 
    Expression(FUNCTIONCALL), name(n), arguments(a) {}

string FunctionCall::getName() {
    return name;
}

vector<Expression *> FunctionCall::getArguments() {
    return arguments;
}

FunctionCall::~FunctionCall() {
    for(auto& arg : arguments) {
        delete arg;
    }
}

Statement::Statement(StatementType ty) : stmttype (ty) {}

Statement::~Statement() {}

SkipStatement::SkipStatement() : Statement(SKIP) {}

SkipStatement::~SkipStatement() {}

AssignmentStatement::AssignmentStatement(string vname, Expression *e) :
           Statement(ASSIGN), variable(vname), expression(e) {}


AssignmentStatement::~AssignmentStatement() {
    delete(expression);
}

string AssignmentStatement::getVariable() {
    return variable;
}

Expression *AssignmentStatement::getExpression() {
    return expression;
}

SequenceStatement::SequenceStatement(vector<Statement *> s) :
    Statement(SEQUENCE), statements(s) {}

SequenceStatement::SequenceStatement() : Statement(SEQUENCE) {}

void SequenceStatement::addStatement(Statement *s) {
    statements.push_back(s);
}

vector<Statement *>& SequenceStatement::getStatements() {
    return statements;
}

SequenceStatement::~SequenceStatement() {
    for(auto &s : statements) {
        delete s;
    }
}

BlockStatement::BlockStatement(DeclarationList * d, Statement *s) :
    Statement(BLOCK), declarations(d), statement(s) {}

BlockStatement::BlockStatement(Statement *s) :
    Statement(BLOCK), statement(s) {
        vector<Declaration *>v;
        declarations = new DeclarationList(v);   
}

DeclarationList& BlockStatement::getDeclarations() {
    return *declarations;
}

Statement& BlockStatement::getStatement() {
    return *statement;
}

BlockStatement::~BlockStatement() {
    delete declarations;
    delete statement;
}

BranchStatement::BranchStatement(Expression *c, Statement *t, Statement *e) :
    Statement(BRANCH), condition(c), thenStatement(t), elseStatement(e) {}

Expression& BranchStatement::getCondition() { return *condition; }

Statement& BranchStatement::getThenStatement() { return *thenStatement; }

Statement& BranchStatement::getElseStatement() { return *elseStatement; }

BranchStatement::~BranchStatement() {
    delete condition;
    delete thenStatement;
    delete elseStatement;
}

LoopStatement::LoopStatement(Expression *c, Statement *b) :
    Statement(LOOP), condition(c), body(b) {}

Expression& LoopStatement::getCondition() { return *condition; }

Statement& LoopStatement::getBody() { return *body; }

LoopStatement::~LoopStatement() {
    delete condition;
    delete body;
}

ReturnStatement::ReturnStatement(Expression *e ) :
    Statement(RETURN), expression(e) {}

Expression& ReturnStatement::getExpression() {
    return *expression;
}

ReturnStatement::~ReturnStatement() {
    delete expression;
}

FunctionDefinition::FunctionDefinition(string n, Type t, 
        DeclarationList *d, BlockStatement *b) :
    name(n), returnType(t), parameters(d), body(b) {}

Type FunctionDefinition::getReturnType() {
    return returnType;
}

string FunctionDefinition::getName() {
    return name;
}

DeclarationList& FunctionDefinition::getParameters() {
    return *parameters;
}

BlockStatement& FunctionDefinition::getBody() {
    return *body;
}

FunctionDefinition::~FunctionDefinition() {
    delete parameters;
    delete body;
}

FunctionDefinitionList::FunctionDefinitionList(
        vector<FunctionDefinition *>& fundefs) :
    functionDefinitions(fundefs) {}

FunctionDefinitionList::FunctionDefinitionList() :
    functionDefinitions(vector<FunctionDefinition *>({})) {}

vector<FunctionDefinition *>& FunctionDefinitionList::getFunctionDefinitions() {
    return functionDefinitions;
}

FunctionDefinitionList::~FunctionDefinitionList() {
    for(auto& fundef : functionDefinitions) {
        delete fundef;
    }
}

Program::Program(string n, DeclarationList *dl,
        FunctionDefinitionList *fdef, Statement *s)
    : name (n), declarations(dl), functionDefinitions(fdef), statement (s) {}

Program::Program()
    : name ("default program"), 
    functionDefinitions (new FunctionDefinitionList()), statement (NULL) {
    
}

string Program::getName() {
    return name;
}

DeclarationList& Program::getDeclarationList() {
    return *declarations;
}

FunctionDefinitionList& Program::getFunctionDefinitionList() {
    return *functionDefinitions;
}

Statement& Program::getStatement() {
    return *statement;
}

Program::~Program() {
    delete declarations;
    delete functionDefinitions;
    delete statement;
}

Program *theProgram;

/////////////////////////// accept methods 
void Declaration::accept(Visitor *visitor) {

}

void DeclarationList::accept(Visitor *visitor) {
    for(int i = declarations.size() - 1; i >= 0; i--) {
        visitor->visitDeclaration(*(declarations[i]));
    }
}

void Empty::accept(Visitor *visitor) {
}

void Num::accept(Visitor *visitor) {
}

void Var::accept(Visitor *visitor) {
}

void AddExpression::accept(Visitor *visitor) {
    visitor->visitExpression(*right);
    visitor->visitExpression(*left);
}


void SubExpression::accept(Visitor *visitor) {
    visitor->visitExpression(*right);
    visitor->visitExpression(*left);
}

void MulExpression::accept(Visitor *visitor) {
    visitor->visitExpression(*right);
    visitor->visitExpression(*left);
}

void DivExpression::accept(Visitor *visitor) {
    visitor->visitExpression(*right);
    visitor->visitExpression(*left);
}

void FunctionCall::accept(Visitor *visitor) {
    for(int i = arguments.size() - 1; i >= 0; i--) {
        visitor->visitExpression(*(arguments[i]));
    }
}

void SkipStatement::accept(Visitor *visitor) {
}

void AssignmentStatement::accept(Visitor *visitor) {
    visitor->visitExpression(*expression);
}

void SequenceStatement::accept(Visitor *visitor) {
    for(int i = statements.size() - 1; i >= 0; i--) {
        visitor->visitStatement(*(statements[i]));
    }
}

void BranchStatement::accept(Visitor *visitor) {
    visitor->visitStatement(*elseStatement);
    visitor->visitStatement(*thenStatement);
    visitor->visitExpression(*condition);
}

void LoopStatement::accept(Visitor *visitor) {
    visitor->visitStatement(*body);
    visitor->visitExpression(*condition);
}

void BlockStatement::accept(Visitor *visitor) {
    visitor->visitStatement(*statement);
    visitor->visitDeclarationList(*declarations);
}

void ReturnStatement::accept(Visitor *visitor) {
    visitor->visitExpression(*expression);
}

void FunctionDefinition::accept(Visitor *visitor) {
    visitor->visitStatement(*body);
    visitor->visitParameterList(*parameters);
}

void FunctionDefinitionList::accept(Visitor *visitor) {
    for(int i = functionDefinitions.size() - 1; i >= 0; i--) {
        visitor->visitFunctionDefinition(*(functionDefinitions[i]));
    }
}

void Program::accept(Visitor *visitor) {
    visitor->visitStatement(*statement);
    visitor->visitFunctionDefinitionList(*functionDefinitions);
    visitor->visitDeclarationList(*declarations);
}

} // namespace ast

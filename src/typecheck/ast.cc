#include <iostream>
#include <vector>

#include "ast.hh" 

using namespace std;

namespace ast {

TypeExpr::TypeExpr(const string n, TypeType tt) : name(n), typeType(tt) {}

string TypeExpr::getName() { return name; }

TypeConst::TypeConst(const string n) : TypeExpr(n, TYPECONST) {}

TypeVar::TypeVar(const string n, const int i)  : TypeExpr(n, TYPEVAR), typeNum(i) {} 

int TypeVar::nextNum = 0;

TypeVar *TypeVar::getNewTypeVar() {
	TypeVar *t = new TypeVar("", TypeVar::nextNum);
	TypeVar::nextNum++;
	return t;
}

int TypeVar::getTypeNum() { return typeNum; }

FunctionType::FunctionType(TypeExpr *rtype, vector<TypeExpr *> pars) : TypeExpr("", FUNCTIONTYPE), returnType(rtype), parameterTypes (pars) {}

TypeExpr& FunctionType::getReturnType() { return *returnType; }

vector<TypeExpr *>& FunctionType::getParameterTypes() { return parameterTypes; }

FunctionType::~FunctionType() {
    for(auto& p : parameterTypes) {
        delete p;
    }
}

Language::Language() {
    nativeTypes.insert(new TypeConst("int"));
    nativeTypes.insert(new TypeConst("bool"));
}

Language *Language::instance = NULL;

Language& Language::getInstance() {
    if(instance == NULL) {
        instance = new Language();
    }
    return *instance;
}

TypeExpr * Language::getNativeType(string typeName) {
    for(auto& ty : nativeTypes) {
        if(ty->getName() == typeName) {
            return ty;
        }
    }
    return NULL;
}

Language::~Language() {
    for(auto& nt : nativeTypes) {
        delete nt;
    }
}

Declaration::Declaration(string n, TypeExpr *t) : variable(n), type(t) {}

string Declaration::getVariable() { return variable; }

TypeExpr& Declaration::getType() { return *type; }

Declaration::~Declaration() {}

DeclarationWithoutType::DeclarationWithoutType(string n) :Declaration(n, TypeVar::getNewTypeVar()) {}
DeclarationWithType::DeclarationWithType(string n, TypeExpr *t) :Declaration(n, t) {}

DeclarationList::DeclarationList(vector<Declaration *>& d) : declarations (d) {}

vector<Declaration *>& DeclarationList::getDeclarations() { return declarations; }

DeclarationList::~DeclarationList() {
    for(auto& d : declarations) {
        delete(d);
    }
}

Expression::Expression(ExpressionType et) : exprtype (et) {}
Expression::~Expression() {}

Num::Num(int v) : Expression(NUM), value(v) {}

BoolConst::BoolConst(bool v) : Expression(BOOL), value(v) {}

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

EqExpression::EqExpression(Expression *l, Expression *r) : BinaryExpression(l, r, EQ) {}

FunctionCall::FunctionCall(string n, vector<Expression *>a) : 
    Expression(FUNCTIONCALL), name(n), arguments(a) {}

string FunctionCall::getName() {
    return name;
}

vector<Expression *>& FunctionCall::getArguments() {
    return arguments;
}

FunctionCall::~FunctionCall() {
    for(auto& arg : arguments) {
        delete arg;
    }
}

Statement::Statement(StatementType ty) : stmttype (ty) {}

Statement::~Statement() {}

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

Program::Program(string n, DeclarationList *dl,
        Statement *s)
    : name (n), declarations(dl), statement (s) {}

Program::Program()
    : name ("default program"), 
    statement (NULL) {
    
}

string Program::getName() {
    return name;
}

DeclarationList& Program::getDeclarationList() {
    return *declarations;
}

Statement& Program::getStatement() {
    return *statement;
}

Program::~Program() {
    delete declarations;
    delete statement;
}

Program *theProgram;
} // namespace ast

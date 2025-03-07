#include <iostream>
#include <vector>

#include "ast.hh" 

using namespace std;

namespace ast {

TypeExpr::TypeExpr(const string n, TypeType tt) : name(n), typeType(tt) {}

string TypeExpr::getName() { return name; }

ostream& operator << (ostream& out, TypeExpr& type) {
    out << type.name;
    return out;
}

bool TypeExpr::operator == (TypeExpr& type) {
    return this == &type;
}

TypeConst::TypeConst(const string n) : TypeExpr(n, TYPECONST) {}

string TypeConst::toString() { return "TypeConst(" + name + ")"; }

TypeVar::TypeVar(const string n, const int i)  : TypeExpr(n, TYPEVAR), typeNum(i) {} 

int TypeVar::nextNum = 0;

TypeVar *TypeVar::getNewTypeVar() {
    TypeVar *t = new TypeVar("", TypeVar::nextNum);
    TypeVar::nextNum++;
    return t;
}

int TypeVar::getTypeNum() { return typeNum; }

string TypeVar::toString() { return "TypeVar(" + name + ", " + to_string(typeNum) + ")"; }

FunctionType::FunctionType(TypeExpr *rtype, vector<TypeExpr *> pars) : TypeExpr("", FUNCTIONTYPE), returnType(rtype), parameterTypes (pars) {}

TypeExpr& FunctionType::getReturnType() { return *returnType; }

vector<TypeExpr *>& FunctionType::getParameterTypes() { return parameterTypes; }

FunctionType::~FunctionType() {
    for(auto& p : parameterTypes) {
        delete p;
    }
}

string FunctionType::toString() {
    string str_rtype = returnType->toString();
    string str_partypes = "";
    for(auto& partype : parameterTypes) {
	    str_partypes += partype->toString() + ", ";
    }
    return "FunctionType(" + str_partypes + " --> " + str_rtype + ")";
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

Declaration::Declaration(string& n, TypeExpr *t) : variable(n), type(t) {}

string& Declaration::getVariable() { return variable; }

TypeExpr& Declaration::getType() { return *type; }

Declaration::~Declaration() {}

DeclarationWithoutType::DeclarationWithoutType(string& n) :Declaration(n, TypeVar::getNewTypeVar()) {}
DeclarationWithType::DeclarationWithType(string& n, TypeExpr *t) :Declaration(n, t) {
/*
            char str[50];
            sprintf(str, "%llx", (long long int)&n);
	    cout << "DeclarationWithType::variable = " << str << endl;
*/
}

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

string Num::toString() {
	cout << "Num::toString" << value << endl;
	char str[50];
	sprintf(str, "%d", value);
	return string(str);
}

BoolConst::BoolConst(bool v) : Expression(BOOL), value(v) {}

string BoolConst::toString() {
	if(value == true) {
		return "true";
	}
	return "false";
}

Var::Var(string& n) : Expression(VAR), name(n) {}

string& Var::getName() {
	return name;
}

string Var::toString() {
	return name;
}

BinaryExpression::BinaryExpression(Expression *l, Expression *r, ExpressionType ty) :
    Expression(ty), left(l), right(r){}

BinaryExpression::~BinaryExpression() {
    delete left;
    delete right;
}

Expression& BinaryExpression::getLeft() { return *left; }

Expression& BinaryExpression::getRight() { return *right; }

AddExpression::AddExpression(Expression *l, Expression *r) : BinaryExpression(l, r, ADD) {}

string AddExpression::toString() {
	return getLeft().toString() + " + " + getRight().toString();
}

EqExpression::EqExpression(Expression *l, Expression *r) : BinaryExpression(l, r, EQ) {}

string EqExpression::toString() {
	return getLeft().toString() + " = " + getRight().toString();
}

FunctionCall::FunctionCall(string& n, vector<Expression *>a) : 
    Expression(FUNCTIONCALL), name(n), arguments(a) {}

string& FunctionCall::getName() {
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

string FunctionCall::toString() {
	string str = name + "(";
	for(auto& arg : arguments) {
		str += arg->toString() + ", ";
	}
	str += ")";
	return str;
}

Statement::Statement(StatementType ty) : stmttype (ty) {}

Statement::~Statement() {}

AssignmentStatement::AssignmentStatement(string& vname, Expression *e) :
           Statement(ASSIGN), variable(vname), expression(e) {}


AssignmentStatement::~AssignmentStatement() {
    delete(expression);
}

string& AssignmentStatement::getVariable() {
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

string Program::defaultName = "Default Program";

Program::Program(string& n, DeclarationList *dl,
        Statement *s)
    : name (n), declarations(dl), statement (s) {}

Program::Program()
    : name (Program::defaultName), 
    statement (NULL) {
    
}

string& Program::getName() {
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

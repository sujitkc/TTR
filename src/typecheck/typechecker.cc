#include <iostream>
#include "ast.hh"
#include "typechecker.hh"

using namespace std;
using namespace ast;

namespace typechecker {

template <typename T> Env<T>::Env(Env<T> *p) : parent(p) {}

template <typename T> Env<T> *Env<T>::getParent() {
    return parent;
}

template <typename T> void Env<T>::print() {
}

template <typename T> T& Env<T>::get(string vname) {

    if(table.find(vname) != table.end()) {
        return *table[vname];
    }
    if(parent != NULL) {
        return parent->get(vname);
    }
    throw ("Variable " + vname + " not found.");
}

template <typename T> void Env<T>::addMapping(string name, T *value) {
    if(table.find(name) == table.end()) {
        table[name] = value;
    }
    else {
        string m = "Env::addMapping : repeat declaration for name " + name + ".";
        throw m;
    }
}

template <typename T> Env<T>::~Env() {}

ValueEnv::ValueEnv(ValueEnv *p) : Env(p) {}

void ValueEnv::print() {
    for(auto &d : table) {
        
        cout << d.first << " : " << d.second << endl;
    }
}

Typechecker::Typechecker() {
    valueEnv = new ValueEnv(NULL);
    substitution.addType(*(Language::getInstance().getNativeType("int")));
    substitution.addType(*(Language::getInstance().getNativeType("bool")));
}

Typechecker::~Typechecker() {
    delete valueEnv;
}

ValueEnv& Typechecker::getValueEnv() {
    return *valueEnv;
}

TypeExpr& Typechecker::typecheckExpression(Expression& e) {
    switch(e.exprtype) {
        case VAR:
            return typecheckVar(dynamic_cast<Var&>(e));
        case NUM:
            return typecheckNum(dynamic_cast<Num&>(e));
        case BOOL:
            return typecheckBoolConst(dynamic_cast<BoolConst&>(e));
        case ADD:
            return typecheckAddExpression(dynamic_cast<AddExpression&>(e));
        case FUNCTIONCALL:
            return typecheckFunctionCall(dynamic_cast<FunctionCall&>(e));
        default:
            string m = "Typechecker::typecheckExpression : Unknown expression type!" + to_string(e.exprtype);
            throw m;
    }
}

TypeExpr& Typechecker::typecheckVar(Var& v) {
    return valueEnv->get(v.name);
}

TypeExpr& Typechecker::typecheckNum(Num& n) {
    return *(Language::getInstance().getNativeType("int"));
}

TypeExpr& Typechecker::typecheckBoolConst(BoolConst& n) {
    return *(Language::getInstance().getNativeType("bool"));
}

TypeExpr& Typechecker::typecheckAddExpression(AddExpression& add) {
    return *(Language::getInstance().getNativeType("int"));
}

TypeExpr& Typechecker::typecheckEqExpression(EqExpression& eq) {
    return *(Language::getInstance().getNativeType("int"));
}


TypeExpr& Typechecker::typecheckFunctionCall(FunctionCall& funccall) {
    TypeExpr& type = valueEnv->get(funccall.getName());
    FunctionType& fsig = (FunctionType&)(type);
    vector<Expression *> args = funccall.getArguments();
    vector<TypeExpr *> ptypes = fsig.getParameterTypes();
    if(ptypes.size() != args.size()) {
        throw "Typechecker::typecheckFunctionCall : function call " + funccall.getName() + " #arguments unequal to #parameters.";
    }
    /*
    for(unsigned int i = 0; i < args.size(); i++) {
        if(typecheckExpression(*(args[i])) != ptypes[i]) {
                throw "Typechecker::typecheckFunctionCall : function call " + funccall.getName() + " argument type unequal to parameter type on position " + to_string(i);
        }
    }
    */
    return fsig.getReturnType();
}

void Typechecker::typecheckStatement(Statement& stmt) {
    switch(stmt.stmttype) {
        case ASSIGN:
            return typecheckAssignment(dynamic_cast<AssignmentStatement&>(stmt));
        case SEQUENCE:
            return typecheckSequence(dynamic_cast<SequenceStatement&>(stmt));
        default:
            throw "Typechecker::typecheckStatement : Unknown statement type!";
    }
}

void Typechecker::typecheckAssignment(AssignmentStatement& stmt) {
    TypeExpr& ltype = valueEnv->get(stmt.getVariable());
    TypeExpr& rtype = typecheckExpression(*(stmt.getExpression()));
}

void Typechecker::typecheckSequence(SequenceStatement& stmt) {
    for(auto& statement : stmt.getStatements()) {
        typecheckStatement(*statement);
    }
}

void Typechecker::typecheckProgram(Program& program) {
    cout << "Typechecking " << program.getName() << endl;
    for(auto& d : program.getDeclarationList().getDeclarations()) {
        valueEnv->addMapping(d->getVariable(), &(d->getType()));
    }
    
    typecheckStatement(program.getStatement());
}

} // namespace typechecker

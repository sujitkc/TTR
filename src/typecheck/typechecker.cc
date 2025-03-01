#include <iostream>
#include "ast.hh"
#include "typechecker.hh"

using namespace std;
using namespace ast;

namespace typechecker {

template <typename T1, typename T2> Env<T1, T2>::Env(Env<T1, T2> *p) : parent(p) {}

template <typename T1, typename T2> Env<T1, T2> *Env<T1, T2>::getParent() {
    return parent;
}

template <typename T1, typename T2> void Env<T1, T2>::print() {
}

template <typename T1, typename T2> T2& Env<T1, T2>::get(T1 *key) {

    if(table.find(key) != table.end()) {
        return *(table[key]);
    }
    if(parent != NULL) {
        return parent->get(key);
    }
    char str[50];
    sprintf(str, "%llx", (long long int)key);
    throw ("Key " + keyToString(key) + " (" + str + ") not found.");
}

template <typename T1, typename T2> void Env<T1, T2>::addMapping(T1 *key, T2 *value) {
/*
        char str[50];
    sprintf(str, "%llx", (long long int)key);
    cout << "addMaping::key = " << str << endl;    
*/
        if(table.find(key) == table.end()) {
        table[key] = value;
    }
    else {
        string m = "Env::addMapping : repeat declaration for name " + keyToString(key) + ".";
        throw m;
    }
}

template <typename T1, typename T2> Env<T1, T2>::~Env() {}

SymbolTable::SymbolTable(SymbolTable *p) : Env(p) {}

string SymbolTable::keyToString(string* key) {
    return *key;
}

void SymbolTable::print() {
    for(auto &d : table) {
        
        cout << keyToString(d.first) << " (" << d.first << "): " << d.second << endl;
    }
}

TypeVarTable::TypeVarTable(TypeVarTable *p) : Env(p) {}

string TypeVarTable::keyToString(Expression * key) {
    return key->toString();
}

void TypeVarTable::print() {
    for(auto &d : table) {
        
        cout << keyToString(d.first) << " : " << d.second << endl;
    }
}

Typechecker::Typechecker() {
    valueEnv = new SymbolTable(NULL);
    typeVarTable = new TypeVarTable(NULL);
    substitution.addType(*(Language::getInstance().getNativeType("int")));
    substitution.addType(*(Language::getInstance().getNativeType("bool")));
}

Typechecker::~Typechecker() {
    delete valueEnv;
    delete typeVarTable;
}

SymbolTable& Typechecker::getSymbolTable() {
    return *valueEnv;
}

void Typechecker::attachTypeVartoExpression(Expression& e) {
    switch(e.exprtype) {
        case VAR:
            attachTypeVartoVar(dynamic_cast<Var&>(e));
        case NUM:
            attachTypeVartoNum(dynamic_cast<Num&>(e));
        case BOOL:
            attachTypeVartoBoolConst(dynamic_cast<BoolConst&>(e));
        case ADD:
            attachTypeVartoAddExpression(dynamic_cast<AddExpression&>(e));
        case FUNCTIONCALL:
            attachTypeVartoFunctionCall(dynamic_cast<FunctionCall&>(e));
        default:
            string m = "Typechecker::attachTypeVartoExpression : Unknown expression type!" + to_string(e.exprtype);
            throw m;
    }
}

void Typechecker::attachTypeVartoVar(Var& v) {

}

void Typechecker::attachTypeVartoNum(Num& n) {

}

void Typechecker::attachTypeVartoBoolConst(BoolConst& b) {

}

void Typechecker::attachTypeVartoAddExpression(AddExpression& add) {

}

void Typechecker::attachTypeVartoEqExpression(EqExpression& eq) {

}

void Typechecker::attachTypeVartoFunctionCall(FunctionCall& fcall) {

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
    return valueEnv->get(&(v.getName()));
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
    TypeExpr& type = valueEnv->get(&(funccall.getName()));
    FunctionType& fsig = (FunctionType&)(type);
    /*
    vector<Expression *> args = funccall.getArguments();
    vector<TypeExpr *> ptypes = fsig.getParameterTypes();
    if(ptypes.size() != args.size()) {
        throw "Typechecker::typecheckFunctionCall : function call " + funccall.getName() + " #arguments unequal to #parameters.";
    }
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
    TypeExpr& ltype = valueEnv->get(&(stmt.getVariable()));
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
        string& vname = d->getVariable();
/*
        char str[50];
    sprintf(str, "%llx", (long long int)&vname);
    cout << "typecheckProgram::vname = " << str << endl;
*/
        valueEnv->addMapping(&vname, &(d->getType()));
    }
    cout << "Value environment:" << endl;
    valueEnv->print(); 
    typecheckStatement(program.getStatement());
}

} // namespace typechecker

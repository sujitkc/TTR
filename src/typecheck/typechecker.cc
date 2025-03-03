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

    if(hasKey(key)) {
        return *(table[key]);
    }
    if(parent != NULL) {
        return parent->get(key);
    }
    
    char str[50];
    sprintf(str, "%llx", (long long int)key);
    throw ("Key " + keyToString(key) + " (" + str + ") not found.");
}

template <typename T1, typename T2> bool Env<T1, T2>::hasKey(T1 *key) {
    return table.find(key) != table.end();
}

template <typename T1, typename T2> void Env<T1, T2>::addMapping(T1 *key, T2 *value) {

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

string Substitution::valueToString(TypeExpr& type) {
	return type.toString();
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

void Typechecker::unify(TypeExpr& type1, TypeExpr& type2) {
    if(type1.typeType == TYPEVAR) {
        substitution.myunion(type2, type1);
    }
    else if(type1.typeType == TYPECONST) {
        if(type2.typeType == TYPECONST) {
            if(type1.getName() == type2.getName()) {}

        }
        else {
            throw "Typechecker::unify failed!";
        }
    }
    else if(type1.typeType == FUNCTIONTYPE) {

    }
}

SymbolTable& Typechecker::getSymbolTable() {
    return *valueEnv;
}

void Typechecker::attachTypeVartoExpression(Expression& e) {
    if(typeVarTable->hasKey(&e)) {
        throw "Typechecker::attachTypeVartoExpr - expression " +
                   e.toString() +
                   " already present in the type var table.";
    }

    switch(e.exprtype) {
        case VAR:
            attachTypeVartoVar(dynamic_cast<Var&>(e));
	    break;
        case NUM:
            attachTypeVartoNum(dynamic_cast<Num&>(e));
	    break;
        case BOOL:
            attachTypeVartoBoolConst(dynamic_cast<BoolConst&>(e));
	    break;
        case ADD:
            attachTypeVartoAddExpression(dynamic_cast<AddExpression&>(e));
	    break;
        case FUNCTIONCALL:
            attachTypeVartoFunctionCall(dynamic_cast<FunctionCall&>(e));
	    break;
        default:
            string m = "Typechecker::attachTypeVartoExpression : Unknown expression type!" + to_string(e.exprtype);
            throw m;
    }
}

void Typechecker::attachTypeVartoVar(Var& v) {
    TypeVar *tv = TypeVar::getNewTypeVar();
    typeVarTable->addMapping(&v, tv);
    substitution.addType(*tv);
}

void Typechecker::attachTypeVartoNum(Num& n) {
    TypeVar *tv = TypeVar::getNewTypeVar();
    typeVarTable->addMapping(&n, tv);
    substitution.addType(*tv);
    unify(*tv, *(Language::getInstance().getNativeType("int")));
}

void Typechecker::attachTypeVartoBoolConst(BoolConst& b) {
    TypeVar *tv = TypeVar::getNewTypeVar();
    typeVarTable->addMapping(&b, tv);
    substitution.addType(*tv);
    unify(*tv, *(Language::getInstance().getNativeType("bool")));
}

void Typechecker::attachTypeVartoAddExpression(AddExpression& add) {
    TypeVar *tv = TypeVar::getNewTypeVar();
    typeVarTable->addMapping(&add, tv);
    substitution.addType(*tv);
    unify(*tv, *(Language::getInstance().getNativeType("int")));
    attachTypeVartoExpression(add.getLeft());
    attachTypeVartoExpression(add.getRight());
}

void Typechecker::attachTypeVartoEqExpression(EqExpression& eq) {
    TypeVar *tv = TypeVar::getNewTypeVar();
    typeVarTable->addMapping(&eq, tv);
    substitution.addType(*tv);
    unify(*tv, *(Language::getInstance().getNativeType("bool")));
    attachTypeVartoExpression(eq.getLeft());
    attachTypeVartoExpression(eq.getRight());
}

void Typechecker::attachTypeVartoFunctionCall(FunctionCall& fcall) {
    TypeVar *tv = TypeVar::getNewTypeVar();
    typeVarTable->addMapping(&fcall, tv);
    substitution.addType(*tv);
    for(auto& arg : fcall.getArguments()) {
        attachTypeVartoExpression(*arg);
    }
}

void Typechecker::attachTypeVarInProgram(Program& program) {
    attachTypeVarInStatement(program.getStatement());    
}

void Typechecker::attachTypeVarInStatement(Statement& stmt) {
    switch(stmt.stmttype) {
        case ASSIGN:
            return attachTypeVarInAssignment(dynamic_cast<AssignmentStatement&>(stmt));
        case SEQUENCE:
            return attachTypeVarInSequence(dynamic_cast<SequenceStatement&>(stmt));
        default:
            throw "Typechecker::typecheckStatement : Unknown statement type!";
    }
}

void Typechecker::attachTypeVarInAssignment(AssignmentStatement& assign) {
	attachTypeVartoExpression(*(assign.getExpression()));
}

void Typechecker::attachTypeVarInSequence(SequenceStatement& seq) {
	for(auto& s : seq.getStatements()) {
		attachTypeVarInStatement(*s);
	}
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
    return *(Language::getInstance().getNativeType("bool"));
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
    attachTypeVarInProgram(program);
    cout << endl << "Substitution: " << endl;
    substitution.print();
    cout << endl;
    typecheckStatement(program.getStatement());
}

} // namespace typechecker

#include <iostream>
#include "ast.hh"
#include "unionfind.hh"
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
        
        cout << keyToString(d.first) << " (" << d.first << "): " << d.second->toString() << endl;
    }
}

TypeVarTable::TypeVarTable(TypeVarTable *p) : Env(p) {}

string TypeVarTable::keyToString(Expression * key) {
    return key->toString();
}

void TypeVarTable::print() {
    for(auto &d : table) {
        
        cout << keyToString(d.first) << " : " << d.second->toString() << endl;
    }
}

string Substitution::valueToString(TypeExpr& type) {
    return type.toString();
}

Substitution::~Substitution() {

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
    string reason = "";
    TypeExpr& rep1 = substitution.find(type1);
    TypeExpr& rep2 = substitution.find(type2);
    if(rep1.typeType == TYPEVAR) {
        substitution.myunion(rep2, rep1);
        return;
    }
    else if(rep1.typeType == TYPECONST) {
        if(rep2.typeType == TYPEVAR) {
                substitution.myunion(rep1, rep2);
        return;
        }
        else if(rep2.typeType == TYPECONST) {
            if(rep1.getName() == rep2.getName()) {
                return;
            }
        }
    reason = "rep1.type = " + to_string(rep1.typeType) +
               "; rep2.type = " + to_string(rep2.typeType);
    }
    else if(rep1.typeType == FUNCTIONTYPE) {
        if(rep2.typeType == TYPEVAR) {
            substitution.myunion(rep1, rep2);
        return;
        }
        else if(rep2.typeType == FUNCTIONTYPE) {
            FunctionType& ftype1 = (FunctionType&) rep1;
            FunctionType& ftype2 = (FunctionType&) rep2;
            unify(ftype1.getReturnType(), ftype2.getReturnType());
            vector<TypeExpr *> pars1 = ftype1.getParameterTypes();
            vector<TypeExpr *> pars2 = ftype2.getParameterTypes();
            if(pars1.size() == pars2.size()) {
                for(int i = 0; i < pars1.size(); i++) {
                    unify(*(pars1[i]), *(pars2[i]));
                }
        return;
            }
            reason = "rep1.type = " + to_string(rep1.typeType) + "; rep2.type = " + to_string(rep2.typeType);
        }
    }
    throw "Typechecker::unify failed!" + reason;
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
        case EQ:
            attachTypeVartoEqExpression(dynamic_cast<EqExpression&>(e));
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
        case EQ:
            return typecheckEqExpression(dynamic_cast<EqExpression&>(e));
        case FUNCTIONCALL:
            return typecheckFunctionCall(dynamic_cast<FunctionCall&>(e));
        default:
            string m = "Typechecker::typecheckExpression : \
                   Unknown expression type!" + to_string(e.exprtype);
            throw m;
    }
}

TypeExpr& Typechecker::typecheckVar(Var& v) {
    TypeExpr& ty = valueEnv->get(&(v.getName()));
    unify(typeVarTable->get(&v), ty);
    return ty;
}

TypeExpr& Typechecker::typecheckNum(Num& n) {
    TypeExpr *intType = Language::getInstance().getNativeType("int");
    unify(typeVarTable->get(&n), *intType);
    return *(intType);
}

TypeExpr& Typechecker::typecheckBoolConst(BoolConst& b) {
    TypeExpr *boolType = Language::getInstance().getNativeType("bool");
    unify(typeVarTable->get(&b), *boolType);
    return *(boolType);
}

TypeExpr& Typechecker::typecheckAddExpression(AddExpression& add) {
    TypeExpr *intType = Language::getInstance().getNativeType("int");
    unify(typeVarTable->get(&add), *intType);

    Expression& left = add.getLeft();
    TypeExpr& ltype = typecheckExpression(left);
    unify(typeVarTable->get(&left), *intType);

    Expression& right = add.getRight();
    TypeExpr& rtype = typecheckExpression(right);
    unify(typeVarTable->get(&right), *intType);

    return *(intType);
}

TypeExpr& Typechecker::typecheckEqExpression(EqExpression& eq) {
    TypeExpr *boolType = Language::getInstance().getNativeType("bool");
    TypeExpr& eqType = typeVarTable->get(&eq);
    unify(eqType, *boolType);

    Expression& left = eq.getLeft();
    TypeExpr& ltype = typecheckExpression(left);

    Expression& right = eq.getRight();
    TypeExpr& rtype = typecheckExpression(right);
    unify(ltype, rtype);

    return *(boolType);
}


TypeExpr& Typechecker::typecheckFunctionCall(FunctionCall& funccall) {
    TypeExpr& ftype = valueEnv->get(&(funccall.getName()));
    FunctionType& fsig = (FunctionType&)(ftype);
    vector<TypeExpr *> argTypes;
    vector<Expression *> args = funccall.getArguments();
    for(auto& arg : args) {
        TypeExpr& argtype = typecheckExpression(*arg);
        argTypes.push_back(&argtype);
    }
    TypeExpr& rtype = typeVarTable->get(&funccall);
    FunctionType *ftype2 = new FunctionType(&rtype, argTypes);
    substitution.addType(*ftype2);
    unify(*ftype2, fsig);
    return substitution.find(rtype);
}

void Typechecker::typecheckStatement(Statement& stmt) {
    switch(stmt.stmttype) {
        case ASSIGN:
            typecheckAssignment(dynamic_cast<AssignmentStatement&>(stmt));
            return;
        case SEQUENCE:
            typecheckSequence(dynamic_cast<SequenceStatement&>(stmt));
            return;
        default:
            throw "Typechecker::typecheckStatement : Unknown statement type!";
    }
}

void Typechecker::typecheckAssignment(AssignmentStatement& stmt) {
    TypeExpr& ltype = valueEnv->get(&(stmt.getVariable()));
    TypeExpr& rtype = typecheckExpression(*(stmt.getExpression()));
    unify(ltype, rtype);
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
        valueEnv->addMapping(&vname, &(d->getType()));
        try {
            substitution.addType(d->getType());
        }
        catch(...) {
		cout << "addType failed for " << vname << " : " << d->getType() << endl;
	}
    }
    cout << "Value environment:" << endl;
    valueEnv->print();
    attachTypeVarInProgram(program);
    cout << endl << "Substitution (before): " << endl;
    substitution.print();
    cout << endl;
    typecheckStatement(program.getStatement());
    cout << endl << "Substitution (after): " << endl;
    substitution.print();
    cout << endl;
}

} // namespace typechecker

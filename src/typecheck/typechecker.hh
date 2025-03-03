#ifndef TYPECHECKER
#define TYPECHECKER

#include <map>
#include <set>
#include "ast.hh"
#include "substitution.cc"

using namespace std;
using namespace ast;

namespace typechecker {
template<typename T1, typename T2> class Env {
    protected:
        map<T1 *, T2 *> table;
        Env<T1, T2> *parent;

    public:
        Env(Env<T1, T2> *parent);
        virtual Env<T1, T2> *getParent();
        virtual T2& get(T1 *);
        virtual bool hasKey(T1 *);
        virtual void addMapping(T1 *, T2 *);
        virtual string keyToString(T1 *) = 0;
        virtual void print() = 0;
        virtual ~Env();
};

class SymbolTable : public Env<string, TypeExpr> {
    public:
        SymbolTable(SymbolTable *parent);
        virtual void print();
        virtual string keyToString(string *);
};

class TypeVarTable : public Env<Expression, TypeExpr> {
    public:
        TypeVarTable(TypeVarTable *parent);
        virtual void print();
        virtual string keyToString(Expression *);
};

class Substitution : public UnionFind<TypeExpr&> {
    public:
        virtual string valueToString(TypeExpr&);
};

class Typechecker {
    private:
        SymbolTable *valueEnv;
        TypeVarTable *typeVarTable;
        Substitution substitution;

        void unify(TypeExpr&, TypeExpr&);
        void attachTypeVartoExpression(Expression&);
        void attachTypeVartoVar(Var&);
        void attachTypeVartoNum(Num&);
        void attachTypeVartoBoolConst(BoolConst&);
        void attachTypeVartoAddExpression(AddExpression&);
        void attachTypeVartoEqExpression(EqExpression&);
        void attachTypeVartoFunctionCall(FunctionCall&);
        void attachTypeVarInProgram(Program&);
        void attachTypeVarInStatement(Statement&);
        void attachTypeVarInAssignment(AssignmentStatement&);
        void attachTypeVarInSequence(SequenceStatement&);
    public:
        Typechecker();
        ~Typechecker();

        SymbolTable& getSymbolTable();

        TypeExpr& typecheckExpression(Expression&);
        TypeExpr& typecheckVar(Var&);
        TypeExpr& typecheckNum(Num&);
        TypeExpr& typecheckBoolConst(BoolConst&);
        TypeExpr& typecheckAddExpression(AddExpression&);
        TypeExpr& typecheckEqExpression(EqExpression&);
        TypeExpr& typecheckFunctionCall(FunctionCall&);

        void typecheckStatement(Statement&);
        void typecheckAssignment(AssignmentStatement&);
        void typecheckSequence(SequenceStatement&);

        void typecheckProgram(Program&);
};
} // namespace typechecker
#endif

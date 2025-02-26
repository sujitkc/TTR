#ifndef TYPECHECKER
#define TYPECHECKER

#include <map>
#include <set>
#include "ast.hh"
#include "substitution.cc"

using namespace std;
using namespace ast;

namespace typechecker {
template<typename T> class Env {
    protected:
        map<string, T *> table;
        Env<T> *parent;
    public:
        Env(Env<T> *parent);
        virtual Env<T> *getParent();
        virtual T& get(string);
        virtual void addMapping(string, T *);
        virtual void print() = 0;
        virtual ~Env();
};

class ValueEnv : public Env<TypeExpr> {
    public:
        ValueEnv(ValueEnv *parent);
        virtual void print();
};

class Substitution : public UnionFind<TypeExpr&> {};
class Typechecker {
    private:
        ValueEnv *valueEnv;
        Substitution substitution;
    public:
        Typechecker();
        ~Typechecker();

        ValueEnv& getValueEnv();

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

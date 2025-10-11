#ifndef Z3SOLVER_HH
#define Z3SOLVER_HH

#include<memory>
#include <stack>
#include<string>

#include "astvisitor.hh"
#include "solver.hh"
#include "z3++.h"

using namespace std;

class Z3InputMaker : public ExprVisitor {
    private:
        z3::context& ctx;
	stack<z3::expr> theStack;

    public:
        Z3InputMaker(z3::context&);
        z3::expr makeZ3Input(unique_ptr<Expr> expr);

    private:
	void visit(const Var &node);
        void visit(const FuncCall &node);
        void visit(const Num &node);
        void visit(const String &node);
        void visit(const Set &node);
        void visit(const Map &node);
        void visit(const Tuple &node);
        // virtual void visit(const PolymorphicFuncCall &node);
};

class Z3Solver : public Solver {
    public:
        Result solve(unique_ptr<Expr>) const;
};
#endif

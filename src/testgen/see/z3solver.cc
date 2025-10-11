#include "z3solver.hh"

Z3InputMaker::Z3InputMaker(z3::context& c) : ctx(c) {}

z3::expr Z3InputMaker::makeZ3Input(unique_ptr<Expr> expr) {
    z3::expr x = ctx.int_const("x");
    return x;
}

void visit(const Var &node) {
}

void visit(const FuncCall &node) {
}

void visit(const Num &node) {
}

void visit(const String &node) {
}

void visit(const Set &node) {
}

void visit(const Map &node) {
}

void visit(const Tuple &node) {
}

// virtual void visit(const PolymorphicFuncCall &node);


Result Solver::solve(unique_ptr<Expr> formula) const {
    z3::context ctx;
    Z3InputMaker inputMaker(ctx);
    inputMaker.makeZ3Input(std::move(formula));
    return Result(false, map<string, unique_ptr<ResultValue> >());
}

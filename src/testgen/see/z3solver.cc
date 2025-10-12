#include "z3solver.hh"

Z3InputMaker::Z3InputMaker(z3::context& c) : ctx(c) {}

z3::expr Z3InputMaker::makeZ3Input(unique_ptr<Expr> expr) {
    z3::expr x = ctx.int_const("x");
    return x;
}

void Z3InputMaker::visit(const Var &node) {
    z3::expr x = ctx.int_const(node.name.c_str());
    theStack.push(x);
    variables.push_back(x);
}

void Z3InputMaker::visit(const FuncCall &node) {
}

void Z3InputMaker::visit(const Num &node) {
    theStack.push(ctx.int_val(node.value));
}

void Z3InputMaker::visit(const String &node) {
    theStack.push(ctx.string_val(node.value));
}

void Z3InputMaker::visit(const Set &node) {
}

void Z3InputMaker::visit(const Map &node) {
}

void Z3InputMaker::visit(const Tuple &node) {
}

// virtual void visit(const PolymorphicFuncCall &node);


Result Solver::solve(unique_ptr<Expr> formula) const {
    z3::context ctx;
    Z3InputMaker inputMaker(ctx);
    inputMaker.makeZ3Input(std::move(formula));
    z3::solver s(ctx);

    if(s.check() == z3::sat) {
        z3::model m = s.get_model();
        map<string, unique_ptr<ResultValue> > var_values;
    }
    return Result(false, map<string, unique_ptr<ResultValue> >());
}

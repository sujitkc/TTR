#include "see.hh"

unique_ptr<Expr> computePathConstraint(vector<unique_ptr<Expr>> C) {
	return make_unique<Num>(0);
}

// If the statement is a call to an API function, then none of its parameters
// should be variables whose values are symbolic expression.
bool isReady(unique_ptr<Stmt> s) {
	return false;
}

// If the expression is a variable and is not mapped to a symbolic expression
// then the expression is ready. For any other type of expression, if it has
// a variable in it, e.g. a function call with a variable being passed as an
// argument, then all such expression should be ready.
bool isReady(unique_ptr<Expr> e) {
	return false;
}

// If an expression has a symbolic variable as one of its subexpressions, then
// it is symbolic expression.
bool isSymbolic(unique_ptr<Expr> e) {
	return false;
}

void SEE::execute() {}

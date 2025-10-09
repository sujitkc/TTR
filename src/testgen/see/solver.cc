#include "solver.hh"

ResultValue::ResultValue(ResultType t) : type(t) {
}

BoolResultValue::BoolResultValue(bool v) : ResultValue(ResultType::BOOL), value(v) {
}

IntResultValue::IntResultValue(int v) : ResultValue(ResultType::INT), value(v) {
}

Result::Result(bool tf, map<string, unique_ptr<ResultValue> > m) : isSat(tf), model(std::move(m)) {
}

Result Solver::solve(unique_ptr<Expr> formula) const {
  return Result(false, map<string, unique_ptr<ResultValue> >());
}

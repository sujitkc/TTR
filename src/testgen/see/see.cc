#include "see.hh"

ResultValue::ResultValue(ResultType t) : type(t) {
}

BoolResultValue::BoolResultValue(bool v) : ResultValue(ResultType::BOOL), value(v) {
}

IntResultValue::IntResultValue(int v) : ResultValue(ResultType::INT), value(v) {
}

SymExResult::SymExResult(bool tf, map<string, unique_ptr<ResultValue> > m) : isSat(tf), model(std::move(m)) {
}



#include "tester.hh"

void Tester::generateTest() {}


unique_ptr<Program> Tester::generateCTC(unique_ptr<Program> atc) {
    return atc;
}

unique_ptr<Program> Tester::generateATC(unique_ptr<Spec> spec, vector<string> ts) {
	vector<unique_ptr<Stmt>> stmts;
    return make_unique<Program>(std::move(stmts));
}

unique_ptr<Program> Tester::rewriteATC(unique_ptr<Program> atc) {
    return atc;
}

#ifndef TESTER_HH
#define TESTER_HH

#include <memory>
#include <string>
#include <vector>

#include "ast.hh"

using namespace std;
class Tester {
    private:
        unique_ptr<Program> generateCTC(unique_ptr<Program>);
        unique_ptr<Program> generateATC(unique_ptr<Spec>, vector<string>);
        unique_ptr<Program> rewriteATC(unique_ptr<Program>);
    public:
        void generateTest();
};
#endif
